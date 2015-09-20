/*
 *
 * This example reads from the default PCM device
 * and writes to standard output for 5 seconds of data.
 *
 * */

/* Use the newer ALSA API */
#define ALSA_PCM_NEW_HW_PARAMS_API

#include <alsa/asoundlib.h>
#include "asound.h"
#include "Broadcast.h"

int main() {
	int rc;
	int res;
	snd_pcm_t *handle;

	bcast_setup_tx_socket();

	/* Open PCM device for recording (capture). */
	rc = snd_pcm_open(&handle, "looprec", SND_PCM_STREAM_CAPTURE, 0);
	if (rc < 0) {
		fprintf(stderr,
				"unable to open pcm device: %s\n",
				snd_strerror(rc));
		exit(1);
	}

	int bytes_per_frame = BITRATE * 1024 * NUM_FRAMES / SAMPLE_RATE / 8;

	set_alsa_hw(handle, SAMPLE_RATE, NUM_CHANNELS, 16 * 1000);
	set_alsa_sw(handle);

	void *packet = malloc(bytes_per_frame); /* 2 bytes/sample, 2 channels */

	while (1)
	{
		bzero(packet, bytes_per_frame);
		/* rc = num frames */
		rc = snd_pcm_readi(handle, packet, NUM_FRAMES);
                printf("Buff:%s\n", (char *) packet);
		if (rc == -EPIPE) {
                        /* EPIPE means overrun */
                        fprintf(stderr, "overrun occurred\n");
                        snd_pcm_prepare(handle);
                } else if (rc < 0) {
                        fprintf(stderr,
                                        "error from read: %s\n",
                                        snd_strerror(rc));
			snd_pcm_recover(handle, rc, 0);
                } else if (rc != NUM_FRAMES) {
                        fprintf(stderr, "short read, read %d frames\n", rc);
                }
		res = bcast_tx(packet, 0, bytes_per_frame);
                if (res != bytes_per_frame)
                        fprintf(stderr,
                                        "short write: wrote %d bytes\n", rc);

                if (res < 0) {
                        perror("sendto");
                        exit(1);
                }
                printf("Packet Sent\n");
        }

	snd_pcm_drain(handle);
	snd_pcm_close(handle);
	free(packet);

	return 0;
}
