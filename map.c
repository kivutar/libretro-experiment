#include "game.h"
#include "strl.h"
#include "map.h"

void map_draw_layer(map_t *self, surface_t *surfaces, unsigned layer_id)
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

                        tileset_numtiles
                           = (surfaces[1].w/self->tilewidth)
                           * (surfaces[1].h/self->tileheight)
                           + (surfaces[0].w/self->tilewidth)
                           * (surfaces[0].h/self->tileheight);

                        if (id > tileset_numtiles)
                        {
                           tileset_id = 2;
                           id -= tileset_numtiles;
                        }

                        tileset_numtiles
                           = (surfaces[0].w/self->tilewidth)
                           * (surfaces[0].h/self->tileheight);

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
                           surfaces[tileset_id].w, 
                           surfaces[tileset_id].h, 
                           surfaces[tileset_id].image, 
                           id);
                     }
                  }
               }
            }
         }
      }
   }
}

static void map_parse_tilesets(map_t *self, json_value *tilesets, unsigned i)
{
   unsigned j, k;

   for(j = 0; j < tilesets->u.array.length; j++) {

      json_value *tileset = tilesets->u.array.values[j];

      for(k = 0; k < tileset->u.object.length; k++)
      {
         printf("ola %d\n", self->tileset_callback != NULL);
         if (!strcmp(tileset->u.object.values[k].name, "image") &&
               self->tileset_callback)
            self->tileset_callback(
                  tileset->u.object.values[i].value->u.string.ptr, j);
      }
   }
}

static void map_parse_objects(map_t *self, json_value *objects)
{
   unsigned l, m;

   for(l = 0; l < objects->u.array.length; l++) {

      json_value *obj = objects->u.array.values[l];
      int ox = 0;
      int oy = 0;
      int ow = 0;
      int oh = 0;

      for(m = 0; m < obj->u.object.length; m++)
      {
         char name[256];
         json_value *value;

         strlcpy(name, obj->u.object.values[m].name, sizeof(name));
         value = obj->u.object.values[m].value;

         if (!strcmp(name, "x"))
            ox = value->u.integer;

         if (!strcmp(name, "y"))
            oy = value->u.integer;

         if (!strcmp(name, "width"))
            ow = value->u.integer;

         if (!strcmp(name, "height"))
            oh = value->u.integer;
      }

      ground_new(ox, oy, ow, oh);
   }
}

static void map_parse_layers(map_t *self, json_value *layers)
{
   unsigned j, k;

   for(j = 0; j < layers->u.array.length; j++) {

      json_value *layer = layers->u.array.values[j];

      for(k = 0; k < layer->u.object.length; k++) {

         char name[256];
         json_value *value;

         value = layer->u.object.values[k].value;
         strlcpy(name, layer->u.object.values[k].name, sizeof(name));

         if (!strcmp(name, "objects"))
            map_parse_objects(self, value);
      }
   }
}

static void map_parse(map_t *self)
{
   unsigned i;

   for(i = 0; i < self->data.u.object.length; i++) {

      char name[256];
      json_value *value;

      value = self->data.u.object.values[i].value;
      strlcpy(name, self->data.u.object.values[i].name, sizeof(name));

      if (!strcmp(name, "width"))
         self->width = value->u.integer;

      if (!strcmp(name, "height"))
         self->height = value->u.integer;

      if (!strcmp(name, "tilewidth"))
         self->tilewidth = value->u.integer;

      if (!strcmp(name, "tileheight"))
         self->tileheight = value->u.integer;

      if (!strcmp(name, "tilesets"))
         map_parse_tilesets(self, value, i);

      if (!strcmp(name, "layers"))
         map_parse_layers(self, value);
   }
}

map_t* map_new(char *path, void (*tileset_callback)(char *, unsigned))
{
   FILE *fp;
   long lSize;
   char *json_buf;

   fp = fopen(path, "rb");

   fseek(fp , 0L , SEEK_END);
   lSize = ftell( fp );
   rewind(fp);

   json_buf = calloc( 1, lSize+1 );
   fread(json_buf , lSize, 1 , fp);
   fclose(fp);

   map_t *self = NULL;
   self = (map_t*)calloc(1, sizeof(map_t));

   self->data = * json_parse(json_buf, strlen(json_buf));

   free(json_buf);

   self->tileset_callback = tileset_callback;

   map_parse(self);

   return self;
}
