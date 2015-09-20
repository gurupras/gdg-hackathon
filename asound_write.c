/*

   This example reads standard from input and writes
   to the default PCM device for 5 seconds of data.

*/

/* Use the newer ALSA API */
#define ALSA_PCM_NEW_HW_PARAMS_API

#include <alsa/asoundlib.h>
#include "asound.h"
#include "Broadcast.h"

int main() {
	int rc;
	snd_pcm_t *handle;

	bcast_setup_rx_socket();

	/* Open PCM device for playback. */
	rc = snd_pcm_open(&handle, "default",
			SND_PCM_STREAM_PLAYBACK, 0);
	if (rc < 0) {
		fprintf(stderr,
				"unable to open pcm device: %s\n",
				snd_strerror(rc));
		exit(1);
	}

	set_alsa_hw(handle, SAMPLE_RATE, NUM_CHANNELS, BUFFER_TIME * 1000);
	set_alsa_sw(handle);

	char buffer[32768];
	while (1) {
		bzero(buffer, sizeof(buffer));

		if ((rc = bcast_rx(buffer, sizeof(buffer)) < 0)) {
			perror("recvfrom");
			exit(1);
		}
//		printf("Buff:%s\n\n",buffer);
		if (rc == 0) {
			//fprintf(stderr, "end of file on input\n");
//			break;
		} else if (rc != sizeof(buffer)) {
			//fprintf(stderr,
			//		"short read: read %d bytes\n", rc);
		}
		rc = snd_pcm_writei(handle, buffer, NUM_FRAMES);
		if (rc == -EPIPE) {
			/* EPIPE means underrun */
			//fprintf(stderr, "underrun occurred\n");
			snd_pcm_prepare(handle);
		} else if (rc < 0) {
			//fprintf(stderr,
			//		"error from writei: %s\n",
			//		snd_strerror(rc));
		}  else if (rc != NUM_FRAMES) {
			//fprintf(stderr,
			//		"short write, write %d frames\n", rc);
		}
	}

	snd_pcm_drain(handle);
	snd_pcm_close(handle);
	free(buffer);

	return 0;
}
