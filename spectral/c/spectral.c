/*
  Implements a spectral analyzer. WAVE specification: http://www-mmsp.ece.mcgill.ca/Documents/AudioFormats/WAVE/WAVE.html

  Copyright (c) 2011 Dennis Jarosch. All rights reserved.
  Use of this source code is governed by a BSD-style license that can be
  found in the LICENSE file.
*/

#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>

#include "analysis.h"
#include "complex.h"

#define WAVE_FORMAT_PCM		 	0x0001
#define WAVE_FORMAT_IEEE_FLOAT 	0x0003
#define WAVE_FORMAT_ALAW		0x0006
#define WAVE_FORMAT_MULAW 		0x0007
#define WAVE_FORMAT_EXTENSIBLE 	0xFFFE

#define N_WINDOW 				256
#define OVERLAP					128

int info_flag  = 0;
int print_flag = 0;
//_______________________________________________________________________

typedef struct chunk
{
	char 		  id[4];
	unsigned int  size;
	char 		  format[4];
} chunk_t;

typedef struct format_chunk
{
	char  		   id[4];
	unsigned int   size;
	unsigned short format;
	unsigned short num_channels;
	unsigned int   sample_rate;
	unsigned int   byte_rate;
	unsigned short block_align;
	unsigned short bits_per_sample;
	/* extended */
	unsigned short ext_size;
	unsigned short valid_bits_per_sample;
	unsigned int   channel_mask;
	char           guid[16];
} format_t;

typedef struct fact_chunk
{
	char			id[4];
	unsigned int	size;
	unsigned int	sample_length;
} fact_t;

typedef struct data_chunk
{
	char			id[4];
	unsigned int	size;	
} data_t;

//_______________________________________________________________________

static size_t readChunk(FILE *fd, chunk_t *chunk)
{
	return fread((void*)chunk, sizeof(chunk_t), 1, fd);
}
//_______________________________________________________________________

static size_t readFormatChunk(FILE *fd, format_t *chunk)
{
	size_t elems;

	memset(chunk, 0, sizeof(format_t));
	elems = fread((void*)chunk, 24, 1, fd);

	/* handle extensions */
	if (chunk->size > 16)
	{
		elems = fread((void*)&chunk->ext_size, sizeof(unsigned short), 1, fd);

		if (chunk->ext_size > 0)		
			elems = fread((void*)&chunk->valid_bits_per_sample, chunk->ext_size, 1, fd);
	}

	return elems;
}
//_______________________________________________________________________

static size_t readFactChunk(FILE *fd, fact_t *chunk)
{
	size_t elems;

	elems = fread((void*)chunk, sizeof(fact_t), 1, fd);
	if (strncmp(chunk->id, "fact", 4) != 0)
	{
		/* no fact subchunk, seek backwards */
		fseek(fd, -sizeof(fact_t), SEEK_CUR);
		elems = 0;
	}

	return elems;
}
//_______________________________________________________________________

static size_t readDataChunk(FILE *fd, data_t *chunk)
{
	return fread((void*)chunk, sizeof(data_t), 1, fd);
}
//_______________________________________________________________________

static void formatString(format_t *s, char output[150])
{
	switch(s->format)
	{
		case WAVE_FORMAT_PCM:
			strcpy(output, "Uncompressed PCM");
		break;

		case WAVE_FORMAT_IEEE_FLOAT:
			strcpy(output, "IEEE float");
		break;

		case WAVE_FORMAT_ALAW:
			strcpy(output, "ALAW");
		break;

		case WAVE_FORMAT_MULAW:
			strcpy(output, "MULAW");
		break;

		case WAVE_FORMAT_EXTENSIBLE:
			strcpy(output, "Extensible");
		break;
	}
}
//_______________________________________________________________________

static void info(FILE * fd)
{
	size_t 		elems;
	chunk_t 	chunk;
	format_t 	format_chunk;
	fact_t		fact_chunk;
	data_t 		data_chunk;
	char 		format_string[150];

	/* read main chunk */
	elems = readChunk(fd, &chunk);
	printf("ChunkID: %4.4s\nSize: %u\nFormat: %4.4s\n", chunk.id, chunk.size, chunk.format);

	/* read format subchunk */
	elems = readFormatChunk(fd, &format_chunk);
	formatString(&format_chunk, format_string);

	printf("SubChunkID: %4.4s\n"
		   "Size: %u\n"
		   "Format: %s\n"
		   "Number of channels: %hu\n"
		   "Sample rate: %u\n"
		   "Byte rate: %u\n"
		   "Block align: %hu\n"
		   "Bits per sample: %hu\n"
		   "Extension size: %hu\n"
		   "Valid bits per sample: %hu\n"
		   "Channel mask: %u\n"
		   "SubFormat: %16s\n",
			format_chunk.id, format_chunk.size, format_string, format_chunk.num_channels,
			format_chunk.sample_rate, format_chunk.byte_rate, format_chunk.block_align, 
			format_chunk.bits_per_sample, format_chunk.ext_size, 
			format_chunk.valid_bits_per_sample, format_chunk.channel_mask, 
			format_chunk.guid);

	/* read fact subchunk */
	elems = readFactChunk(fd, &fact_chunk);
	if (elems)
		printf("SubChunkID: %4.4s\nSize: %u\nSample length: %u\n", fact_chunk.id, 
			fact_chunk.size, fact_chunk.sample_length);	
	
	/* read data subchunk */
	elems = readDataChunk(fd, &data_chunk);
	if (elems)
	printf("SubChunkID: %4.4s\nSize: %u\n", data_chunk.id, data_chunk.size);
}
//_______________________________________________________________________

