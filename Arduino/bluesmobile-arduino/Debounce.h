#pragma once

#include <Arduino.h>

#define DEBOUNCE_DELAY_MS 50

class Debounce
{
public:
  bool pressed();
  bool rising();
  bool falling();

  void loop(bool const reading, uint64_t const time_ms);

private:
  bool debouncing;
  bool last_reading;
  bool stable_state;
  bool state_changed;
  uint64_t last_time_ms;
};