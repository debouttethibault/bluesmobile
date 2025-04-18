#include <Audio.h>
#include <Bounce.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#define PIN_SPI_CS_SD         10

#define PIN_BTN_MTX_01        24
#define PIN_BTN_MTX_02        25
#define PIN_BTN_MTX_03        26
#define PIN_BTN_MTX_10        27
#define PIN_BTN_MTX_20        28
#define PIN_BTN_MTX_30        29
#define PIN_BTN_MIC           30
#define PIN_BTN_STOP          31

#define AUDIO_MIX_CH_PLAYER   0
#define AUDIO_MIX_CH_INPUT    1

uint8_t const BTN_MTX_NUM_ROWS  = 3;
uint8_t const BTN_MTX_NUM_COLS  = 3;
static uint8_t const BTN_MTX_NUM = BTN_MTX_NUM_ROWS * BTN_MTX_NUM_COLS;
static uint8_t const BTN_MTX_COL_PINS[BTN_MTX_NUM_COLS] = { PIN_BTN_MTX_01, PIN_BTN_MTX_02, PIN_BTN_MTX_03 };
static uint8_t const BTN_MTX_ROW_PINS[BTN_MTX_NUM_ROWS] = { PIN_BTN_MTX_10, PIN_BTN_MTX_20, PIN_BTN_MTX_30 };

AudioPlaySdWav        audioPlayer;
AudioInputI2S         audioInput; // Microphone or Line
AudioOutputI2S        audioOutput;
AudioMixer4           audioMixer;

AudioConnection       audioPatchPlayerToMixer(audioPlayer, 0, audioMixer, AUDIO_MIX_CH_PLAYER);
AudioConnection       audioPatchInputToMixer(audioInput, 0, audioMixer, AUDIO_MIX_CH_INPUT);
AudioConnection       audioPatchMixerToOutput(audioMixer, 0, audioOutput, 0);
AudioControlSGTL5000  audioCodec;

void btn_init();
void btn_pressed(uint8_t id);
void btn_released(uint8_t id);
void btn_loop();

int audio_calc_output_hpf_coef();

void setup() 
{
  Serial.begin(115200);

  btn_init();

  AudioMemory(8);
  audioCodec.enable();
  audioCodec.inputSelect(AUDIO_INPUT_LINEIN);
  audioCodec.muteLineout();

  int filterCoef = audio_calc_output_hpf_coef();
  audioCodec.eqFilter(0, &filterCoef);

  audioCodec.volume(0.5);

  audioMixer.gain(1, 1);
  audioMixer.gain(0, 1);

  audioPatchInputToMixer.disconnect();

  SPI.setMOSI(PIN_SPI_MOSI);
  SPI.setSCK(PIN_SPI_SCK);
  if (!(SD.begin(PIN_SPI_CS_SD))) {
    Serial.println("SD ERR: Unable to access SD card");
    while (1) {
      delay(1000);
    }
  }

  audioPlayer.play("");
}

uint64_t time_ms;

void loop() {
  time_ms = millis();

  btn_loop();

  delay(1);
}

void btn_init() 
{
  uint8_t pin;
  uint8_t i;
  for (i = 0; i < BTN_MTX_NUM_COLS; i++)
  {
    pin = BTN_MTX_COL_PINS[i];
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
  }
  for (i = 0; i < BTN_MTX_NUM_ROWS; i++) 
  {
    pin = BTN_MTX_ROW_PINS[i];
    pinMode(pin, INPUT_PULLUP);
  }
}

void btn_loop()
{
  uint8_t i;
  uint8_t j;
  for (i = 0; i < BTN_MTX_NUM_COLS; i++)
  {
    digitalWrite(BTN_MTX_COL_PINS[i], LOW);
    
    for (j = 0; j < BTN_MTX_NUM_ROWS; j++) 
    {
      bool active = !digitalRead(BTN_MTX_ROW_PINS[j]); // ToDo: debounce buttons
    }

    digitalWrite(BTN_MTX_COL_PINS[i], HIGH);
  }
}

int audio_calc_output_hpf_coef()
 {
  int filterCoef;
  calcBiquad(FILTER_HIPASS, 400.0f, 0, 0.707f, 524288, 44100, &filterCoef);
  return filterCoef;
}