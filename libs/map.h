#ifndef MAP_H
#define MAP_H

#include "json.h"
#include "strl.h"
#include <stdio.h>
#include <string.h>

typedef struct
{
   unsigned width;
   unsigned height;
   unsigned tilewidth;
   unsigned tileheight;
   json_value data;
   void (*tileset_callback)(char *, unsigned);
   void (*object_callback)(char *, int, int, int, int);
   void (*tile_callback)(int, int, int, int, int);
} map_t;

map_t* map_new(char *path,
      void (*tileset_callback)(char *, unsigned),
      void (*object_callback)(char *, int, int, int, int),
      void (*tile_callback)(int, int, int, int, int));
void map_draw_layer(map_t *self, unsigned layer_id);

#endif