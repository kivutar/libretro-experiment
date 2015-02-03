#include "audio.h"
#include "strl.h"

unsigned num_sfx = 0;
sfx_t** sfxs = NULL;

void mixer_render(int16_t *buffer)
{
   // Clear buffer
   for (unsigned j = 0; j < AUDIO_FRAMES; j++)
   {
      buffer[j*2+0] = buffer[j*2+1] = 0;
   }

   // Loop over sound effects
   for (unsigned i = 0; i < num_sfx; i++)
   {
      uint8_t* rawsamples8 = calloc(
         AUDIO_FRAMES * sfxs[i]->bps, sizeof(uint8_t));

      bool end = ! fread(rawsamples8,
            sizeof(uint8_t),
            AUDIO_FRAMES * sfxs[i]->bps,
            sfxs[i]->fp);

      int16_t* rawsamples16 = (int16_t*)rawsamples8;
      
      for (unsigned j = 0; j < AUDIO_FRAMES; j++)
      {
         buffer[j*2+0] += rawsamples16[j];
         buffer[j*2+1] += rawsamples16[j];
      }

      if (end && sfxs[i]->loop)
      {
         fseek(sfxs[i]->fp, WAV_HEADER_SIZE, SEEK_SET);
      }

      free(rawsamples8);
   }
}

void sfx_play(sfx_t * self)
{
   fseek(self->fp, WAV_HEADER_SIZE, SEEK_SET);
}

sfx_t* sfx_new(char *path, bool loop)
{
   FILE *fp = fopen(path, "rb");
   if (!fp)
   {
      return NULL;
   }

   wavhead_t head;

   fread(&head, sizeof(uint8_t), WAV_HEADER_SIZE, fp);

   sfx_t *self = NULL;
   self = (sfx_t*)calloc(1, sizeof(sfx_t));
   self->bps = head.NumChannels * head.BitsPerSample / 8;
   self->fp = fp;
   self->loop = loop;
   fseek(self->fp, 0, SEEK_END);

   num_sfx++;
   sfxs = (sfx_t**)realloc(sfxs, num_sfx * sizeof(sfx_t));
   sfxs[num_sfx-1] = self;

   return self;
}