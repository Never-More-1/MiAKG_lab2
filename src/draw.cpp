#include "draw.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#define RGB32(r, g, b) static_cast<uint32_t>((((static_cast<uint32_t>(r) << 8) | g) << 8) | b)

void put_pixel32(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
  // assert(NULL != surface);
  // assert(x < SCREEN_WIDTH);
  // assert(y < SCREEN_HEIGHT);

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

int my_put_pixel(int x, int y, double alpha)
{
  x = (int)(x * cos(alpha) - y * sin(alpha));
  y = (int)(x * sin(alpha) + y * cos(alpha));
  return x, y;
}

void draw(SDL_Surface *s, float a = 1, float x_move = 1, float y_move = 1, double alpha = 0.0)
{
  glm::vec4 Position = glm::vec4(glm::vec3(0.0f), 1.0f);
  glm::mat4 Model = glm::translate(    glm::mat4(1.0f), glm::vec3(1.0f));
  glm::vec4 Transformed = Model * Position;

  float t, x, y;
  clear_surface(s);

  std::cout << x_move << 'x' << std::endl;
  std::cout << y_move << 'y' << std::endl;

// координатная ось
for (int i = 0; i < SCREEN_WIDTH; i++){
  put_pixel32(s, i, SCREEN_HEIGHT/2, RGB32(255, 255, 255));
}

for (int j = 0; j < SCREEN_HEIGHT; j++){
  put_pixel32(s, SCREEN_WIDTH/2, j, RGB32(255, 255, 255));
}
// основная функция
  for (float t = 0; t < 2 * M_PI; t += 0.001) {
    float x = a * cos(t) * (1 + cos(t));
    float y = a * sin(t) * (1 + cos(t));
    float rotate_x = x * cos(alpha) + y * sin(alpha);
    float rotate_y = -x * sin(alpha) + y * cos(alpha);
    x = rotate_x;
    y = rotate_y;
    //        new_x *= scale;
    //        new_y *= scale;
    x += x_move;
    y += y_move;
    put_pixel32(s, x + SCREEN_WIDTH / 2, y + SCREEN_HEIGHT / 2, RGB32(255, 105, 108));
  }
// невидимая окружность
  for (float g = 0; g < 2 * M_PI; g += 0.1) { 
    x = (a / 2) * cos(g) + a/2;
    y = (a / 2) * sin(g);
    float rotate_x = x * cos(alpha) + y * sin(alpha);
    float rotate_y = -x * sin(alpha) + y * cos(alpha);
    x = rotate_x;
    y = rotate_y;
    //        new_x *= scale;
    //        new_y *= scale;
    x += x_move;
    y += y_move;
    put_pixel32(s, x + SCREEN_WIDTH / 2, y + SCREEN_HEIGHT / 2, RGB32(255,0, 255));
  } 

// точки на окружности
  for (float i = 0; i < 2 * M_PI; i += 0.1) { 
    x = 2.5 * cos(i) + a/2;
    y = 2.5 * sin(i) + a/2;
    float rotate_x = x * cos(alpha) - y * sin(alpha);
    float rotate_y = x * sin(alpha) + y * cos(alpha);
    x = rotate_x;
    y = rotate_y;
    //        new_x *= scale;
    //        new_y *= scale;
    x += x_move;
    y -= y_move;
    put_pixel32(s, x + SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - y, RGB32(0, 255, 0));
  } 

  for (float i = 0; i < 2 * M_PI; i += 0.1) { 
    x = 2.5 * cos(i) + 2*a;
    y = 2.5 * sin(i);
    float rotate_x = x * cos(alpha) - y * sin(alpha);
    float rotate_y = x * sin(alpha) + y * cos(alpha);
    x = rotate_x;
    y = rotate_y;
    //        new_x *= scale;
    //        new_y *= scale;
    x += x_move;
    y -= y_move;
    put_pixel32(s, x + SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - y, RGB32(0, 255, 0));
  } 

  for (float i = 0; i < 2 * M_PI; i += 0.1) { 
    x = 2.5 * cos(i);
    y = 2.5 * sin(i);
    float rotate_x = x * cos(alpha) - y * sin(alpha);
    float rotate_y = x * sin(alpha) + y * cos(alpha);
    x = rotate_x;
    y = rotate_y;
    //        new_x *= scale;
    //        new_y *= scale;
    x += x_move;
    y -= y_move;
    put_pixel32(s, x + SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - y, RGB32(0, 255, 0));
  } 

// точки на функции
  for (float i = 0; i < 2 * M_PI; i += 0.1) { 
    x = 2.5 * cos(i) + (3*a)/4;
    y = 2.5 * sin(i) + sqrt(3)*(3*a)/4;
    float rotate_x = x * cos(alpha) - y * sin(alpha);
    float rotate_y = x * sin(alpha) + y * cos(alpha);
    x = rotate_x;
    y = rotate_y;
    //        new_x *= scale;
    //        new_y *= scale;
    x += x_move;
    y -= y_move;
    put_pixel32(s, x + SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - y, RGB32(0, 255, 0));
  }

   for (float i = 0; i < 2 * M_PI; i += 0.1) { 
    x = 2.5 * cos(i) + (3*a)/4;
    y = 2.5 * sin(i) + sqrt(3)*(-1)*(3*a)/4;
    float rotate_x = x * cos(alpha) - y * sin(alpha);
    float rotate_y = x * sin(alpha) + y * cos(alpha);
    x = rotate_x;
    y = rotate_y;
    //        new_x *= scale;
    //        new_y *= scale;
    x += x_move;
    y -= y_move;
    put_pixel32(s, x + SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - y, RGB32(0, 255, 0));
  }
}
