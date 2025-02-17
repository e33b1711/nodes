#include "node.h"
//helper functions for coupling switches to ouputs / rollos

//switch with long / short press
void long_short(const int b_index, const String l_long, const int l_value, const String l_short, const int s_value) {
  if (switches[b_index].edge == -1) {
    if (switches[b_index].last_rising_edge + 700 > switches[b_index].last_falling_edge) {
      write_any(l_long, l_value);
    } else {
      write_any(l_short, s_value);
    }
  }
}

//simple switch to output state
void simple(const int b_index, const int value, const String l) {
  if (switches[b_index].edge == 1) {
    write_any(l, value);
  }
}

//roolo switch click for complete up/down / hold and release for partial up/down
void rollo_lock(const int b_index, const int value, const String l) {
  if (switches[b_index].edge == 1) {
    write_any(l, value);
  }
  if (switches[b_index].edge == -1) {
    if (switches[b_index].last_rising_edge + 1000 < switches[b_index].last_falling_edge) {
      write_any(l, 50);
    }
  }
}
