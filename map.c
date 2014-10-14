#include "game.h"
#include "strl.h"
#include "map.h"

void map_draw_layer(map_t *self, unsigned layer_id)
{
   int i, k;
   for(i = 0; i < self->data.u.object.length; i++) {
      if (!strcmp(self->data.u.object.values[i].name, "layers")) {
         json_value *layers = self->data.u.object.values[i].value;
         json_value *layer = layers->u.array.values[layer_id];
         for(k = 0; k < layer->u.object.length; k++) {
            if (!strcmp(layer->u.object.values[k].name, "data")) {
               json_value *layerdata = layer->u.object.values[k].value;

               int x, y;
               for (y = 0; y < self->height; y++) {
                  for (x = 0; x < self->width; x++) {
                     int id = layerdata->u.array.values[y*self->width+x]->u.integer;
                     if (id)
                     {
                        int tileset_id = 0;
                        int tileset_numtiles = 0;

                        tileset_numtiles = 
                             (self->surfaces[1].w/self->tilewidth) * (self->surfaces[1].h/self->tileheight) 
                           + (self->surfaces[0].w/self->tilewidth) * (self->surfaces[0].h/self->tileheight);

                        if (id > tileset_numtiles)
                        {
                           tileset_id = 2;
                           id -= tileset_numtiles;
                        }

                        tileset_numtiles = (self->surfaces[0].w/self->tilewidth)
                           *(self->surfaces[0].h/self->tileheight);

                        if (id > tileset_numtiles)
                        {
                           tileset_id = 1;
                           id -= tileset_numtiles;
                        }

                        draw_tile(
                           x*self->tilewidth, 
                           y*self->tileheight, 
                           self->tilewidth, 
                           self->tileheight, 
                           self->surfaces[tileset_id].w, 
                           self->surfaces[tileset_id].h, 
                           self->surfaces[tileset_id].image, 
                           id);
                     }
                  }
               }
            }
         }
      }
   }
}

map_t* map_new(char *name)
{
   char path[1024];
   strlcpy(path, "/usr/share/obake/", sizeof(path));
   strlcat(path, name, sizeof(path));

   char jsonstring[4096*16];
   FILE *fp = fopen(path, "rb");
   if (fp)
   {
      fread(jsonstring, sizeof(char), sizeof(jsonstring)-1, fp);
      fclose(fp);
   }

   map_t *self = NULL;
   self = (map_t*)realloc(self, sizeof(map_t));

   self->data = * json_parse(jsonstring, strlen(jsonstring));

   self->surfaces = (surface_t*)malloc(16*sizeof(surface_t));

   int i, j, k, l, m;
   for(i = 0; i < self->data.u.object.length; i++) {

      if (!strcmp(self->data.u.object.values[i].name, "width"))
         self->width = self->data.u.object.values[i].value->u.integer;

      if (!strcmp(self->data.u.object.values[i].name, "height"))
         self->height = self->data.u.object.values[i].value->u.integer;

      if (!strcmp(self->data.u.object.values[i].name, "tilewidth"))
         self->tilewidth = self->data.u.object.values[i].value->u.integer;

      if (!strcmp(self->data.u.object.values[i].name, "tileheight"))
         self->tileheight = self->data.u.object.values[i].value->u.integer;

      if (!strcmp(self->data.u.object.values[i].name, "tilesets")) {
         json_value *tilesets = self->data.u.object.values[i].value;
         for(j = 0; j < tilesets->u.array.length; j++) {
            json_value *tileset = tilesets->u.array.values[j];

            char filename[1024];

            for(k = 0; k < tileset->u.object.length; k++)
            {
               if (!strcmp(tileset->u.object.values[k].name, "image"))
               {
                  strlcpy(filename, "/usr/share/obake/", sizeof(filename));
                  strlcat(filename, tileset->u.object.values[i].value->u.string.ptr, sizeof(filename));
                  strlcat(filename, ".png", sizeof(filename));

                  self->surfaces[j] = surface_new(filename);
               }
            }
         }
      }

      if (!strcmp(self->data.u.object.values[i].name, "layers")) {
         json_value *layers = self->data.u.object.values[i].value;
         for(j = 0; j < layers->u.array.length; j++) {
            json_value *layer = layers->u.array.values[j];
            for(k = 0; k < layer->u.object.length; k++) {
               if (!strcmp(layer->u.object.values[k].name, "objects")) {
                  json_value *objs = layer->u.object.values[k].value;
                  for(l = 0; l < objs->u.array.length; l++) { // 1
                     json_value *obj = objs->u.array.values[l];
                     int ox = 0;
                     int oy = 0;
                     int ow = 0;
                     int oh = 0;
                     for(m = 0; m < obj->u.object.length; m++)
                     {
                        if (!strcmp(obj->u.object.values[m].name, "x"))
                           ox = obj->u.object.values[m].value->u.integer;

                        if (!strcmp(obj->u.object.values[m].name, "y"))
                           oy = obj->u.object.values[m].value->u.integer;

                        if (!strcmp(obj->u.object.values[m].name, "width"))
                           ow = obj->u.object.values[m].value->u.integer;

                        if (!strcmp(obj->u.object.values[m].name, "height"))
                           oh = obj->u.object.values[m].value->u.integer;
                     }
                     ground_new(ox, oy, ow, oh);
                  }
               }
            }
         }
      }
   }

   return self;
}
