#include "game.h"

unsigned num_entities = 0;
entity_t** entities = NULL;
surface_t* surfaces = NULL;

static void tileset_callback(char *name, unsigned j)
{
   char filename[1024];

   strlcpy(filename, "/usr/share/obake/", sizeof(filename));
   strlcat(filename, name, sizeof(filename));
   strlcat(filename, ".png", sizeof(filename));

   surfaces[j] = surface_new(filename);
}

static void object_callback(char *type, int x, int y, int w, int h)
{
   if (!strcmp(type, "ground"))
      ground_new(x, y, w, h);
}

void load_game()
{
   surfaces = (surface_t*)calloc(16, sizeof(surface_t));

   map_test = map_new("/usr/share/obake/test.json",
         &tileset_callback, &object_callback);

   ninja_new();
   obake_new();

   sfx_jump = sfx_new("/usr/share/obake/jump.wav", false);
   sfx_step = sfx_new("/usr/share/obake/step.wav", false);

   sfx_new("/usr/share/obake/verger.wav", true);
}

void update_game()
{
   unsigned i;
   for(i = 0; i < num_entities; i++)
      entities[i]->update(entities[i]);

   detect_collisions();
}

void draw_game()
{
   draw_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0xff000000);

   map_draw_layer(map_test, surfaces, 0);

   unsigned i;
   for(i = 0; i < num_entities; i++)
      entities[i]->draw(entities[i]);

   map_draw_layer(map_test, surfaces, 1);
}