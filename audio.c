#include "audio.h"
#include "strl.h"

unsigned num_sfx = 0;
sfx_t** sfxs = NULL;

void mixer_render()
{
   for (unsigned j = 0; j < AUDIO_FRAMES; j++)
      audio_buffer[j*2+0] = audio_buffer[j*2+1] = 0;

   for (unsigned i = 0; i < num_sfx; i++)
   {
      sfxs[i]->samples_to_read = AUDIO_FRAMES;

      if (sfxs[i]->pos + sfxs[i]->samples_to_read > sfxs[i]->num_samples)
         sfxs[i]->samples_to_read = sfxs[i]->num_samples - sfxs[i]->pos;

      uint8_t* rawsamples8  = (uint8_t*)sfxs[i]->rawsamples + sfxs[i]->bytes_per_sample * sfxs[i]->pos;
      int16_t* rawsamples16 = (int16_t*)rawsamples8;
      
      for (unsigned j = 0; j < sfxs[i]->samples_to_read; j++)
      {
         audio_buffer[j*2+0] += rawsamples16[j];
         audio_buffer[j*2+1] += rawsamples16[j];
      }

      sfxs[i]->pos += AUDIO_FRAMES;
      if (sfxs[i]->pos > sfxs[i]->num_samples)
         sfxs[i]->pos = sfxs[i]->loop ? 0 : sfxs[i]->num_samples;
   }
}

void sfx_play(sfx_t * self)
{
   self->pos = 0;
}

bool sfx_is_playing(sfx_t * self)
{
   return self->pos < self->num_samples;
}

sfx_t* sfx_new(char *name, bool loop)
{
   unsigned wavsize = 0;
   uint8_t wavdata[4096*1024];

   char path[1024];
   strlcpy(path, "/usr/share/obake/", sizeof(path));
   strlcat(path, name, sizeof(path));

   FILE *fp = fopen(path, "rb");
   if (fp)
   {
      fseek(fp, 0L, SEEK_END);
      wavsize = ftell(fp);
      fseek(fp, 0L, SEEK_SET);

      fread(wavdata, sizeof(uint8_t), sizeof(wavdata)-1, fp);
      fclose(fp);
   }

   wavhead_t head;

   memcpy(&head, wavdata, 44);

   sfx_t *self = NULL;
   self = (sfx_t*)realloc(self, sizeof(sfx_t));
   self->bytes_per_sample = head.NumChannels * head.BitsPerSample / 8;
   self->num_samples = head.Subchunk2Size / self->bytes_per_sample;
   self->rawsamples = malloc(head.Subchunk2Size);
   memcpy(self->rawsamples, (uint8_t*)wavdata+44, wavsize-44);
   self->loop = loop;
   self->pos = self->num_samples;

   num_sfx++;
   sfxs = (sfx_t**)realloc(sfxs, num_sfx * sizeof(sfx_t));
   sfxs[num_sfx-1] = self;

   return self;
}