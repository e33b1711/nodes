#include "node.h"
#include "switch.h"
#include "output.h"
#include "temp.h"
#include "timer.h"
#include "rollo.h"
#include "pwm.h"

// home assistant mqtt discovery: everything that has a label is announced

// for states that are sent by user logic, override in the node file
void user_discovery() __attribute__((weak));
void user_discovery() {}

void add_topic(String &config,
               const __FlashStringHelper *key,
               const __FlashStringHelper *base,
               String id) {
    config += F(",\"");
    config += key;
    config += F("\":\"nodes/");
    config += base;
    config += id;
    config += '"';
}

void announce(ha_t type, String id, String label, int max_value) {
    String component;
    String config;
    config.reserve(450);

    config += F("{\"name\":\"");
    config += label;
    config += F("\",\"uniq_id\":\"");
    config += id;
    config += F("\",\"avty_t\":\"nodes/");
    config += node_info.unit_name;
    config += F("/status\"");

    switch (type) {
        case HA_LIGHT:
        case HA_SWITCH:
        case HA_SIREN:
        case HA_BINARY_SENSOR:
            component = F("light");
            if (type == HA_SWITCH)
                component = F("switch");
            if (type == HA_SIREN)
                component = F("siren");
            if (type == HA_BINARY_SENSOR)
                component = F("binary_sensor");
            add_topic(config, F("stat_t"), F("state/"), id);
            if (type != HA_BINARY_SENSOR)
                add_topic(config, F("cmd_t"), F("cmd/"), id);
            if (type == HA_SIREN)
                config += F(",\"cmd_tpl\":\"{{ value }}\"");
            config += F(",\"pl_on\":\"1\",\"pl_off\":\"0\"");
            break;

        case HA_COVER:
            component = F("cover");
            add_topic(config, F("pos_t"), F("state/"), id);
            add_topic(config, F("cmd_t"), F("cmd/"), id);
            config
                    += F(",\"pl_open\":\"0\",\"pl_cls\":\"100\",\"pl_stop\":\"50\",\"pos_open\":0,"
                         "\"pos_clsd\":100");
            break;

        case HA_TEMPERATURE:
        case HA_HUMIDITY:
            component = F("sensor");
            add_topic(config, F("stat_t"), F("state/"), id);
            if (type == HA_TEMPERATURE)
                config += F(",\"dev_cla\":\"temperature\",\"unit_of_meas\":\"°C\"");
            else
                config += F(",\"dev_cla\":\"humidity\",\"unit_of_meas\":\"%\"");
            config += F(",\"stat_cla\":\"measurement\",\"sug_dsp_prc\":1");
            // a failed sensor sends nan
            config += F(
                    ",\"val_tpl\":\"{% set v = value.strip() | lower %}"
                    "{% if v in ['nan', ''] %}{{ None }}{% else %}{{ v | float }}{% endif %}\"");
            break;

        case HA_NUMBER:
            component = F("number");
            add_topic(config, F("stat_t"), F("state/"), id);
            add_topic(config, F("cmd_t"), F("cmd/"), id);
            config += F(",\"min\":0,\"max\":");
            config += String(max_value, DEC);
            break;

        case HA_SENSOR:
            component = F("sensor");
            add_topic(config, F("stat_t"), F("state/"), id);
            config += F(",\"stat_cla\":\"measurement\"");
            break;

        case HA_DIAGNOSTIC:
            component = F("sensor");
            add_topic(config, F("stat_t"), F("state/"), id);
            config += F(",\"ent_cat\":\"diagnostic\"");
            break;

        default:
            return;
    }
    config += '}';

    String object_id = id;
    object_id.replace('/', '_');
    send_retained("homeassistant/" + component + "/" + object_id + "/config", config);
}

void send_discovery() {
    Serial.println("INFO: Sending home assistant discovery.");
    announce(HA_DIAGNOSTIC, node_info.unit_name + "/git_revision",
             "revision_" + node_info.unit_name);

    for (int i = 0; i < num_outputs; i++) {
        if (outputs[i].label)
            announce(outputs[i].ha, outputs[i].name, outputs[i].label);
    }
    for (int i = 0; i < num_timers; i++) {
        if (timers[i].label)
            announce(timers[i].ha, timers[i].name, timers[i].label);
    }
    for (int i = 0; i < num_rollos; i++) {
        if (rollos[i].label)
            announce(HA_COVER, rollos[i].name, rollos[i].label);
    }
    for (int i = 0; i < num_pwms; i++) {
        if (pwms[i].label)
            announce(HA_NUMBER, pwms[i].name, pwms[i].label, pwms[i].max_value);
    }
    // inputs are sensors, not binary sensors, to keep the entity ids from the former mqtt.yaml
    for (int i = 0; i < num_switches; i++) {
        if (switches[i].label and switches[i].is_public)
            announce(HA_SENSOR, switches[i].name, switches[i].label);
    }
    for (int i = 0; i < num_temps; i++) {
        if (!temps[i].label)
            continue;
        announce(HA_TEMPERATURE, "TI_" + temps[i].name, "Temperatur " + String(temps[i].label));
        if (temps[i].sensor == DHT22_T)
            announce(HA_HUMIDITY, "HI_" + temps[i].name, "Feuchte " + String(temps[i].label));
    }
    user_discovery();
}
