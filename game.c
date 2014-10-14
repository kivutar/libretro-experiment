#include "game.h"

int num_entities = 0;
entity_t** entities = NULL;

surface_t surface_new(char *name)
{
   surface_t s;
   rpng_load_image_argb(name, &s.image, &s.w, &s.h);
   return s;
}

void draw_rect(int x, int y, int w, int h, uint32_t c)
{
   int i, j;
   for (j = y; j < y+h; j++)
      for (i = x; i < x+w; i++)
         fb[j * (fbpitch >> 1) + i] = c;
}

void blit(int dest_x, int dest_y, int w, int h, int total_w, int total_h, uint32_t *data, int orig_x, int orig_y)
{
   int i, j;
   int jj = orig_y;
   int imgpitch = total_w * sizeof(uint16_t);
   for (j = dest_y; j < dest_y+h; j++) {
      int ii = orig_x;
      if (j >= 0 && j < SCREEN_HEIGHT) {
         for (i = dest_x; i < dest_x+w; i++) {
            if (i >= 0 && i < SCREEN_WIDTH) {
               uint32_t c = data[jj * (imgpitch >> 1) + ii];
               if (0xff000000 & c)
                  fb[j * (fbpitch >> 1) + i] = c;
            }
            ii++;
         }
      }
      jj++;
   }
}

void draw_image(int x, int y, int w, int h, uint32_t *data)
{
   blit(x, y, w, h, w, h, data, 0, 0);
}

void draw_tile(int dest_x, int dest_y, int w, int h, int total_w, int total_h, uint32_t *data, int id)
{
   int orig_x = ((id-1)%(total_w/w))*w;
   int orig_y = ((id-1)/(total_w/w))*w;
   blit(dest_x + camera.x, dest_y + camera.y, w, h, total_w, total_h, data, orig_x, orig_y);
}

void draw_anim(int dest_x, int dest_y, anim_t *anim)
{
   int steps = anim->surface.w / anim->w;

   anim->t++;
   if (anim->t >= steps * anim->p)
      anim->t = 0;

   if (!anim->p)
      anim->p = 1;

   draw_tile(
      dest_x, 
      dest_y, 
      anim->w, 
      anim->h, 
      anim->surface.w, 
      anim->surface.h, 
      anim->surface.image, 
      anim->t/anim->p + 1);
}

void load_game()
{
   map_new();
   ninja_new();
   obake_new();

   sfx_jump = sfx_new("jump.wav", false);
   sfx_new("twinstudio.wav", true);
}

void render_game()
{
   int i;
   for(i = 0; i < num_entities; i++)
      entities[i]->update(entities[i]);

   draw_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0xff000000);

   detect_collisions();

   for(i = 0; i < num_entities; i++)
      entities[i]->draw(entities[i]);  
   
   video_cb(fb, SCREEN_WIDTH, SCREEN_HEIGHT, fbpitch*2);
}
