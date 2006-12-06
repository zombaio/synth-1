#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "synth.h"

/* amp.c: adjust a waveform's volume */

static void amp(float volume);

int main(int argc, char *argv[])
{
	float vol = 1.0f;
	int i;

	for (i = 1; i < argc; i++)
	{
		if (!strcmp(argv[i], "-vol") && i+1 < argc)
			vol = atof(argv[++i]);
		else if (!strcmp(argv[i], "-help"))
		{
			fprintf(stderr, "options: -vol multiplicand\n");
			exit(0);
		}
	}

	SET_BINARY_MODE
	amp(vol);
	return 0;
}

static void amp(float volume)
{
	float f;

	while (fread(&f, sizeof f, 1, stdin) == 1)
	{
		f *= volume;

		if (fwrite(&f, sizeof f, 1, stdout) < 1)
			return;
	}
}
