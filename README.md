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
include/        example_secrets.h, example_secret_ota.h, generierte version.h
bootloader/     Optiboot für den Mega, Voraussetzung für OTA
ota_upload.py   PlatformIO-Target `ota`: Upload über das Netzwerk
```

Jedes PlatformIO-Environment baut genau ein File aus `src/` (`build_src_filter = +<${PIOENV}.cpp>`).

| Environment | Knoten  |
|-------------|---------|
| `og_ost`    | OG Ost  |
| `og_west`   | OG West |
| `eg_ost`    | EG Ost  |
| `eg_west`   | EG West |
| `ug`        | UG      |
| `garage`    | Garage, mit Codeschloss (Wiegand-Keypad, Sperre nach 5 Fehlversuchen, Events an HA; Haustür-Code: 10 s Countdown mit Piepsern, dann Event `door_open`, HA öffnet das Nuki) |
| `test`      | Testaufbau |

## Bauen und Flashen

Voraussetzung ist [PlatformIO](https://platformio.org/).

```sh
pio run -e og_ost                    # bauen
pio run -e og_ost -t upload          # flashen
pio device monitor                   # serielle Konsole, 115200 Baud
```

Für alle Knoten muss vorher `include/example_secret_ota.h` nach `include/secret_ota.h` kopiert und
das OTA-Passwort gesetzt werden. Für `garage` zusätzlich `include/example_secrets.h` nach
`include/secrets.h` mit den Türcodes. Beide Dateien sind in `.gitignore` eingetragen.

Die Knoten laufen mit [Optiboot](https://github.com/Optiboot/optiboot) statt dem
Original-Bootloader des Mega, `upload` über USB spricht deshalb das Protokoll `arduino` mit 115200 Baud.
Ein Knoten mit dem alten Bootloader lässt sich so nicht mehr per USB flashen, siehe unten.

### Flashen über das Netzwerk (OTA)

Mit der Library [ArduinoOTA](https://github.com/JAndrassy/ArduinoOTA) nimmt jeder Knoten auf
Port 65280 ein neues Image per HTTP an (Benutzer `arduino`, Passwort aus `secret_ota.h`):

```sh
pio run -e og_ost -t ota                           # IP aus custom_ota_host in platformio.ini
pio run -t ota                                     # alle Knoten aus default_envs nacheinander
OTA_HOST=192.168.178.50 pio run -e test -t ota     # Environment ohne custom_ota_host
```

Ohne `pio` im PATH geht dasselbe mit `./ota_all.sh` (alle Knoten) oder `./ota_all.sh og_ost ug`.

`OTA_HOST` gilt nur für Environments ohne `custom_ota_host`, sonst ginge beim Lauf über alle Knoten
jedes Image an dieselbe Adresse. Schlägt ein Knoten fehl, macht `pio run -t ota` mit den übrigen weiter
und zeigt am Ende eine Übersicht. `pio run` ohne `-e` baut ebenfalls nur noch die Knoten aus `default_envs`.

In VS Code liegt das Target unter PlatformIO → Project Tasks → `<knoten>` → Custom → „Upload OTA“,
dafür muss `custom_ota_host` gesetzt sein. „Remote“ in PlatformIO ist etwas anderes (PIO Remote) und
hilft hier nicht. Die Logausgaben gibt es weiterhin nur über USB.

Die IP steht in Home Assistant bei der Diagnose-Entität „IP-Adresse“. Fest eintragen lohnt sich, wenn
die Fritzbox der MAC des Knotens immer dieselbe Adresse gibt. Der Knoten schreibt das Image
in die obere Hälfte des Flash, Optiboot kopiert es danach an den Anfang und startet neu
(Reset-Grund `ota`). Ein Image darf deshalb höchstens 127,5 KB groß sein, `board_upload.maximum_size`
lässt größere Builds schon beim Bauen scheitern. Bricht der Upload ab, bleibt die alte Firmware
unverändert. Das Target prüft nicht, ob das Image zum Knoten passt: Die richtige IP ist Sache des Aufrufers.

Während des Uploads steht die Logik des Knotens für einige Sekunden. Der Watchdog bleibt aktiv,
hängt der Upload, startet der Knoten mit der alten Firmware neu.

### Bootloader brennen

Einmal pro Knoten, mit einem ISP-Programmer (USBasp oder ein zweiter Arduino mit dem Sketch
„ArduinoISP“). Der Mega wird dabei komplett gelöscht, danach die Firmware einmal per USB flashen.

```sh
pio run -e bootloader -t bootloader     # Fuses setzen, Optiboot brennen
pio run -e og_ost -t upload             # Firmware per USB
```

Das Environment `bootloader` ist auf einen Uno mit ArduinoISP an `/dev/ttyACM0` eingestellt,
einen anderen Port mit `--upload-port` angeben (Port mit `pio device list` prüfen). Für USBasp stehen
die Zeilen auskommentiert in `platformio.ini`.
Auf dem Uno gehört nach dem Hochladen von ArduinoISP ein 10-µF-Elko zwischen RESET und GND.
Verkabelung: Uno D13/D12/D11 an SCK/MISO/MOSI des Mega (D52/D50/D51 oder ICSP), Uno D10 an RESET des Mega. Gesetzt werden lfuse `0xFF`, hfuse `0xD6` (1 KB Bootsektor,
EEPROM bleibt beim Löschen erhalten), efuse `0xFD`, Lock `0x0F`. Das Image
`bootloader/optiboot_atmega2560.hex` stammt aus [my_boards](https://github.com/JAndrassy/my_boards)
von Juraj Andrassy, es ist Optiboot 8 mit der Funktion `copy_flash_pages`, die OTA braucht.

Der ISP-Stecker (6 Pins neben dem ATmega) wird vom Ethernet-Shield belegt, das Shield muss dafür ab.
Alternativ an den Mega-Pins 50 (MISO), 51 (MOSI), 52 (SCK) und RESET anschließen. Beim Brennen hängen
alle Ausgänge offen, die Relais also vorher stromlos machen oder den Knoten ausbauen.

Ein Knoten ohne Optiboot läuft mit der neuen Firmware weiter, OTA bleibt dort aber abgeschaltet
(Diagnose-Entität „OTA bereit“ = 0). Der Knoten prüft dazu die Fuses, denn mit dem alten Bootloader
würde der Sprung nach `copy_flash_pages` irgendwo landen.

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
Revision, letzter Reset (`power_on`, `watchdog`, `restart`, `ota`, `reset`), Uptime, freier RAM,
Anzahl der MQTT-Verbindungen und der Ethernet-Resets, IP- und MAC-Adresse, ob der Notbetrieb aktiv ist
und ob OTA bereit ist. Die übrigen Entitäten hängen bewusst an keinem Gerät, damit
Home Assistant ihren Namen nicht um den Gerätenamen ergänzt.

Der Original-Bootloader des Mega löscht das Reset-Register `MCUSR`, Optiboot zumindest das Watchdog-Bit. Der Watchdog läuft deshalb im Modus
„erst Interrupt, dann Reset“: Der Interrupt merkt sich den Grund in RAM, das einen Reset übersteht.

## Robustheit

Ein Watchdog startet den Knoten neu, wenn er hängt (Interrupt nach 8 s, Reset nach weiteren 8 s). Netzwerk- und MQTT-Timeouts sind so
gewählt, dass sie unterhalb dieser Zeit bleiben. Bei Verbindungsverlust versucht der Knoten alle 20 s
neu zu verbinden und arbeitet die lokale Logik in der Zwischenzeit weiter ab. Klappt das dreimal hintereinander
nicht (etwa 1 min), setzt er den W5100 über dessen RST-Leitung (D12) zurück und holt sich die Adresse neu per DHCP;
beim Start passiert das immer. Der Watchdog allein hilft hier nicht, weil RST des W5100 nur an D12 hängt und ein
Reset des Mega den Chip nicht zurücksetzt. Die Diagnose-Entität „Ethernet-Resets“ zählt die Resets seit dem Start.
Ist der Broker länger weg, kommt so jede Minute ein Reset, das schadet nicht.

PWM-Ausgänge werden als Allererstes in `setup()` auf LOW gezogen. Bis dahin hängt der Pin offen,
was ein Leistungssteller als Vollgas lesen kann. Die Zeit im Bootloader überbrückt das nicht,
dafür braucht der Pin einen Pull-down-Widerstand (z. B. 10 kΩ nach GND).

### Notbetrieb

Home Assistant schickt jede Minute ein Lebenszeichen auf `nodes/heartbeat` (Automation
„Controller Heartbeat“). Bleibt es 10 Minuten aus (gezählt auch ab dem Start des Knotens), gehen Ausgänge mit
einem Notwert in diesen Zustand: die Heizventile auf, damit ohne die Thermostate in Home Assistant
nicht kalt wird. Kommt das Lebenszeichen zurück, nehmen sie wieder den Wert an, den Home Assistant
zuletzt gesetzt hat. Befehle während des Notbetriebs werden dafür vorgemerkt.

### Zustand über Neustart und Verbindungsabbruch

Kommt ein Befehl auf `nodes/cmd/<name>` bei einem Ausgang an, schreibt der Knoten den Zielwert retained nach
`nodes/soll/<name>`. Nach jedem Verbinden, also beim Start und nach einem Abbruch, liest er von dort die
Werte seiner Ausgänge. So überstehen Lichter und Ventile einen Neustart, und ein während des Abbruchs
verpasster Befehl wird nachgeholt. Das gilt auch für Ausgänge unter demselben Namen auf mehreren Knoten
(`LI_GA_L1`, `LI_EG_GA`). Ausgenommen sind Ausgänge hinter einem Timer und die Relais der Rollos, Timer,
Rollos und PWM selbst sowie Schalten über die serielle Konsole. Die Topics bleiben nur 10 s abonniert;
ein Befehl aus dieser Zeit gilt vor dem gespeicherten Wert.

Nach einem Stromausfall geht also wieder an, was vorher an war. Ein Taster während eines Broker-Ausfalls
schaltet nur lokal und wird beim Verbinden vom gespeicherten Wert überschrieben.

## Serielle Konsole

Neben den Logausgaben nimmt die serielle Schnittstelle Befehle im Format `!<typ>!<name>!<wert>$` an:

```
!c!LI_OG_BA!1$      Ausgang schalten
!r!TI_OG_BA!$       Zustand lesen
!restart!og_ost!$   Knoten neu starten
```

## Fremd-Libraries

PlatformIO lädt alle Libraries mit fester Version über `lib_deps`:
Ethernet, PubSubClient, [ArduinoOTA](https://github.com/JAndrassy/ArduinoOTA), [OneWire](https://www.pjrc.com/teensy/td_libs_OneWire.html),
[DallasTemperature](https://github.com/milesburton/Arduino-Temperature-Control-Library) (DS18B20),
[DHT22](https://github.com/dvarrel/DHT22) und
[Wiegand](https://github.com/monkeyboard/Wiegand-Protocol-Library-for-Arduino). Die letzten beiden
sind nicht im PlatformIO-Registry und werden direkt über den Git-Tag eingebunden.

## Lizenz

[MIT](LICENSE)
