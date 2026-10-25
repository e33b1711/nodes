# nodes

Firmware für die Hausautomation: mehrere Arduino-Mega-Knoten mit Ethernet-Shield, verbunden über MQTT
mit [Home Assistant](https://www.home-assistant.io/). Jeder Knoten liest Taster und Temperatursensoren
und schaltet Relais für Licht, Rollos, Dachfenster und Heizventile. Die Entitäten meldet er per
[MQTT Discovery](https://www.home-assistant.io/integrations/mqtt/#mqtt-discovery) selbst bei Home Assistant an.

Die Tasterlogik läuft lokal auf dem Knoten, die Grundfunktionen bleiben also auch ohne Home Assistant erhalten.

## Aufbau

```
src/            ein File pro Knoten: Pinbelegung, Entitäten, Tasterlogik
lib/node_lib/   gemeinsamer Code aller Knoten
include/        example_secrets.h, generierte version.h
```

Jedes PlatformIO-Environment baut genau ein File aus `src/` (`build_src_filter = +<${PIOENV}.cpp>`).

| Environment | Knoten  |
|-------------|---------|
| `og_ost`    | OG Ost  |
| `og_west`   | OG West |
| `eg_ost`    | EG Ost  |
| `eg_west`   | EG West |
| `ug`        | UG      |
| `garage`    | Garage, mit Codeschloss (Wiegand-Keypad, Sperre nach 5 Fehlversuchen, Events an HA) |
| `test`      | Testaufbau |

## Bauen und Flashen

Voraussetzung ist [PlatformIO](https://platformio.org/).

```sh
pio run -e og_ost                    # bauen
pio run -e og_ost -t upload          # flashen
pio device monitor                   # serielle Konsole, 115200 Baud
```

Für `garage` muss vorher `include/example_secrets.h` nach `include/secrets.h` kopiert und
mit den Türcodes befüllt werden. Die Datei ist in `.gitignore` eingetragen.

Vor jedem Build schreibt `auto_firmware_version.py` den aktuellen Git-Hash nach `include/version.h`.
Der Knoten veröffentlicht ihn als Diagnose-Entität, mit dem Präfix `dirty_`, wenn beim Build
uncommittete Änderungen vorhanden waren.

## Einen Knoten konfigurieren

Ein Knoten-File legt Tabellen an, die `node_lib` abarbeitet:

```cpp
uint8_t mac[] = {0xDE, 0xBB, 0x7E, 0xE1, 0xCC, 0x04};
node_t node_info = {"og_ost", mac};

output_t outputs[] = {
        // Name, Pin, invertiert, HA-Typ, Name in Home Assistant, Wert ohne Home Assistant
        {"LI_OG_BA", 42, 1, HA_LIGHT, "Badezimmer"},
        {"RO_OG_BA_UP", 49, 1},  // ohne Label: nicht in Home Assistant
        {"RO_OG_BA_DO", 43, 1},
        {"V_OG_BA", 37, true, HA_SWITCH, "Heizventil Badezimmer", 1},  // Notbetrieb: auf
};
const int num_outputs = COUNT_OF(outputs);

rollo_t rollos[] = {
        // Name, Laufzeit in ms, Rollo (true) oder Fenster (false), Label
        {"RO_OG_BA", 31000, true, "Rollo Bad"},  // steuert RO_OG_BA_UP / RO_OG_BA_DO
};

void user_logic() {
    long_short(3, "LI_OG_SZ", 0, "LI_OG_SZ", 3);  // Taster 3: lang aus, kurz umschalten
    rollo_lock(11, 0, "RO_OG_BA");                // Taster 11: Rollo hoch
}
```

Verfügbare Bausteine:

| Typ        | Zweck |
|------------|-------|
| `switch_t` | Tastereingang; erkennt Drücken, kurzes und langes Loslassen. Mit Label: öffentlich als Sensor, sonst als Event-Entität (`press`, `short`, `long`) in Home Assistant |
| `output_t` | Digitaler Ausgang (Relais) |
| `rollo_t`  | Rollo/Fenster aus zwei Ausgängen `<name>_UP` und `<name>_DO`, optional mit Endschaltern |
| `ntimer_t` | Schaltet einen Ausgang für eine feste Zeit ein (Treppenlicht) |
| `pwm_t`    | PWM-Ausgang mit Maximalwert und automatischer Abschaltung |
| `temp_t`   | DHT22 (Temperatur und Feuchte) oder DS18B20 (Temperatur, ein Sensor pro Pin) |

Alle Tabellen müssen definiert sein, gegebenenfalls leer (`pwm_t pwms[] = {};`).
Jeder Pin darf nur einmal belegt werden, sonst bleibt der Knoten beim Start mit einer Fehlermeldung stehen.

Ausgänge, die auf einem anderen Knoten liegen, werden in `user_logic()` einfach über ihren Namen angesprochen:
`write_any()` schickt den Befehl über MQTT, der zuständige Knoten führt ihn aus.

## MQTT

Broker-Adresse und Port stehen in `lib/node_lib/node.h`.

| Topic                                  | Richtung | Inhalt |
|----------------------------------------|----------|--------|
| `nodes/cmd/<name>`                     | → Knoten | Befehl, z. B. `1`, `0`, `ON`, `OFF`, `UP`, `DOWN`, `STOP` oder ein Zahlenwert |
| `nodes/read/<name>`                    | → Knoten | fordert den aktuellen Zustand an |
| `nodes/state/<name>`                   | Knoten → | Zustand; wird bei Änderung und zusätzlich periodisch gesendet |
| `nodes/<knoten>/status`                | Knoten → | `online` / `offline` (Last Will), retained |
| `homeassistant/<component>/<id>/config`| Knoten → | Discovery-Konfiguration, retained |
| `homeassistant/status`                 | → Knoten | `online` nach einem HA-Neustart: Knoten schicken Discovery und alle Zustände neu |
| `nodes/heartbeat`                      | → Knoten | Lebenszeichen von Home Assistant, jede Minute (siehe Notbetrieb) |

Rollos verwenden Positionen: `0` = offen, `100` = geschlossen, `50` = Stopp.
Echte Rollos (`is_rollo = true`) lassen sich zusätzlich über `nodes/cmd/<name>_POS` auf eine Position
in Prozent fahren. Die Position wird über die Laufzeit mitgerechnet und bei jeder vollen Fahrt auf oder zu
neu abgeglichen. Beim Start des Knotens fahren echte Rollos zum Abgleich ganz auf.

## Diagnose

Jeder Knoten erscheint in Home Assistant als Gerät `arduino_<knoten>` mit Diagnose-Entitäten:
Revision, letzter Reset (`power_on`, `watchdog`, `restart`, `reset`), Uptime, freier RAM,
Anzahl der MQTT-Verbindungen, IP- und MAC-Adresse und ob der Notbetrieb aktiv ist. Die übrigen Entitäten hängen bewusst an keinem Gerät, damit
Home Assistant ihren Namen nicht um den Gerätenamen ergänzt.

Der Bootloader des Mega löscht das Reset-Register `MCUSR`. Der Watchdog läuft deshalb im Modus
„erst Interrupt, dann Reset“: Der Interrupt merkt sich den Grund in RAM, das einen Reset übersteht.

## Robustheit

Ein Watchdog startet den Knoten neu, wenn er hängt (Interrupt nach 8 s, Reset nach weiteren 8 s). Netzwerk- und MQTT-Timeouts sind so
gewählt, dass sie unterhalb dieser Zeit bleiben. Bei Verbindungsverlust versucht der Knoten alle 20 s
neu zu verbinden und arbeitet die lokale Logik in der Zwischenzeit weiter ab.

PWM-Ausgänge werden als Allererstes in `setup()` auf LOW gezogen. Bis dahin hängt der Pin offen,
was ein Leistungssteller als Vollgas lesen kann. Die Zeit im Bootloader überbrückt das nicht,
dafür braucht der Pin einen Pull-down-Widerstand (z. B. 10 kΩ nach GND).

### Notbetrieb

Home Assistant schickt jede Minute ein Lebenszeichen auf `nodes/heartbeat` (Automation
„Controller Heartbeat“). Bleibt es 10 Minuten aus (gezählt auch ab dem Start des Knotens), gehen Ausgänge mit
einem Notwert in diesen Zustand: die Heizventile auf, damit ohne die Thermostate in Home Assistant
nicht kalt wird. Kommt das Lebenszeichen zurück, nehmen sie wieder den Wert an, den Home Assistant
zuletzt gesetzt hat. Befehle während des Notbetriebs werden dafür vorgemerkt.

## Serielle Konsole

Neben den Logausgaben nimmt die serielle Schnittstelle Befehle im Format `!<typ>!<name>!<wert>$` an:

```
!c!LI_OG_BA!1$      Ausgang schalten
!r!TI_OG_BA!$       Zustand lesen
!restart!og_ost!$   Knoten neu starten
```

## Fremd-Libraries

PlatformIO lädt alle Libraries mit fester Version über `lib_deps`:
Ethernet, PubSubClient, [OneWire](https://www.pjrc.com/teensy/td_libs_OneWire.html),
[DallasTemperature](https://github.com/milesburton/Arduino-Temperature-Control-Library) (DS18B20),
[DHT22](https://github.com/dvarrel/DHT22) und
[Wiegand](https://github.com/monkeyboard/Wiegand-Protocol-Library-for-Arduino). Die letzten beiden
sind nicht im PlatformIO-Registry und werden direkt über den Git-Tag eingebunden.

## Lizenz

[MIT](LICENSE)
