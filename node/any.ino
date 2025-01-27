
bool write_any(String address, int value) {
  Serial.print("write any: " + address + " ");
  Serial.println(value);
  if (write_valve(address, value)) return true;
  if (write_output(address, value)) return true;
  if (write_rollo(address, value)) return true;
  Serial.print("write any: ERROR found none!");
  return false;
}

bool get_any(String name, int &value) {
  if (get_temp(name, value)) return true;
  if (get_humi(name, value)) return true;
  if (get_valve(name, value)) return true;
  if (get_rollo(name, value)) return true;
  if (get_output(name, value)) return true;
  return false;
}

