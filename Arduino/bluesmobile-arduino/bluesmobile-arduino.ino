// #include <Audio.h>
// #include <Wire.h>
// #include <SPI.h>
// #include <SD.h>
// #include <SerialFlash.h>

#include "Debounce.h"
#include "ButtonMatrix.h"
#include "LongPress.h"

// #define PIN_SPI_CS_SD         10

#define PIN_BTN_MTX_01        24
#define PIN_BTN_MTX_02        26
#define PIN_BTN_MTX_03        28
#define PIN_BTN_MTX_10        25
#define PIN_BTN_MTX_20        27
#define PIN_BTN_MTX_30        29
#define PIN_BTN_COM_MIC_STOP  31
#define PIN_BTN_MIC           30
#define PIN_BTN_STOP          32

#define BTN_MTX_IDX_AUX       0
#define BTN_MTX_IDX_UP        8
#define BTN_MTX_IDX_DOWN      5

// #define AUDIO_MIX_CH_PLAYER   0
// #define AUDIO_MIX_CH_INPUT    1

uint8_t const BTN_MTX_NUM_ROWS  = 3;
uint8_t const BTN_MTX_NUM_COLS  = 3;
static uint8_t const BTN_MTX_NUM = BTN_MTX_NUM_ROWS * BTN_MTX_NUM_COLS;
static uint8_t const BTN_MTX_COL_PINS[BTN_MTX_NUM_COLS] = { PIN_BTN_MTX_01, PIN_BTN_MTX_02, PIN_BTN_MTX_03 };
static uint8_t const BTN_MTX_ROW_PINS[BTN_MTX_NUM_ROWS] = { PIN_BTN_MTX_10, PIN_BTN_MTX_20, PIN_BTN_MTX_30 };
ButtonMatrix button_matrix(BTN_MTX_COL_PINS, BTN_MTX_NUM_COLS, BTN_MTX_ROW_PINS, BTN_MTX_NUM_ROWS);

LongPress button_lp_mtx_up;
LongPress button_lp_mtx_down;

// AudioPlaySdWav        audioPlayer;
// AudioInputI2S         audioInput; // Microphone or Line
// AudioOutputI2S        audioOutput;
// AudioMixer4           audioMixer;

// AudioConnection       audioPatchPlayerToMixer(audioPlayer, 0, audioMixer, AUDIO_MIX_CH_PLAYER);
// AudioConnection       audioPatchInputToMixer(audioInput, 0, audioMixer, AUDIO_MIX_CH_INPUT);
// AudioConnection       audioPatchMixerToOutput(audioMixer, 0, audioOutput, 0);
// AudioControlSGTL5000  audioCodec;

// int audio_calc_output_hpf_coef();

void setup() 
{
  Serial.begin(115200);

  button_matrix.init();

  pinMode(PIN_BTN_COM_MIC_STOP, OUTPUT);
  digitalWrite(PIN_BTN_COM_MIC_STOP, LOW);
  pinMode(PIN_BTN_STOP, INPUT_PULLUP);
  pinMode(PIN_BTN_MIC, INPUT_PULLUP);

  // AudioMemory(8);
  // audioCodec.enable();
  // audioCodec.inputSelect(AUDIO_INPUT_LINEIN);
  // audioCodec.muteLineout();

  // int filterCoef = audio_calc_output_hpf_coef();
  // audioCodec.eqFilter(0, &filterCoef);

  // audioCodec.volume(0.5);

  // audioMixer.gain(1, 1);
  // audioMixer.gain(0, 1);

  // audioPatchInputToMixer.disconnect();

  // SPI.setMOSI(PIN_SPI_MOSI);
  // SPI.setSCK(PIN_SPI_SCK);
  // if (!(SD.begin(PIN_SPI_CS_SD))) {
  //   Serial.println("SD ERR: Unable to access SD card");
  //   while (1) {
  //     delay(1000);
  //   }
  // }

  // audioPlayer.play("");
}

uint64_t time_ms;

Debounce btn_stop_deb;

void loop() {
  time_ms = millis();

  Serial.print(time_ms);
  Serial.print('\t');

  button_matrix.loop(time_ms);

  button_lp_mtx_up.loop(button_matrix.rising(BTN_MTX_IDX_UP), button_matrix.falling(BTN_MTX_IDX_UP), time_ms);
  button_lp_mtx_down.loop(button_matrix.rising(BTN_MTX_IDX_DOWN), button_matrix.falling(BTN_MTX_IDX_DOWN), time_ms);

  Serial.print("BTN UP: ");
  if (button_lp_mtx_up.pressed())
  {
    Serial.print("SHORT");
  }
  if (button_lp_mtx_up.long_pressed())
  {
    Serial.print("LONG");
  }
  Serial.print("; ");

  Serial.print("BTN DOWN: ");
  if (button_lp_mtx_down.pressed())
  {
    Serial.print("SHORT");
  }
  if (button_lp_mtx_down.long_pressed())
  {
    Serial.print("LONG");
  }
  Serial.print("; ");

  Serial.println();

  delay(10);
}

// int audio_calc_output_hpf_coef()
// {
//   int filterCoef;
//   calcBiquad(FILTER_HIPASS, 400.0f, 0, 0.707f, 524288, 44100, &filterCoef);
//   return filterCoef;
// }