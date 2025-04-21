#pragma once

#include <Arduino.h>

#define LONG_PRESS_DELAY 600

class LongPress
{
public:
  bool pressed();
  bool long_pressed();

  void loop(bool const rising, bool const falling, uint64_t const time_ms);

private:
  bool pressing;
  bool pressed_state;
  bool long_pressed_state;
  
  uint64_t rising_time_ms;
};