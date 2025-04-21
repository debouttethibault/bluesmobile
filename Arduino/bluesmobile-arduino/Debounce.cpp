#include "Debounce.h"

void Debounce::loop(bool const reading, uint64_t const time_ms) 
{
  this->state_changed = false;

  if (reading != this->last_reading) // Input changed, start debouncing
  {
    this->last_time_ms = time_ms;
  }

  if ((time_ms - this->last_time_ms) >= DEBOUNCE_DELAY_MS) 
  {
    if (reading != this->stable_state)
    {
      this->stable_state = reading;
      this->state_changed = true;
    }
  }

  this->last_reading = reading;
}

bool Debounce::pressed()
{
  return this->stable_state;
}

bool Debounce::rising()
{
  return (this->state_changed && this->stable_state);
}

bool Debounce::falling()
{
  return (this->state_changed && !this->stable_state);
}