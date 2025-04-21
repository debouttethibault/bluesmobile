#include "BM_Audio.h"

#include "BM_Config.h"

// AudioPlaySdWav        audioPlayer;
// AudioInputI2S         audioInput; // Microphone or Line
// AudioOutputI2S        audioOutput;
// AudioMixer4           audioMixer;

// AudioConnection       audioPatchPlayerToMixer(audioPlayer, 0, audioMixer, AUDIO_MIX_CH_PLAYER);
// AudioConnection       audioPatchInputToMixer(audioInput, 0, audioMixer, AUDIO_MIX_CH_INPUT);
// AudioConnection       audioPatchMixerToOutput(audioMixer, 0, audioOutput, 0);
// AudioControlSGTL5000  audioCodec;

// int audio_calc_output_hpf_coef();

// int audio_calc_output_hpf_coef()
// {
//   int filterCoef;
//   calcBiquad(FILTER_HIPASS, 400.0f, 0, 0.707f, 524288, 44100, &filterCoef);
//   return filterCoef;
// }