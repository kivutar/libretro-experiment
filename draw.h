#ifndef DRAW_H
#define DRAW_H

#include <stdlib.h>
#include "rpng.h"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

typedef struct 
{
   int x;
   int y;
} point_t;

typedef struct surface_s
{
   uint32_t *image;
   unsigned w;
   unsigned h;
} surface_t;

typedef struct anim_s
{
   surface_t surface;
   int t;
   int p;
   int w;
   int h;
} anim_t;

surface_t surface_new(char *name);
void draw_rect(int x, int y, int w, int h, uint32_t c);
void blit(int dest_x, int dest_y, int w, int h, int total_w, int total_h, uint32_t *data, int orig_x, int orig_y);
void draw_image(int x, int y, int w, int h, uint32_t *data);
void draw_tile(int dest_x, int dest_y, int w, int h, int total_w, int total_h, uint32_t *data, int id);
void draw_anim(int dest_x, int dest_y, anim_t* anim);

uint32_t fb[SCREEN_WIDTH * SCREEN_HEIGHT * 2];
size_t fbpitch;

point_t camera;

#endif
