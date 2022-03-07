#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <math.h>

typedef struct {
	char chunk_id[4];
	uint32_t chunk_size;
	char format[4];
	
	char subchunk1_id[4];
	uint32_t subchunk1_size;
	uint16_t audio_format;
	uint16_t channels;
	uint32_t sample_rate;
	uint32_t byte_rate;
	uint16_t block_align;
	uint16_t bits_per_sample;
	
	char subchunk2_id[4];
	uint32_t subchunk2_size;
} wav_header;

typedef struct {
	int16_t left;
	int16_t right;
} sample;

#define BITS_PER_SAMPLE 16
#define SAMPLE_RATE 44100
#define CHANNELS 2

void write_wav_header(FILE* wav_file, uint32_t frame_count) {
	wav_header header;
	
	// RIFF Header
	header.chunk_id[0] = 'R';
	header.chunk_id[1] = 'I';
	header.chunk_id[2] = 'F';
	header.chunk_id[3] = 'F';

	header.chunk_size = 36 + frame_count * CHANNELS * BITS_PER_SAMPLE / 8;
	
	header.format[0] = 'W';
	header.format[1] = 'A';
	header.format[2] = 'V';
	header.format[3] = 'E';
	
	// fmt Chunk
	header.subchunk1_id[0] = 'f';
	header.subchunk1_id[1] = 'm';
	header.subchunk1_id[2] = 't';
	header.subchunk1_id[3] = ' ';
	
	header.subchunk1_size = 16;
	header.audio_format = 1;
	header.channels = CHANNELS;
	header.sample_rate = SAMPLE_RATE;
	header.byte_rate = SAMPLE_RATE * CHANNELS * BITS_PER_SAMPLE / 8;
	header.block_align = CHANNELS * BITS_PER_SAMPLE / 8;
	header.bits_per_sample = BITS_PER_SAMPLE;
	
	// data chunk
	header.subchunk2_id[0] = 'd';
	header.subchunk2_id[1] = 'a';
	header.subchunk2_id[2] = 't';
	header.subchunk2_id[3] = 'a';
	header.subchunk2_size = frame_count * CHANNELS * BITS_PER_SAMPLE / 8;
	
	fwrite(&header, sizeof(header), 1, wav_file);
}

sample* generate_samples(int freq, int frames) {
	sample* samples = malloc(sizeof(sample) * frames);
	
	double t = M_PI * 2 * freq / SAMPLE_RATE;
	for (int i = 0; i < frames; i++) {
		double s = INT16_MAX * sin(t * i);
		samples[i].left = samples[i].right = s;
	}
	
	return samples;
}

void write_wav(char* filename, int freq, int dur) {
	FILE* wav = fopen(filename, "wb");
	assert(wav);
	
	int total_frames = SAMPLE_RATE * dur/1000;
	sample* samples = generate_samples(freq, total_frames);
	
	write_wav_header(wav, total_frames);
	fwrite(samples, sizeof(sample), total_frames, wav);	
}

int main(int argc, char* argv[]) {
	int freq, dur;
	char* endptr;
	
	if (argc != 4 || *argv[1] == '\0' || *argv[2] == '\0' || *argv[3] == '\0') {
		printf("Usage: %s [frequency] [duration in millisecs]\n", argv[0]);
		return -1;
	}
		
	freq = strtol(argv[1], &endptr, 10);
	if (*endptr != '\0') {
		printf("Invalid argument: %s\n", argv[1]);
		return -1;
	}
	dur = strtol(argv[2], &endptr, 10);
	if (*endptr != '\0') {
		printf("Invalid argument: %s\n", argv[2]);
		return -1;
	}
	
	write_wav(argv[3], freq, dur);
}
