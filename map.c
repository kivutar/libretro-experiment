#include "game.h"
#include "json.h"
#include "strl.h"

static json_value data;
static int width, height, tilewidth, tileheight;
surface_t tileset_surfaces[10];

static void update(entity_t *self)
{
}

static void draw(entity_t *self)
{
   int i, j, k;
   for(i = 0; i < data.u.object.length; i++) {
      if (!strcmp(data.u.object.values[i].name, "layers")) {
         json_value *layers = data.u.object.values[i].value;
         for(j = 0; j < layers->u.array.length; j++) {
            json_value *layer = layers->u.array.values[j];
            for(k = 0; k < layer->u.object.length; k++) {
               if (!strcmp(layer->u.object.values[k].name, "data")) {
                  json_value *layerdata = layer->u.object.values[k].value;

                  int x, y;
                  for (y = 0; y < height; y++) {
                     for (x = 0; x < width; x++) {
                        int id = layerdata->u.array.values[y*width+x]->u.integer;
                        if (id)
                        {
                           int tileset_id = 0;
                           int tileset_numtiles = 0;

                           tileset_numtiles = 
                                (tileset_surfaces[1].w/tilewidth) * (tileset_surfaces[1].h/tileheight) 
                              + (tileset_surfaces[0].w/tilewidth) * (tileset_surfaces[0].h/tileheight);

                           if (id > tileset_numtiles)
                           {
                              tileset_id = 2;
                              id -= tileset_numtiles;
                           }

                           tileset_numtiles = (tileset_surfaces[0].w/tilewidth)
                              *(tileset_surfaces[0].h/tileheight);

                           if (id > tileset_numtiles)
                           {
                              tileset_id = 1;
                              id -= tileset_numtiles;
                           }

                           draw_tile(
                              x*tilewidth, 
                              y*tileheight, 
                              tilewidth, 
                              tileheight, 
                              tileset_surfaces[tileset_id].w, 
                              tileset_surfaces[tileset_id].h, 
                              tileset_surfaces[tileset_id].image, 
                              id);
                        }
                     }
                  }
               }
            }
         }
      }
   }
}

entity_t* map_new()
{
   char jsonstring[4096*16];
   FILE *fp = fopen("/usr/share/obake/test.json", "rb");
   if (fp)
   {
      fread(jsonstring, sizeof(char), sizeof(jsonstring)-1, fp);
      fclose(fp);
   }

   data = * json_parse(jsonstring, strlen(jsonstring));

   int i, j, k, l, m;
   for(i = 0; i < data.u.object.length; i++) {

      if (!strcmp(data.u.object.values[i].name, "width"))
         width = data.u.object.values[i].value->u.integer;

      if (!strcmp(data.u.object.values[i].name, "height"))
         height = data.u.object.values[i].value->u.integer;

      if (!strcmp(data.u.object.values[i].name, "tilewidth"))
         tilewidth = data.u.object.values[i].value->u.integer;

      if (!strcmp(data.u.object.values[i].name, "tileheight"))
         tileheight = data.u.object.values[i].value->u.integer;

      if (!strcmp(data.u.object.values[i].name, "tilesets")) {
         json_value *tilesets = data.u.object.values[i].value;
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

                  tileset_surfaces[j] = surface_new(filename);
               }
            }
         }
      }

      if (!strcmp(data.u.object.values[i].name, "layers")) {
         json_value *layers = data.u.object.values[i].value;
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

   entity_t *self = NULL;
   self = (entity_t*)realloc(self, sizeof(entity_t));

   self->x = 0;
   self->y = 0;
   self->w = 0;
   self->h = 0;
   self->update = &update;
   self->draw = &draw;
   self->on_collide = NULL;

   num_entities++;
   entities = (entity_t**)realloc(entities, num_entities * sizeof(entity_t));
   entities[num_entities-1] = self;

   return self;
}
