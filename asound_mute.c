#include <alsa/asoundlib.h>
#include <stdio.h>
#include <stdlib.h>

void SetAlsaMasterMute(int volume)
{
	long min, max;
	int step;
	snd_mixer_t *handle;
	snd_mixer_selem_id_t *sid;
	const char *card = "default";
	const char *selem_name = "Master";

	snd_mixer_open(&handle, 0);
	snd_mixer_attach(handle, card);
	snd_mixer_selem_register(handle, NULL, NULL);
	snd_mixer_load(handle);

	snd_mixer_selem_id_alloca(&sid);
	snd_mixer_selem_id_set_index(sid, 0);
	snd_mixer_selem_id_set_name(sid, selem_name);
	snd_mixer_elem_t* elem = snd_mixer_find_selem(handle, sid);

	if (snd_mixer_selem_has_playback_switch(elem)) {
		snd_mixer_selem_get_playback_volume_range(elem, &min, &max);
		step = (max - min) / 100;
		volume = volume * step;
		snd_mixer_selem_set_playback_volume_all(elem, volume);
	}

	snd_mixer_close(handle);
}

int main(int argc, char **argv) {
	if(argc < 2) {
		printf("USAGE: int value\n");
		return 1;
	}
	SetAlsaMasterMute(atoi(argv[1]));
	return 0;
}
