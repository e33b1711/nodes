//Serial as used for debug messages of all sorts
//additional the same protocoll as over ethernet is implemented
#include "node.h"

String input_buffer_debug = "";


bool get_message_debug() {
  while (Serial.available()) {
    //get the new byte:
    char inChar = Serial.read();
    //add it to the inputString:
    input_buffer_debug += inChar;
    if (inChar == '$') {
      return true;
    }
  }
  return false;
}

void send_state_debug(String name, int value) {
  String message = "!s!" + name + '!' + String(value, DEC)  + "$\n";
  Serial.print(message);
}

void post_all_debug() {
  send_state_debug(node_info.unit_name, " posting all: start");
  for (int i=0; i<num_outputs; i++){
    send_state_debug(outputs[i].name, outputs[i].value);
  }
  for (int i=0; i<num_rollos; i++){
    send_state_debug(rollos[i].name, rollos[i].value);
  }
  for (int i = 0; i < num_valves; i++) {
    send_state_debug(valves[i].name, valves[i].value);
  }
  send_state_debug(node_info.unit_name, " posting all: end");
}


void parse_message_debug() {
  //message parser
  Serial.print("handle_comm: parsing message: ");
  Serial.println(input_buffer_debug);

  int index1 = input_buffer_debug.indexOf('!');
  int index2 = input_buffer_debug.indexOf('!', index1 + 1);
  int index3 = input_buffer_debug.indexOf('!', index2 + 1);
  int index4 = input_buffer_debug.indexOf('$');

  String type = input_buffer_debug.substring(index1 + 1, index2);
  String name = input_buffer_debug.substring(index2 + 1, index3);
  String value_string = input_buffer_debug.substring(index3 + 1, index4);
  input_buffer_debug = "";

  int value = value_string.toInt();
  if (value_string == "STOP") { value = 50; };
  if (value_string == "UP") { value = 0; };
  if (value_string == "DOWN") { value = 100; };
  if (value_string == "ON") { value = 1; };
  if (value_string == "OFF") { value = 0; };

  //message handler
  if (type == "post_all") {
    if (name == node_info.unit_name) post_all_debug();
  } else if (type == "restart") {
    if (name == node_info.unit_name) delay(100000);
  } else if (type == "r") {
    if (get_any(name, value)) send_state_debug(name, value);
  } else if (type == "c") {
    write_any(name, value);
  }
}

void send_state_debug(String name, String value) {
  String message = "!s!" + name + '!' + value + "$\n";
  Serial.print(message);
}



void handle_debug() {
  if (get_message_debug()) parse_message_debug();
}

void init_debug() {
alloc_pin(0);
alloc_pin(1);
Serial.begin(9600);
post_all_debug();
}