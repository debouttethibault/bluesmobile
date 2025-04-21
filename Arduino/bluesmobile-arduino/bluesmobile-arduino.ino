// #include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <Audio.h>

#include "Debounce.h"
#include "ButtonMatrix.h"
#include "LongPress.h"

#include "BM_Config.h"
#include "BM_Audio.h"

uint8_t const BTN_MTX_NUM_ROWS  = 3;
uint8_t const BTN_MTX_NUM_COLS  = 3;
static uint8_t const BTN_MTX_NUM = BTN_MTX_NUM_ROWS * BTN_MTX_NUM_COLS;
static uint8_t const BTN_MTX_COL_PINS[BTN_MTX_NUM_COLS] = { PIN_BTN_MTX_01, PIN_BTN_MTX_02, PIN_BTN_MTX_03 };
static uint8_t const BTN_MTX_ROW_PINS[BTN_MTX_NUM_ROWS] = { PIN_BTN_MTX_10, PIN_BTN_MTX_20, PIN_BTN_MTX_30 };

uint8_t const BTN_MTX_NUM_PRESETS = 6;
static uint8_t const BTN_MTX_IDX_PRESETS[BTN_MTX_NUM_PRESETS] = { BTN_MTX_P_1, BTN_MTX_P_2, BTN_MTX_P_3, BTN_MTX_P_4, BTN_MTX_P_5, BTN_MTX_P_6 };

ButtonMatrix btn_matrix(BTN_MTX_COL_PINS, BTN_MTX_NUM_COLS, BTN_MTX_ROW_PINS, BTN_MTX_NUM_ROWS); // Main button matrix handler
LongPress btn_lp_mtx_up;              // Long press handler for UP button
LongPress btn_lp_mtx_down;            // Long press handler for DOWN button
Debounce btn_deb_stop;
Debounce btn_deb_mic;

uint8_t const audio_mic_gain = 36;
bool audio_volume_changed;
float audio_volume = 0.5;
AudioInputI2S         audio_input; // Microphone or Line
AudioOutputI2S        audio_output;
AudioConnection       audio_patch_input_output(audio_input, 0, audio_output, 0);
AudioControlSGTL5000  audio_codec;

void btn_init();
void btn_loop();

void setup() 
{
  Serial.begin(115200);

  SPI.setMOSI(PIN_SPI_MOSI);
  SPI.setSCK(PIN_SPI_SCK);
  if (!(SD.begin(PIN_SPI_CS_SD))) {
    Serial.println("SD ERR: Unable to access SD card");
    while (1) {
      delay(1000);
    }
  }

  btn_init();

  AudioMemory(8);
  audio_codec.enable();
  
  audio_codec.inputSelect(AUDIO_INPUT_MIC);
  audio_codec.muteLineout();
  audio_codec.micGain(audio_volume);
  audio_codec.volume(0.5);

  audio_patch_input_output.disconnect();

  // audioPlayer.play("");
}

uint64_t time_ms;

void loop() {
  time_ms = millis();

  btn_loop();

  if (btn_deb_mic.rising())
  {
    audio_patch_input_output.connect();
  }
  else if (btn_deb_mic.falling())
  {
    audio_patch_input_output.disconnect();
  }

  if (btn_lp_mtx_down.pressed())
  {
    audio_volume -= 0.1f;
    audio_volume_changed = true;
  }
  if (btn_lp_mtx_up.pressed())
  {
    audio_volume += 0.1f;
    audio_volume_changed = true;
  }

  if (audio_volume_changed)
  {
    audio_volume_changed = false;
    audio_volume = constrain(audio_volume, 0.0f, 1.0f);
    audio_codec.volume(audio_volume);

    Serial.print("VOL=");
    Serial.println(audio_volume);
  }

  for (uint8_t i = 0; i < BTN_MTX_NUM_PRESETS; i++) 
  {
    if (btn_matrix.rising(BTN_MTX_IDX_PRESETS[i]))
    {
      Serial.print("BTN PRESET ");
      Serial.print(i);
    }
  }

  delay(10);
}

void btn_init()
{
  btn_matrix.init();
  
  pinMode(PIN_BTN_COM_MIC_STOP, OUTPUT);
  digitalWrite(PIN_BTN_COM_MIC_STOP, LOW);

  pinMode(PIN_BTN_STOP, INPUT_PULLUP);
  pinMode(PIN_BTN_MIC, INPUT_PULLUP);
}

void btn_loop() 
{
  btn_deb_stop.loop(!digitalRead(PIN_BTN_STOP), time_ms); // Debounce stop button
  btn_deb_mic.loop(!digitalRead(PIN_BTN_MIC), time_ms);   // Debounce mic button

  btn_matrix.loop(time_ms); // Handle button matrix (and debounce)

  btn_lp_mtx_up.loop(btn_matrix.rising(BTN_MTX_IDX_UP), btn_matrix.falling(BTN_MTX_IDX_UP), time_ms); // Handle long press UP
  btn_lp_mtx_down.loop(btn_matrix.rising(BTN_MTX_IDX_DOWN), btn_matrix.falling(BTN_MTX_IDX_DOWN), time_ms); // Handle long press DOWN
}

