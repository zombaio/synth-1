#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "synth.h"

/* mul.c: multiply waveforms */

#define MAXINPUTS 288
static void mul(FILE **inputs, int numfiles);

int main(int argc, char *argv[])
{
	int i;
	FILE *inputs[MAXINPUTS];
	int numfiles = 0;

	for (i = 1; i < argc; i++)
	{
		if (!strcmp(argv[i], "-help"))
		{
			fprintf(stderr, "options: none\n");
			fprintf(stderr, "Also list 1 to %d input filenames.\n",
				MAXINPUTS);
			exit(0);
		}
		else if (argv[i][0] != '-')
		{
			if (numfiles == MAXINPUTS)
			{
				fprintf(stderr, "mul: too many files\n");
				return 1;
			}
			inputs[numfiles] = fopen(argv[i], "rb");
			if (inputs[numfiles] == NULL)
			{
				fprintf(stderr, "mul: can't open %s\n",
					argv[i]);
			}
			else
				numfiles++;
		}
	}

	if (numfiles == 0)
	{
		fprintf(stderr, "mul: no files\n");
		return 1;
	}

	SET_BINARY_MODE
	mul(inputs, numfiles);

	for (i = 0; i < numfiles; i++)
		fclose(inputs[i]);

	return 0;
}

static void mul(FILE **inputs, int numfiles)
{
	int i;
	int filedone[MAXINPUTS];
	int filesleft = numfiles;
	float f;

	memset(filedone, 0, numfiles * sizeof filedone[0]);

	for (;;)
	{
		float product = 1.0f;

		for (i = 0; i < numfiles; i++)
		{
			if (filedone[i])
				continue;

			if (fread(&f, sizeof f, 1, inputs[i]) < 1)
			{
				filedone[i] = 1;
				filesleft--;
				if (filesleft == 0)
					return;
				continue;
			}

			product *= f;
		}

		f = product;
		fwrite(&f, sizeof f, 1, stdout);
	}
}
