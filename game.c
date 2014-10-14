#include "game.h"

unsigned num_entities = 0;
entity_t** entities = NULL;

void load_game()
{
   map_test = map_new("test.json");

   ninja_new();
   obake_new();

   sfx_jump = sfx_new("jump.wav", false);
   sfx_new("verger.wav", true);
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

   map_draw_layer(map_test, 0);

   unsigned i;
   for(i = 0; i < num_entities; i++)
      entities[i]->draw(entities[i]);

   map_draw_layer(map_test, 1);
}