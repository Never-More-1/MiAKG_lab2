#include "draw.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#define RGB32(r, g, b) static_cast<uint32_t>((((static_cast<uint32_t>(r) << 8) | g) << 8) | b)

void put_pixel32(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
  assert(NULL != surface);
  assert(x < SCREEN_WIDTH);
  assert(y < SCREEN_HEIGHT);

  Uint32 *pixels = (Uint32 *)surface->pixels;
  pixels[(y * surface->w) + x] = pixel;
}

void clear_surface(SDL_Surface * s) {
  for (int x = 1; x < SCREEN_WIDTH; x++)
    for(int y = 1; y < SCREEN_HEIGHT; y++)
      put_pixel32(s,x,y,RGB32(0,0,0));
}

Uint32 get_pixel32(SDL_Surface *surface, int x, int y)
{
  assert(NULL != surface);
  assert(x < SCREEN_WIDTH);
  assert(y < SCREEN_HEIGHT);

  Uint32 *pixels = (Uint32 *)surface->pixels;
  return pixels[(y * surface->w) + x];
}

void draw(SDL_Surface *s, float a = 1, float x_move = 1, float y_move = 1)
{
  glm::vec4 Position = glm::vec4(glm::vec3(0.0f), 1.0f);
  glm::mat4 Model = glm::translate(    glm::mat4(1.0f), glm::vec3(1.0f));
  glm::vec4 Transformed = Model * Position;

  float t, x, y;
  clear_surface(s);
// координатная ось
for (int i = 0; i < SCREEN_WIDTH; i++){
  put_pixel32(s, i, SCREEN_HEIGHT/2, RGB32(255, 255, 255));
}

for (int j = 0; j < SCREEN_HEIGHT; j++){
  put_pixel32(s, SCREEN_WIDTH/2, j, RGB32(255, 255, 255));
}
// основная функция
  for (t = 0; t < 400; t+= 0.001) {
    x = a * cos(t) * (1 + cos(t)) + 200 + x_move;
    y = a * sin(t) * (1 + cos(t)) + 200 + y_move; 
    put_pixel32(s, x + SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - y, RGB32(255, 105, 108));
  }
// невидимая окружность
  for (float g = 0; g < 2 * M_PI; g += 0.1) { 
    x = (a / 2) * cos(g) + 200 + a/2 + x_move;
    y = (a / 2) * sin(g) + 200 + y_move; 
    put_pixel32(s, x + SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - y, RGB32(127, 255, 212));
  } 
// точки на окружности
  for (float i = 0; i < 2 * M_PI; i += 0.1) { 
    x = 2.5 * cos(i) + 200 + a/2 + x_move;
    y = 2.5 * sin(i) + 200 + a/2 + y_move; 
    put_pixel32(s, x + SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - y, RGB32(0, 255, 0));
  } 

  for (float i = 0; i < 2 * M_PI; i += 0.1) { 
    x = 2.5 * cos(i) + 200 + 2*a + x_move;
    y = 2.5 * sin(i) + 200 + y_move; 
    put_pixel32(s, x + SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - y, RGB32(0, 255, 0));
  } 
// точки на функции
  for (float i = 0; i < 2 * M_PI; i += 0.1) { 
    x = 2.5 * cos(i) + 200 + (3*a)/4 + x_move;
    y = 2.5 * sin(i) + 200 + sqrt(3)*(3*a)/4 + y_move; 
    put_pixel32(s, x + SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - y, RGB32(0, 255, 0));
  }

   for (float i = 0; i < 2 * M_PI; i += 0.1) { 
    x = 2.5 * cos(i) + 200 + (3*a)/4 + x_move;
    y = 2.5 * sin(i) + 200 + sqrt(3)*(-1)*(3*a)/4 + y_move; 
    put_pixel32(s, x + SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - y, RGB32(0, 255, 0));
  }

  for (float i = 0; i < 2 * M_PI; i += 0.1) { 
    x = 2.5 * cos(i) + 200 + x_move;
    y = 2.5 * sin(i) + 200 + y_move; 
    put_pixel32(s, x + SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - y, RGB32(0, 255, 0));
  } 
}
