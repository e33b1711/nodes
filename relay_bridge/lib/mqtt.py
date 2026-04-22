"""mqtt to openhab interface"""
import logging
import os
import time
import sys
import paho.mqtt.client as mqtt


logger = logging.getLogger(__name__)
logger.setLevel(logging.INFO)

BROKER = "127.0.0.1"
STATE_PREFIX = "power_state/"
ARD_COMMAND_PREFIX = "ard_command/"
ARD_STATE_PREFIX = "ard_state/"
client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
COMMAND_TOPIC_PREFIX = "power_command/"
COMMAND_TOPICS = [COMMAND_TOPIC_PREFIX + "solar2heat",
                  COMMAND_TOPIC_PREFIX + "solar2car",
                  COMMAND_TOPIC_PREFIX + "charger_setpoint",
                  COMMAND_TOPIC_PREFIX + "charging"]
FIRST_RECONNECT_DELAY = 1
RECONNECT_RATE = 20
MAX_RECONNECT_COUNT = 12
MAX_RECONNECT_DELAY = 60


def callback0(client, userdata, message):
    """Message callback for testing."""
    print("message received ", str(message.payload.decode("utf-8")))
    print("message topic=", message.topic)
    print("message qos=", message.qos)
    print("message retain flag=", message.retain)
    payload = str(message.payload.decode("utf-8"))
    print(payload)


def callback1(client, userdata, message):
    """Message callback for testing."""
    print("message received ", str(message.payload.decode("utf-8")))
    print("message topic=", message.topic)
    print("message qos=", message.qos)
    print("message retain flag=", message.retain)
    payload = str(message.payload.decode("utf-8"))
    print(payload)


def callback2(client, userdata, message):
    """Message callback for testing."""
    print("message received ", str(message.payload.decode("utf-8")))
    print("message topic=", message.topic)
    print("message qos=", message.qos)
    print("message retain flag=", message.retain)
    payload = str(message.payload.decode("utf-8"))
    print(payload)


def callback3(client, userdata, message):
    """Message callback for testing."""
    print("message received ", str(message.payload.decode("utf-8")))
    print("message topic=", message.topic)
    print("message qos=", message.qos)
    print("message retain flag=", message.retain)
    payload = str(message.payload.decode("utf-8"))
    print(payload)

def on_disconnect(arg_client, userdata, flags, reason_code, properties):
    """MQTT recoverer."""
    logger.error("Disconnected with result code: %s", reason_code)
    os._exit(1)


def mqtt_init(topics, callbacks, reconnect=True, broker=BROKER):
    """Initialize the client"""
    
    # Configure Paho's native auto-reconnect behavior
    client.reconnect_delay_set(min_delay=FIRST_RECONNECT_DELAY, max_delay=MAX_RECONNECT_DELAY)
    
    try:
        client.connect(broker)
    except Exception as e:
        logger.error(e)
        logger.error("Mqtt server unavailable. Good bye.")
        os._exit(1)

    client.on_message = callback0
    if reconnect:
        client.on_disconnect = on_disconnect
    else:
        client.on_disconnect = on_disconnect_exit
        
    for topic, callback in zip(topics, callbacks):
        client.subscribe(topic)
        client.message_callback_add(topic, callback)
        
    client.loop_start()
    logger.info('mqtt client initialized.')


def mqtt_stop():
    """Initialize the client"""
    client.loop_stop()
    logger.info('mqtt client stopped.')


def mqtt_publish(data):
    """Publish a dict on mqtt"""
    for key, value in data.items():
        client.publish(STATE_PREFIX + key, value)
        logger.debug("publish: %s%s / %s", STATE_PREFIX, key, str(value))
    logger.info('mqtt_publish done.')


def mqtt_publish_ard_command(data):
    """Publish a dict on mqtt"""
    for key, value in data.items():
        client.publish(ARD_COMMAND_PREFIX + key, value)
        logger.debug("publish: %s%s / %s", ARD_COMMAND_PREFIX, key, str(value))
    logger.info('mqtt_publish done.')


def mqtt_publish_ard_state(data):
    """Publish a dict on mqtt"""
    for key, value in data.items():
        client.publish(ARD_STATE_PREFIX + key, value)
        logger.debug("publish: %s%s / %s", ARD_STATE_PREFIX, key, str(value))
    logger.info('mqtt_publish done.')


if __name__ == "__main__":
    logging.basicConfig(level=0)

    all_data = {
        'criticial_loads_power': -88,
        'charger_power': -99}

    print(all_data)
    mqtt_init(COMMAND_TOPICS, [callback0, callback1,
              callback2, callback3], broker=BROKER)
    mqtt_publish(all_data)

    time.sleep(60)
    mqtt_stop()
    print("done.")
