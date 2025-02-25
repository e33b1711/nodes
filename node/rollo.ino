void update_rollos() {
  for (int i = 0; i < num_rollos; i++) {
    if (rollos[i].value != rollos[i].old_value) {
      rollos[i].old_value = rollos[i].value;
      switch (rollos[i].value) {
        case 0:
          write_output(rollos[i].l_up, 1);
          write_output(rollos[i].l_down, 0);
          rollos[i].stop_time = millis() + rollos[i].duration;
          rollos[i].stop_pending = true;
          break;
        case 100:
          write_output(rollos[i].l_up, rollos[i].is_rollo);
          write_output(rollos[i].l_down, 1);
          rollos[i].stop_time = millis() + rollos[i].duration;
          rollos[i].stop_pending = true;
          break;
        default:
          write_output(rollos[i].l_up, 0);
          write_output(rollos[i].l_down, 0);
          rollos[i].stop_time = rollos[i].stop_time;
          rollos[i].stop_pending = false;
      }
    }
    if (rollos[i].stop_pending and rollos[i].stop_time < millis()) {
      rollos[i].stop_pending = false;
      write_output(rollos[i].l_up, 0);
      write_output(rollos[i].l_down, 0);
    }
  }
}

bool write_rollo(String name, int value) {
  for (int i = 0; i < num_rollos; i++) {
    if (rollos[i].name == name) {
      if (rollos[i].is_rollo) rollos[i].value = 50;
      if (value == 100) {
        rollos[i].value = 100;
      }
      if (value == 0) {
        rollos[i].value = 0;
      }
      send_state(name, rollos[i].value);
      return true;
    }
  }
  return false;
}

bool get_rollo(String name, int &value) {
  for (int i = 0; i < num_temps; i++) {
    if (rollos[i].name == name) {
      value = rollos[i].value;
      return true;
    }
  }
  return false;
}