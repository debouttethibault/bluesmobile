// #include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

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

uint8_t const TRACK_NUM = 6;
uint8_t const TRACK_NUM_PER_PAGE = 6;
uint8_t const TRACK_NUM_PAGES = 1;
static String const TRACK_FILE_NAMES[TRACK_NUM] = {
  "rodania.wav",  "la_cucaracha.wav",  "general_lee.wav",  "police.wav",  "rainbow.wav",  "vis_in_de_leie.wav", // Page 1
};

uint64_t time_ms;

bool track_page_changed = false;
uint8_t track_page_idx = 0;
int16_t track_preset_requested_idx = TRACK_IDX_NA;

bool audio_volume_changed = false;
float audio_volume = 0.5f;

bool stop_requested;

BM_Audio audio;

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

  audio.init();
}

void loop() {
  time_ms = millis();

  stop_requested = false;
  track_preset_requested_idx = TRACK_IDX_NA;
  track_page_changed = false;
  audio_volume_changed = false;

  btn_loop();

//region Handle microphone
  if (btn_deb_mic.rising())
  {
    audio.microphone(true);

    Serial.println("MIC ON");
  }
  else if (btn_deb_mic.falling())
  {
    audio.microphone(false);

    Serial.println("MIC OFF");
  }
//endregion

//region Handle volume
  if (btn_lp_mtx_down.short_pressed())
  {
    audio_volume -= 0.1f;
    audio_volume_changed = true;
  }
  if (btn_lp_mtx_up.short_pressed())
  {
    audio_volume += 0.1f;
    audio_volume_changed = true;
  }

  if (audio_volume_changed)
  {
    audio_volume = constrain(audio_volume, 0.0f, 1.0f);
    // audio_codec.volume(audio_volume);

    Serial.print("VOL=");
    Serial.println(audio_volume);
  }
//endregion

//region Handle preset pages
  if (btn_lp_mtx_down.long_pressed())
  {
    track_page_idx--;
    track_page_changed = true;
  }
  if (btn_lp_mtx_up.long_pressed())
  {
    track_page_idx++;
    track_page_changed = true;
  }
  if (track_page_changed) 
  {
    track_page_idx = constrain(track_page_idx, 0, TRACK_NUM_PAGES - 1);

    Serial.print("PAGE IDX=");
    Serial.println(track_page_idx);
  }
//endregion

//region Handle player
  if (btn_deb_stop.rising())
  {
    audio.stop();

    Serial.print("PLAYER STOP");
  }
  else 
  {
    for (uint8_t i = 0; i < BTN_MTX_NUM_PRESETS; i++) 
    {
      if (btn_matrix.rising(BTN_MTX_IDX_PRESETS[i]))
      {
        Serial.print("PRESET IDX=");
        Serial.println(i);

        track_preset_requested_idx = i;
      }
    }

    if (track_preset_requested_idx > TRACK_IDX_NA)
    {
      uint8_t track_idx = ((TRACK_NUM_PER_PAGE * track_page_idx) + track_preset_requested_idx);
      if (track_idx < 0 || track_idx >= TRACK_NUM)
      {
        Serial.println("PLAYER TRACK INVALID");
      }
      else
      {
        Serial.print("PLAYER TRACK=");
        Serial.println(track_idx);

        audio.play(TRACK_FILE_NAMES[track_idx]);
      }
    }
  }
//endregion Handle player

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

