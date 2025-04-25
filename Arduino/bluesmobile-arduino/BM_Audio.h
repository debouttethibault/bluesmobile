#pragma once

#include <Arduino.h>
#include <Audio.h>

#define AUDIO_MIX_CH_PLAYER   0
#define AUDIO_MIX_CH_INPUT    1

#define AUDIO_FLAG_MICROPHONE  (1 << 0)
#define AUDIO_FLAG_LINEIN      (1 << 1)
#define AUDIO_FLAG_PLAYER      (1 << 2)

class MyAudioConnection : public AudioConnection
{
public:
  using AudioConnection::AudioConnection;
  int init(AudioStream &source, unsigned char sourceOutput,
		AudioStream &destination, unsigned char destinationInput);
  bool connected();
};

class BM_Audio
{
public:
  BM_Audio();
  ~BM_Audio();

  void init();

  bool microphone(bool enable);
  bool linein(bool enable);
  bool play(String file);
  bool stop(); // stop all sources (except microphone)

  bool volume(float vol);

private:
  float const           init_volume = 0.5f;
  uint8_t const         mic_gain  = 8;

  uint8_t               prev_active_flag;
  uint8_t               active_flag;

  AudioOutputI2S        output;
  AudioPlaySdWav        player;
  AudioInputI2S         input;

  MyAudioConnection     patch_player_to_output;
  MyAudioConnection     patch_input_to_output;
  AudioControlSGTL5000  codec;
};