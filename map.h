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
	surface_t* surfaces;
} map_t;

map_t* map_new(char *name);
void map_draw_layer(map_t *self, unsigned layer_id);

#endif