#include <Audio.h>
#include <Bounce.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#define SDCARD_CS_PIN    10
#define SDCARD_MOSI_PIN  11
#define SDCARD_SCK_PIN   13

#define AUDIO_MIX_CH_PLAYER 0
#define AUDIO_MIX_CH_MIC    1

AudioPlaySdWav        audioPlayer;
AudioInputI2S         audioInput; // Microphone or Line
AudioOutputI2S        audioOutput;
AudioMixer4           audioMixer;

AudioConnection       audioPatchPlayerToMixer(audioPlayer, 0, audioMixer, AUDIO_MIX_CH_PLAYER);
AudioConnection       audioPatchInputToMixer(audioInput, 0, audioMixer, AUDIO_MIX_CH_MIC);
AudioConnection       audioPatchMixerToOutput(audioMixer, 0, audioOutput, 0);
AudioControlSGTL5000  audioCodec;

void setup() {
  Serial.begin(9600);

  AudioMemory(8);
  audioCodec.enable();
  audioCodec.inputSelect(AUDIO_INPUT_MIC);
  audioCodec.volume(0.5);

  audioMixer.gain(1, 1);
  audioMixer.gain(0, 1);

  // audioPatchInputToMixer.disconnect();

  SPI.setMOSI(SDCARD_MOSI_PIN);
  SPI.setSCK(SDCARD_SCK_PIN);
  if (!(SD.begin(SDCARD_CS_PIN))) {
    Serial.println("SD ERR: Unable to access SD card");
    while (1) {
      delay(1000);
    }
  }

}

void loop() {
  delay(1000);
}

