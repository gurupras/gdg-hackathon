/*
 *
 * This example reads from the default PCM device
 * and writes to standard output for 5 seconds of data.
 *
 * */

/* Use the newer ALSA API */
#define ALSA_PCM_NEW_HW_PARAMS_API

#include <stdio.h>
#include <unistd.h>

#include <alsa/asoundlib.h>
#include "Broadcast.h"

int main(int argc, char **argv) {
	int rc;
	int res;
	int size;
	snd_pcm_t *handle;
	snd_pcm_hw_params_t *params;
	unsigned int val;
	int dir;
	snd_pcm_uframes_t frames;
	char *buffer;
	int num_frames_read;
	int readi_size;

	bcast_setup_tx_socket();

	/* Open PCM device for recording (capture). */
	rc = snd_pcm_open(&handle, "looprec", SND_PCM_STREAM_CAPTURE, 0);
	if (rc < 0) {
		fprintf(stderr,
				"unable to open pcm device: %s\n",
				snd_strerror(rc));
		exit(1);
	}

	/* Allocate a hardware parameters object. */
	snd_pcm_hw_params_alloca(&params);

	/* Fill it in with default values. */
	snd_pcm_hw_params_any(handle, params);

	/* Set the desired hardware parameters. */

	/* Interleaved mode */
	snd_pcm_hw_params_set_access(handle, params,
			SND_PCM_ACCESS_RW_INTERLEAVED);

	/* Signed 16-bit little-endian format */
	snd_pcm_hw_params_set_format(handle, params,
			SND_PCM_FORMAT_S16_LE);

	/* Two channels (stereo) */
	snd_pcm_hw_params_set_channels(handle, params, 2);

	/* 44100 bits/second sampling rate (CD quality) */
	val = 44100;
	snd_pcm_hw_params_set_rate_near(handle, params,
			&val, &dir);

	/* Set period size to 32 frames. */
	frames = 960;
	snd_pcm_hw_params_set_period_size_near(handle,
			params, &frames, &dir);

	/* Write the parameters to the driver */
	rc = snd_pcm_hw_params(handle, params);
	if (rc < 0) {
		fprintf(stderr,
				"unable to set hw parameters: %s\n",
				snd_strerror(rc));
		exit(1);
	}

	/* Use a buffer large enough to hold one period */
	snd_pcm_hw_params_get_period_size(params,
			&frames, &dir);
	size = frames * 8; /* 2 bytes/sample, 2 channels */
	buffer = (char *) malloc(size);

	snd_pcm_hw_params_get_period_time(params,
			&val, &dir);

	printf("Size of buffer: %d\n", size);
	printf("Num frames: %d\n", frames);
	while (1)
	{
		bzero(buffer, size);
		num_frames_read = snd_pcm_readi(handle, buffer, frames);
		readi_size = num_frames_read * 4;

//		printf("Buff:%s\n",buffer);
		if (num_frames_read == -EPIPE) {
			/* EPIPE means overrun */
			fprintf(stderr, "overrun occurred\n");
			snd_pcm_prepare(handle);
		} else if (num_frames_read < 0) {
			fprintf(stderr, "error from read: %s\n", snd_strerror(num_frames_read));
		} else if (num_frames_read != (int)frames) {
			fprintf(stderr, "short read, read %d frames\n", num_frames_read);
			continue;
		}
		res = bcast_tx(buffer, 0, readi_size);
		if (res < 0) {
			perror("sendto");
			exit(1);
		}
		if (res != readi_size) {
			fprintf(stderr, "short write: wrote %d bytes\n", num_frames_read);
		}

	}

	snd_pcm_drain(handle);
	snd_pcm_close(handle);
	free(buffer);

	return 0;
}
