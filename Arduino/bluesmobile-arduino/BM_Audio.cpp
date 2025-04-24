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

  this->patch_player_to_mixer.init(player, 0, mixer, AUDIO_MIX_CH_PLAYER);
  this->patch_player_to_mixer.connect();

  this->patch_input_to_mixer.init(input, 0, mixer, AUDIO_MIX_CH_INPUT);
  this->patch_input_to_mixer.connect();

  this->patch_mixer_to_output.connect(mixer, 0, output, 0);

  this->mixer.gain(AUDIO_MIX_CH_PLAYER, 0.0f);
  this->mixer.gain(AUDIO_MIX_CH_INPUT, 0.0f);
}

bool BM_Audio::microphone(bool enable) {
  if (enable)
  {
    this->mixer.gain(AUDIO_MIX_CH_INPUT, 1.0f);
  }
  else
  {
    this->mixer.gain(AUDIO_MIX_CH_INPUT, 0.0f);
  }
  return true;
}

bool BM_Audio::linein(bool enable) {
  // ToDo: implement linein
  return false;
}

bool BM_Audio::play(String file)
{
  bool ret;

  this->mixer.gain(AUDIO_MIX_CH_PLAYER, 1.0f);

  ret = this->player.play(file.c_str());
  
  return ret;
}

bool BM_Audio::stop() 
{
  this->mixer.gain(AUDIO_MIX_CH_PLAYER, 0.0f);

  if (this->player.isPlaying())
  {
    this->player.stop();
  }
  
  return true;
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