#ifndef GAME_H
#define GAME_H

#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <stdbool.h>
#include "libretro.h"
#include "rpng.h"
#include "audio.h"
#include "map.h"
#include "strl.h"

typedef struct entity_s
{
   int w;
   int h;
   int x;
   int y;
   int f;
   float t;
   bool  d;
   anim_t *anim;
   void (*update)(struct entity_s *);
   void (*draw)(struct entity_s *);
   void (*on_collide)(struct entity_s *, struct entity_s *, int dx, int dy);
} entity_t;

typedef struct
{
   int up;
   int down;
   int left;
   int right;
   int start;
   int select;
   int a;
   int b;
   int x;
   int y;
} key_state_t;

key_state_t ks;

void add_entity(entity_t* ent);

void load_game();
void update_game();
void draw_game();

void detect_collisions();
bool solid_at(int x, int y);

entity_t* obake_new();
entity_t* flame_new(entity_t *center);
entity_t* ninja_new();
entity_t* ground_new(int x, int y, int w, int h);

extern unsigned num_entities;
extern entity_t** entities;

extern unsigned num_sfx;
extern sfx_t** sfxs;

sfx_t *sfx_jump;
sfx_t *sfx_step;

map_t *map_test;

#endif
