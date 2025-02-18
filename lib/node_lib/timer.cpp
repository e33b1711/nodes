#include "node.h"
#include "timer.h"

void setup_timers()
{
  Serial.println("setup timers");
  int i;
  for (i = 0; i < num_timers; i++)
  {
    timers[i].value = false;
    timers[i].running = false;
    send_state(timers[i].name, (int)timers[i].value);
  }
}

void update_timer(int i)
{
  // start
  if (timers[i].value and !timers[i].running)
  {
    timers[i].running = true;
    timers[i].set_time = millis();
    write_any(timers[i].slave, 1, false);
    Serial.print(timers[i].name + ": timer an");
  }
  // timer: running
  if (timers[i].value and timers[i].running)
  {
    if ((timers[i].set_time + timers[i].duration) < millis())
    {
      timers[i].running = false;
      timers[i].value = false;
      write_any(timers[i].slave, 0, false);
      Serial.print(timers[i].name + ": timer abgelaufen");
    }
  }
  // timer: external off
  // timer: running
  if (!timers[i].value and timers[i].running)
  {
    timers[i].running = false;
    write_any(timers[i].slave, 0, false);
    Serial.print(timers[i].name + ": timer abgebrochen.");
  }
}

void handle_timers()
{
  for (int i = 0; i < num_timers; i++)
    update_timer(i);
}

bool write_timer(String name, int value, bool silent)
{
  int i;
  for (i = 0; i < num_timers; i++)
  {
    if (timers[i].name == name)
    {
      timers[i].set_time = millis();
      switch (value)
      {
      case 0:
        timers[i].value = false;
        break;
      case 1:
        timers[i].value = true;
        break;
      default:
        timers[i].value = not(timers[i].value);
        break;
      }
      if (!silent)
        send_state(name, timers[i].value);
      return true;
    }
  }
  return false;
}

bool get_timer(String name, int &value)
{
  for (int i = 0; i < num_timers; i++)
  {
    if (timers[i].name == name)
    {
      value = (int)timers[i].running;
      return true;
    }
  }
  return false;
}
