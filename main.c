#define ALSA_PCM_NEW_HW_PARAMS_API

#include <alsa/asoundlib.h>
#include <stdio.h>

void print_info(void);
void test_open(void);
void set_hardware_params(snd_pcm_t *handle, snd_pcm_hw_params_t *params);
void write_params(snd_pcm_t *handle, snd_pcm_hw_params_t *params);
void describe_device(snd_pcm_t *handle, snd_pcm_hw_params_t *params);

int main(void)
{
	print_info();
	test_open();
	return 0;
}

void print_info(void)
{
	int val;

	printf("\nALSA library version: %s\n", SND_LIB_VERSION_STR);

	printf("\nPCM stream types:\n");
	for (val = 0; val <= SND_PCM_STREAM_LAST; val++)
		printf("- %s\n", snd_pcm_stream_name((snd_pcm_stream_t) val));

	printf("\nPCM access types:\n");
	for (val = 0; val <= SND_PCM_ACCESS_LAST; val++)
		printf("- %s\n", snd_pcm_access_name((snd_pcm_access_t) val));

	printf("\nPCM formats:\n");
	for (val = 0; val <= SND_PCM_FORMAT_LAST; val++) {
		if (snd_pcm_format_name((snd_pcm_format_t) val) != NULL)
			printf("- %s (%s)\n", snd_pcm_format_name(val), snd_pcm_format_description(val));
	}

	printf("\nPCM subsubformats:\n");
	for (val = 0; val <= SND_PCM_SUBFORMAT_LAST; val++)
		printf("- %s (%s)\n", snd_pcm_subformat_name(val), snd_pcm_subformat_description(val));

	printf("\nPCM states:\n");
	for (val = 0; val <= SND_PCM_STATE_LAST; val++)
		printf("- %s\n", snd_pcm_state_name((snd_pcm_state_t) val));

	return;
}

/* Open the default PCM device, set some parameters, and print
 * information about the device. */
void test_open(void)
{
	snd_pcm_t *handle;
	snd_pcm_hw_params_t *params;
	int rc;

	/* Open the PCM device for playback */
	if ((rc = snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
		fprintf(stderr, "Unable to open default PCM device: %s\n", snd_strerror(rc));
		exit(1);
	}

	/* Allocate a hardware param object */
	snd_pcm_hw_params_alloca(&params);

	/* Fill the params with default values */
	snd_pcm_hw_params_any(handle, params);

	set_hardware_params(handle, params);
	write_params(handle, params);
	describe_device(handle, params);
	snd_pcm_close(handle);
	return;
}

void set_hardware_params(snd_pcm_t *handle, snd_pcm_hw_params_t *params)
{
	/* Interleaved mode */
	snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);

	/* Signed 16-bit little-endian format */
	snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE);

	/* Two channels (stereo) */
	snd_pcm_hw_params_set_channels(handle, params, 2);

	/* 44100 bits/second sampling rate (CD quality) */
	unsigned int val = 44100;
	int dir;
	snd_pcm_hw_params_set_rate_near(handle, params, &val, &dir);

	return;
}

void write_params(snd_pcm_t *handle, snd_pcm_hw_params_t *params)
{
	int rc;

	if ((rc = snd_pcm_hw_params(handle, params)) < 0) {
		fprintf(stderr, "Unable to set hw params: %s\n", snd_strerror(rc));
		exit(1);
	}

	return;
}

void describe_device(snd_pcm_t *handle, snd_pcm_hw_params_t *params)
{
	unsigned int access;
	printf("\nPCM Handle Name: %s\n", snd_pcm_name(handle));
	printf("PCM State: %s\n", snd_pcm_state_name(snd_pcm_state(handle)));
	snd_pcm_hw_params_get_access(params, (snd_pcm_access_t *)&access);
	printf("PCM Access type: %s\n", snd_pcm_access_name((snd_pcm_access_t) access));
	return;
}
