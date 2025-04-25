#include "BM_Audio.h"

#include "BM_Config.h"

BM_Audio::BM_Audio() { }
BM_Audio::~BM_Audio() { }

void BM_Audio::init()
{
  AudioMemory(8);
  this->codec.enable();
  
  this->codec.inputSelect(AUDIO_INPUT_MIC);
  this->codec.muteLineout();
  this->codec.micGain(this->mic_gain);
  this->codec.volume(this->init_volume);

  this->patch_input_to_output.init(input, 0, output, 0);

  this->patch_player_to_output.init(player, 0, output, 0);
}

static inline bool mic_enabled_cond(uint8_t active_flag, bool enable)
{
  return (0 == (active_flag & AUDIO_FLAG_MICROPHONE) && true == enable);
}

static inline bool mic_disabled_cond(uint8_t active_flag, bool enable)
{
  return (AUDIO_FLAG_MICROPHONE == (active_flag & AUDIO_FLAG_MICROPHONE) && false == enable);
}

bool BM_Audio::microphone(bool enable) {
  if (mic_enabled_cond(this->active_flag, enable))
  {
    this->patch_input_to_output.connect();

    this->active_flag |= AUDIO_FLAG_MICROPHONE;
  }
  else if (mic_disabled_cond(this->active_flag, enable))
  {
    this->patch_input_to_output.disconnect();

    this->active_flag &= ~AUDIO_FLAG_MICROPHONE;
  }
  return true;
}

bool BM_Audio::linein(bool enable) {
  // ToDo: implement linein
  return false;
}

bool BM_Audio::play(String file)
{
  // ToDo: implement player
  return false;
}

bool BM_Audio::stop() 
{
  return false;
}

bool BM_Audio::volume(float vol) {
  bool ret;

  vol = constrain(vol, 0.0f, 1.0f);
  
  ret = this->codec.volume(vol);

  return ret;
}

//region MyAudioConnection
int MyAudioConnection::init(AudioStream &source, unsigned char sourceOutput,
		AudioStream &destination, unsigned char destinationInput)
{
  int result = 1;

  if (!this->isConnected)
  {
    this->src = &source;
    this->src_index = sourceOutput;
    this->dst = &destination;
    this->dest_index = destinationInput;

    result = 0;
  }

  return result;
}

bool MyAudioConnection::connected()
{
  return this->isConnected;
}
//endregion