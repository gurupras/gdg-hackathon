#define FRAME_SIZE 4
#define SAMPLE_RATE 44100
#define NUM_FRAMES 960
#define BITRATE 128
#define NUM_CHANNELS 2
#define BUFFER_TIME 16

#include <alsa/asoundlib.h>

int set_alsa_hw(snd_pcm_t *pcm,
                unsigned int rate, unsigned int channels,
                unsigned int buffer);
int set_alsa_sw(snd_pcm_t *pcm);

