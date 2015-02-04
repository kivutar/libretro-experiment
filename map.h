#ifndef MAP_H
#define MAP_H

#include "draw.h"
#include "json.h"

typedef struct
{
   unsigned width;
   unsigned height;
   unsigned tilewidth;
   unsigned tileheight;
   json_value data;
   void (*tileset_callback)(char *, unsigned);
   void (*object_callback)(char *, int, int, int, int);
} map_t;

map_t* map_new(char *name,
      void (*tileset_callback)(char *, unsigned),
      void (*object_callback)(char *, int, int, int, int));
void map_draw_layer(map_t *self, surface_t *surfaces, unsigned layer_id);

#endif