static void analyze(FILE *fd)
{
	size_t 		elems;
	chunk_t 	chunk;
	format_t 	format_chunk;
	fact_t		fact_chunk;
	data_t 		data_chunk;

	/* read main chunk */
	elems = readChunk(fd, &chunk);
	if (elems != 1)
	{
		printf("Error reading main chunk, exiting...\n");
		exit(EXIT_FAILURE);
	}
	
	/* read format subchunk */
	elems = readFormatChunk(fd, &format_chunk);
	if (elems != 1)
	{
		printf("Error reading format chunk, exiting...\n");
		exit(EXIT_FAILURE);
	}

	/* benchmarking */
	struct timeval  tv_start, tv_end;
	struct timezone tz;

	gettimeofday(&tv_start, &tz);

	/* read fact subchunk */
	elems = readFactChunk(fd, &fact_chunk);
	
	/* read fact subchunk */
	elems = readDataChunk(fd, &data_chunk);

	/* alloc space for data, extended by N_WINDOW zeroed padding */
	int bytes_per_win = N_WINDOW * format_chunk.block_align; 
	int incr 		  = (N_WINDOW - OVERLAP) * format_chunk.block_align;
	int iterations 	  = (data_chunk.size - bytes_per_win) / incr + 1;

	if ((data_chunk.size - bytes_per_win) % incr)
		++iterations;

	int total_size    = (iterations - 1) * incr + bytes_per_win;

	char *data = calloc(total_size, 1);

	/* load data into memory */
	elems = fread((void*)data, 1, data_chunk.size, fd);

	int  i, n, left, right;
	char *data_ptr = data;
#ifdef PRINT
	int  j;
#endif

	int bytes_per_sample = format_chunk.bits_per_sample / 8;
	int sampling_mask    = pow(2, format_chunk.bits_per_sample) - 1;
	int two_comp_mask 	 = 1 << (format_chunk.bits_per_sample - 1);

	/* we support only 2 channels (stereo) for now */
	complex_t left_channel[N_WINDOW];
	complex_t right_channel[N_WINDOW];
	complex_t *result_left, *result_right;

	double *amps_left, *amps_right;

#ifdef PRINT
	double max = 0.0;
	
	if (print_flag)
		printf("# Time Frequency Amplitude\n");
#endif
	
	for (i = 0; i < iterations - 1; i++)
	{
		for (n = 0; n < N_WINDOW; n++)
		{
			left  = *(int*)(data_ptr + (format_chunk.block_align * n));
			right = *(int*)(data_ptr + (format_chunk.block_align * n) + bytes_per_sample);

			/* remove unwanted bits, to support 16 and 24 bit sampling */
			left  = left & sampling_mask;
			right = right & sampling_mask;

			/* convert to 2 complement signed */
			left  = left - ((left & two_comp_mask) << 1);
			right = right - ((right & two_comp_mask) << 1);

			/* hann window function */
			left_channel[n].im = 0;
			left_channel[n].re = left * hann(n, N_WINDOW - 1);

			right_channel[n].im = 0;
			right_channel[n].re = right * hann(n, N_WINDOW - 1);
		}

		/* perform FFT (Cooley-Tukey) */
		result_left  = fft(left_channel, N_WINDOW);
		result_right = fft(right_channel, N_WINDOW);

		amps_left  = amplitude(result_left, N_WINDOW);
		amps_right = amplitude(result_right, N_WINDOW);

#ifdef PRINT
		if (print_flag)
		{
			double time = (double)(i * incr) / 
				(bytes_per_sample * format_chunk.num_channels * format_chunk.sample_rate);

			for (j = 0; j < N_WINDOW / 2; j++)
			{
				printf("%.4lf %d %.2lf\n", time, 
					j * (format_chunk.sample_rate / (N_WINDOW / 2)), amps_left[j]);	

				if (amps_left[j] > max)
					max = amps_left[j];
			}
			printf("\n");
		}
#endif
			
		free(result_left);
		free(result_right);
		free(amps_left);
		free(amps_right);

		data_ptr = data_ptr + incr;
	}

	free(data);

	gettimeofday(&tv_end, &tz);

	unsigned duration = (tv_end.tv_sec * 1000 + tv_end.tv_usec / 1000) 
						- (tv_start.tv_sec * 1000 + tv_start.tv_usec / 1000);

	if (!print_flag)
		printf("Completed, time was %u ms (%.2f seconds)\n", duration, (duration / 1000.0));
#ifdef PRINT
	else
		printf("# max: %.2lf\n", max);
#endif
}
//_______________________________________________________________________

int main(int argc, char* argv[])
{
	int c;

	while (1)
	{
		static struct option long_options[] =
		{
			{"info",  no_argument, &info_flag,  1},
			{"print", no_argument, &print_flag, 1},
			{0, 0, 0, 0}
		};

		int option_index = 0;
     
		c = getopt_long (argc, argv, "ip", long_options, &option_index);

		if (c == -1)
			break;
	}

	if (optind >= argc)
	{
		printf("Usage: spectral [--info, --print] <filename.wav>\n");
		exit(EXIT_FAILURE);
	}

	FILE * fd = fopen(argv[optind], "r");

	if (!fd)
	{
		printf("Error opening file\n");
		exit(EXIT_FAILURE);
	}		

	if (info_flag)
		info(fd);
	else
		analyze(fd);

	exit(EXIT_SUCCESS);
}
//_______________________________________________________________________
