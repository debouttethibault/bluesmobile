#include "ButtonMatrix.h"

ButtonMatrix::ButtonMatrix(uint8_t const * pins_cols, uint8_t const num_cols, uint8_t const * pins_rows, uint8_t const num_rows)
{
  this->num_buttons = num_cols * num_rows;
  this->pins_cols = pins_cols;
  this->num_cols = num_cols;
  this->pins_rows = pins_rows;
  this->num_rows = num_rows;

  this->debouncers = new Debounce[this->num_buttons];
}

ButtonMatrix::~ButtonMatrix()
{
  delete[] this->debouncers;
}

void ButtonMatrix::init() 
{
  uint8_t i;
  for (i = 0; i < num_cols; i++)
  {
    pinMode(pins_cols[i], OUTPUT);
    digitalWrite(pins_cols[i], HIGH);
  }
  for (i = 0; i < num_rows; i++) 
  {
    pinMode(pins_rows[i], INPUT_PULLUP);
  }
}

void ButtonMatrix::loop(uint64_t const time_ms)
{
  uint8_t i;
  uint8_t j;
  uint8_t k;
  
  bool reading;
  k = 0;

  Debounce * debouncer;

  for (i = 0; i < this->num_cols; i++)
  {
    digitalWrite(this->pins_cols[i], LOW);
    
    for (j = 0; j < this->num_rows; j++) 
    {
      debouncer = &this->debouncers[k];
      reading = !digitalRead(this->pins_rows[j]);
#ifdef BTN_MTX_DEBUG
      Serial.print(i);
      Serial.print(j);
      Serial.print('=');
      Serial.print(reading);
      Serial.print("; ");
#endif
      debouncer->loop(reading, time_ms);
      k++;
    }

    digitalWrite(this->pins_cols[i], HIGH);
  }
}

bool ButtonMatrix::pressed(uint8_t const idx)
{
  return this->debouncers[idx].pressed();
}
bool ButtonMatrix::rising(uint8_t const idx)
{
  return this->debouncers[idx].rising();
}
bool ButtonMatrix::falling(uint8_t const idx) 
{
  return this->debouncers[idx].falling();
}