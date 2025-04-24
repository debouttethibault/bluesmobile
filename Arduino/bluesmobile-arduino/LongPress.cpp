#include "LongPress.h"

/*
* LongPress:
*   * On button rising edge:
*       * Start timer
*       * If button released (falling) before delay -> short press
*       * If button released after delay -> long press
*/

void LongPress::loop(bool const rising, bool const falling, uint64_t const time_ms) 
{
  this->long_pressed_state = false;
  this->short_pressed_state = false;

  if (rising)
  {
    this->rising_time_ms = time_ms;
    this->pressing = true;
  }

  if (this->pressing)
  {
    if (falling)
    {
      this->short_pressed_state = true;

      this->pressing = false;
      this->rising_time_ms = 0;
    }
    else if ((time_ms - this->rising_time_ms) > LONG_PRESS_DELAY)
    {
      this->long_pressed_state = true;

      this->pressing = false;
      this->rising_time_ms = 0;
    }
  }
}
 
bool LongPress::short_pressed()
{
  return this->short_pressed_state;
}
bool LongPress::long_pressed()
{
  return this->long_pressed_state;
}