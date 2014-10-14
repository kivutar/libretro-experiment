#include "game.h"

static anim_t anim_ninja_stand_left;
static anim_t anim_ninja_stand_right;
static anim_t anim_ninja_run_left;
static anim_t anim_ninja_run_right;
static anim_t anim_ninja_jump_left;
static anim_t anim_ninja_jump_right;
static anim_t anim_ninja_fall_left;
static anim_t anim_ninja_fall_right;
static anim_t anim_ninja_duck_left;
static anim_t anim_ninja_duck_right;
static float vy = 0;
static float ay = 0.2;
static float vx = 0;
static float ax = 0.1;
static anim_t* old_anim;

static bool on_the_ground(entity_t *self)
{
   return solid_at(self->x+8, self->y+32);
}

static void update(entity_t *self)
{
   // apply gravity if not on the ground
   if (!on_the_ground(self))
   {
      vy += ay;
      self->y += vy;
   }

   // jumping
   if (ks.b && on_the_ground(self))
   {
      self->y -= 1;
      vy = -4;
      sfx_play(sfx_jump);
   }

   // moving
   if (ks.left) {
      vx -= ax;
      if (vx < -3)
         vx = -3;
      self->d = false;
   }
   if (ks.right) {
      vx += ax;
      if (vx > 3)
         vx = 3;
      self->d = true;
   }

   // apply speed
   self->x += vx;

   // decelerating
   if (!(ks.right && vx > 0) && !(ks.left && vx < 0) && on_the_ground(self))
   {
      if (vx > 0)
      { 
         vx -= 0.5;
         if (vx < 0)
            vx = 0;
      }
      else if (vx < 0)
      { 
         vx += 0.5; 
         if (vx > 0)
            vx = 0;
      }
   }

   // run animation
   anim_ninja_run_left.p = 9 - abs(vx);
   anim_ninja_run_right.p = 9 - abs(vx);
   if (vx == 0 && on_the_ground(self))
      self->anim = self->d ? &anim_ninja_stand_right : &anim_ninja_stand_left;
   if (vx != 0 && on_the_ground(self))
      self->anim = self->d ? &anim_ninja_run_right : &anim_ninja_run_left;

   // jump animation
   if (!on_the_ground(self) && vy < 0)
      self->anim = self->d ? &anim_ninja_jump_right : &anim_ninja_jump_left;
   if (!on_the_ground(self) && vy > 0)
      self->anim = self->d ? &anim_ninja_fall_right : &anim_ninja_fall_left;

   // duck animation
   if (vx == 0 && on_the_ground(self) && ks.down)
      self->anim = self->d ? &anim_ninja_duck_right : &anim_ninja_duck_left;

   if (self->anim != old_anim)
      self->anim->t = 0;

   old_anim = self->anim;

   if (self->anim == &anim_ninja_run_left
         || self->anim == &anim_ninja_run_right)
      if (self->anim->i == 5 || self->anim->i == 11)
         if (!sfx_is_playing(sfx_step))
            sfx_play(sfx_step);

   // camera
   camera.x = - self->x + SCREEN_WIDTH/2 - self->w/2;
   if (camera.x > 0)
      camera.x = 0;
   if (camera.x < -1280 + SCREEN_WIDTH)
      camera.x = -1280 + SCREEN_WIDTH;
}

static void draw(entity_t *self)
{
   draw_anim(
      self->x - 16,
      self->y - 16,
      self->anim);
}

static void on_collide(entity_t *self, entity_t *other, int dx, int dy)
{
   if (abs(dy) < abs(dx) && dy < 0)
   {
      vy = 0;
      self->y += dy;
      sfx_play(sfx_step);
   }

   if (abs(dx) < abs(dy) && dx < 0)
   {
      vx = 0;
      self->x += dx;
   }

   if (abs(dx) < abs(dy) && dx > 0)
   {
      vx = 0;
      self->x += dx;
   }

   if (abs(dx) < abs(dy) && dx < 0)
   {
      vx = 0;
      self->x += dx;
   }
}

entity_t* ninja_new()
{
   anim_ninja_stand_left.surface = surface_new("/usr/share/obake/ninja_stand_left.png");
   anim_ninja_stand_left.t = 0;
   anim_ninja_stand_left.p = 240;
   anim_ninja_stand_left.w = 48;
   anim_ninja_stand_left.h = 48;

   anim_ninja_stand_right.surface = surface_new("/usr/share/obake/ninja_stand_right.png");
   anim_ninja_stand_right.t = 0;
   anim_ninja_stand_right.p = 240;
   anim_ninja_stand_right.w = 48;
   anim_ninja_stand_right.h = 48;

   anim_ninja_run_left.surface = surface_new("/usr/share/obake/ninja_run_left.png");
   anim_ninja_run_left.t = 0;
   anim_ninja_run_left.p = 1;
   anim_ninja_run_left.w = 48;
   anim_ninja_run_left.h = 48;

   anim_ninja_run_right.surface = surface_new("/usr/share/obake/ninja_run_right.png");
   anim_ninja_run_right.t = 0;
   anim_ninja_run_right.p = 3;
   anim_ninja_run_right.w = 48;
   anim_ninja_run_right.h = 48;

   anim_ninja_jump_left.surface = surface_new("/usr/share/obake/ninja_jump_left.png");
   anim_ninja_jump_left.t = 0;
   anim_ninja_jump_left.p = 1;
   anim_ninja_jump_left.w = 48;
   anim_ninja_jump_left.h = 48;

   anim_ninja_jump_right.surface = surface_new("/usr/share/obake/ninja_jump_right.png");
   anim_ninja_jump_right.t = 0;
   anim_ninja_jump_right.p = 1;
   anim_ninja_jump_right.w = 48;
   anim_ninja_jump_right.h = 48;

   anim_ninja_fall_left.surface = surface_new("/usr/share/obake/ninja_fall_left.png");
   anim_ninja_fall_left.t = 0;
   anim_ninja_fall_left.p = 1;
   anim_ninja_fall_left.w = 48;
   anim_ninja_fall_left.h = 48;

   anim_ninja_fall_right.surface = surface_new("/usr/share/obake/ninja_fall_right.png");
   anim_ninja_fall_right.t = 0;
   anim_ninja_fall_right.p = 1;
   anim_ninja_fall_right.w = 48;
   anim_ninja_fall_right.h = 48;

   anim_ninja_duck_left.surface = surface_new("/usr/share/obake/ninja_duck_left.png");
   anim_ninja_duck_left.t = 0;
   anim_ninja_duck_left.p = 1;
   anim_ninja_duck_left.w = 48;
   anim_ninja_duck_left.h = 48;

   anim_ninja_duck_right.surface = surface_new("/usr/share/obake/ninja_duck_right.png");
   anim_ninja_duck_right.t = 0;
   anim_ninja_duck_right.p = 1;
   anim_ninja_duck_right.w = 48;
   anim_ninja_duck_right.h = 48;

   entity_t *self = NULL;
   self = (entity_t*)realloc(self, sizeof(entity_t));

   self->w = 16;
   self->h = 32;
   self->x = SCREEN_WIDTH/2  - self->w/2;
   self->y = SCREEN_HEIGHT/2 - self->h/2;
   self->d = true;
   self->anim = &anim_ninja_stand_left;
   self->update = &update;
   self->draw = &draw;
   self->on_collide = &on_collide;

   num_entities++;
   entities = (entity_t**)realloc(entities, num_entities * sizeof(entity_t));
   entities[num_entities-1] = self;

   return self;
}
