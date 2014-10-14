#ifndef AUDIO_H
#define AUDIO_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define AUDIO_FRAMES (44100 / 60)

int16_t audio_buffer[2 * AUDIO_FRAMES];

typedef struct
{
   char ChunkID[4];
   uint32_t ChunkSize;
   char Format[4];
   char Subchunk1ID[4];
   uint32_t Subchunk1Size;
   uint16_t AudioFormat;
   uint16_t NumChannels;
   uint32_t SampleRate;
   uint32_t ByteRate;
   uint16_t BlockAlign;
   uint16_t BitsPerSample;
   char Subchunk2ID[4];
   uint32_t Subchunk2Size;
} wavhead_t;

typedef struct
{
   wavhead_t head;
   unsigned bytes_per_sample;
   void* rawsamples;
   unsigned pos;
   unsigned num_samples;
   unsigned samples_to_read;
   int16_t* tmp_buffer;
   bool loop;
} sfx_t;

sfx_t* sfx_new(char *name, bool loop);
void sfx_play(sfx_t * sfx);
void mixer_render();

#endif