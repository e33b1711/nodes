//handles ethernet
#include <SPI.h>
#include <Ethernet.h>

EthernetClient client;
long last_try_connect = 0;
const int retry_period = 60000;
String message_buffer = "";
String input_buffer = "";

void init_hw() {
  alloc_pin(node_info.ethernet_sc_pin);
  alloc_pin(node_info.ethernet_reset_pin);
  Ethernet.init(node_info.ethernet_sc_pin);
  Ethernet.begin(node_info.mac, node_info.ip);
  delay(500);
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found. Stop!");
    while (true) delay(1);
  }
}

bool connect_server() {
  while (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("setup_comm: Ethernet cable is not connected.");
    delay(500);
    return false;
  }
  if (!client.connect(node_info.server, node_info.port)) {
    Serial.println("setup_comm: connection failed");
    return false;
  }
  Serial.println("setup_comm: connected");
  return true;
}

void setup_comm() {
  Serial.println("setup_comm...");
  init_hw();
  connect_server();
  message_buffer.reserve(100);
  last_try_connect = millis();
}

void send_command(String name, int value) {
  String message = "!c!" + name + '!' + String(value, DEC) + "$\n";
  message_buffer += message;
}

void send_state(String name, int value) {
  String message = "!s!" + name + '!' + String(value, DEC) + "$\n";
  message_buffer += message;
}

void send_state(String name, String value) {
  String message = "!s!" + name + '!' + value + "$\n";
  message_buffer += message;
}

void parse_message() {
  //message parser
  Serial.print("handle_comm: parsing message: ");
  Serial.println(input_buffer);

  int index1 = input_buffer.indexOf('!');
  int index2 = input_buffer.indexOf('!', index1 + 1);
  int index3 = input_buffer.indexOf('!', index2 + 1);
  int index4 = input_buffer.indexOf('$');

  String type = input_buffer.substring(index1 + 1, index2);
  String name = input_buffer.substring(index2 + 1, index3);
  String value_string = input_buffer.substring(index3 + 1, index4);
  input_buffer = "";

  int value = value_string.toInt();
  if (value_string == "STOP") { value = 50; };
  if (value_string == "UP") { value = 0; };
  if (value_string == "DOWN") { value = 100; };
  if (value_string == "ON") { value = 1; };
  if (value_string == "OFF") { value = 0; };

  //message handler
  if (type == "post_all") {
    if (name == node_info.unit_name) post_all();
  } else if (type == "restart") {
    if (name == node_info.unit_name) delay(100000);
  } else if (type == "r") {
    if (get_any(name, value)) send_state(name, value);
  } else if (type == "c") {
    write_any(name, value);
  }
}

void send_messages() {
  if (client.connected() && message_buffer != "") {
    client.print(message_buffer);
    Serial.println("handle_comm: Sending message_buffer...");
    Serial.print(message_buffer);
    message_buffer = "";
  }
}

bool get_message() {
  while (client.available()) {
    //get the new byte:
    char inChar = client.read();
    //add it to the inputString:
    input_buffer += inChar;
    if (inChar == '$') {
      return true;
    }
  }
  return false;
}

void handle_comm() {
  if (maintain_connection()) {
    if (get_message()) parse_message();
    send_messages();
  }
}

bool maintain_connection() {
  if (client.connected()) {
    return true;
  } else {
    input_buffer = "";
    message_buffer = "";
    if (last_try_connect + retry_period < millis()) {
      last_try_connect = millis();
      Serial.println("handel_comm: trying to recover...");
      return connect_server();
    }
    return false;
  }
}

void post_all() {
  send_state(node_info.unit_name + " posting all: start", 0);
  for (int i = 0; i < num_outputs; i++) {
    send_state(outputs[i].name, outputs[i].value);
  }
  for (int i = 0; i < num_rollos; i++) {
    send_state(rollos[i].name, rollos[i].value);
  }
  for (int i = 0; i < num_valves; i++) {
    send_state(valves[i].name, valves[i].value);
  }
  send_state(node_info.unit_name + " posting all: end", 0);
}
