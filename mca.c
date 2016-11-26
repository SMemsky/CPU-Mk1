/*

CPU Mk1 MicroCode Assembler
Copyright 2016 Alexander Popov

About:

Simple tool for assembling .mc microcode into Logisim RAM/ROM images

Compile:

$ gcc mca.c -o mca

Input data:

0 d
...
n d

Output data:

v2.0 raw
d d d d d d d d
d d d d d d d d
...
d d d

*/

// NOTE: THERE IS A BUG (possibly) RELATED TO OVERFLOW OF 'buffer'
// This may lead to first value in output file to be zero

// NOTE: This is my first C program -- code may and will look bad

#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define BUFFER_SIZE 256
#define IMAGE_SIZE 64

int main(int argc, char* argv[])
{
	char* input_file = NULL;
	char* output_file = NULL;

	FILE* input = NULL;
	FILE* output = NULL;

	char* buffer = calloc(BUFFER_SIZE, sizeof(char));
	uint64_t* code = calloc(IMAGE_SIZE, sizeof(uint64_t));
	unsigned highest_index = 0;

	if (argc < 3)
	{
		printf("Usage: %s input_file output_file\n", argv[0]);
	}

	input_file = argv[1];
	output_file = argv[2];

	input = fopen(input_file, "r");
	if (input != NULL)
	{
		while (fgets(buffer, BUFFER_SIZE, input) != NULL)
		{
			char* comment_start = strstr(buffer, "//");
			if (comment_start != NULL)
			{
				*comment_start = 0;
			}

			char* start = buffer;
			while (isspace(*start))
			{
				++start;
			}
			if (*start == 0)
			{
				continue;
			}

			uint16_t address = 0;
			char temp_buffer[BUFFER_SIZE];

			sscanf(start, "%x %s", &address, temp_buffer);

			bool unexpected = false;
			char* a = temp_buffer;
			uint64_t value = 0;
			do
			{
				if (*a == '|')
				{
					++a;
					continue;
				}
				else if (*a == '1' || *a == '0')
				{
					int b = *a == '1' ? 1 : 0;
					value = (value << 1) | b;
				}
				else
				{
					unexpected = true;
					break;
				}

				++a;
			} while (*a != 0);

			if (unexpected)
			{
				printf("Unexpected character: '%c'", *a);

				break;
			}

			code[address] = value;

			if (highest_index < address)
			{
				highest_index = address;
			}
		}

		fclose(input);
	}
	else
	{
		printf("Unable to open file '%s'\n", input_file);

		return 0;
	}

	// Write result to output

	output = fopen(output_file, "w");
	if (output != NULL)
	{
		// Write Logisim header
		fprintf(output, "v2.0 raw\n");

		unsigned i = 0;
		for (i = 0; i <= highest_index; ++i)
		{
			if (i % 8 == 7 || i == highest_index)
			{
				fprintf(output, "%x\n", code[i]);
			}
			else
			{
				fprintf(output, "%x ", code[i]);
			}
		}

		fclose(output);
	}
	else
	{
		printf("Unable to open or create file '%s'\n", output_file);

		return 0;
	}

	free(code);
	free(buffer);

	return 0;
}
