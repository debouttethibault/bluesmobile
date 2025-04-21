#pragma once

#include <Arduino.h>

#include "Debounce.h"

// #define BTN_MTX_DEBUG 1

class ButtonMatrix
{
public:
  ButtonMatrix(uint8_t const * pins_cols, uint8_t const num_cols, uint8_t const * pins_rows, uint8_t const num_rows);
  ~ButtonMatrix();

  bool pressed(uint8_t const idx);
  bool rising(uint8_t const idx);
  bool falling(uint8_t const idx);

  void init();
  void loop(uint64_t const time_ms);
private:
  Debounce * debouncers;

  uint8_t const * pins_cols;
  uint8_t const * pins_rows;
  uint8_t num_buttons;
  uint8_t num_rows;
  uint8_t num_cols;
};