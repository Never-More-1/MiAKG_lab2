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

void clear_surface(SDL_Surface *s)
{
  for (int x = 1; x < SCREEN_WIDTH; x++)
    for (int y = 1; y < SCREEN_HEIGHT; y++)
      put_pixel32(s, x, y, RGB32(0, 0, 0));
}

Uint32 get_pixel32(SDL_Surface *surface, int x, int y)
{
  assert(NULL != surface);
  assert(x < SCREEN_WIDTH);
  assert(y < SCREEN_HEIGHT);

  Uint32 *pixels = (Uint32 *)surface->pixels;
  return pixels[(y * surface->w) + x];
}

void affine_transform(float *x, float *y, float x_move, float y_move, float alpha)
{
  float rotate_x = (*x) * cos(alpha) + (*y) * sin(alpha);
  float rotate_y = -(*x) * sin(alpha) + (*y) * cos(alpha);
  *x = rotate_x;
  *y = rotate_y;
  *x += x_move;
  *y += y_move;
}

void draw(SDL_Surface *s, float a, float b, float x_move, float y_move, float alpha)
{
  glm::vec4 Position = glm::vec4(glm::vec3(0.0f), 1.0f);
  glm::mat4 Model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f));
  glm::vec4 Transformed = Model * Position;
  // Оси координат
  for (int i = 0; i < SCREEN_WIDTH; i++)
    put_pixel32(s, i, SCREEN_HEIGHT / 2, RGB32(255, 255, 255));

  for (int j = 0; j < SCREEN_HEIGHT; j++)
    put_pixel32(s, SCREEN_WIDTH / 2, j, RGB32(255, 255, 255));
// Окружность
for (float phi = 0; phi < 2 * M_PI; phi += 0.01) {
  float x = SCREEN_WIDTH / 2 + b * cos(phi); 
  float y = SCREEN_HEIGHT / 2 + b * sin(phi); 
  float center_x = x - SCREEN_WIDTH / 2;
  float center_y = y - SCREEN_HEIGHT / 2; 
  float rotate_x = center_x * cos(alpha) - center_y * sin(alpha) + SCREEN_WIDTH / 2;
  float rotate_y = center_x * sin(alpha) + center_y * cos(alpha) + SCREEN_HEIGHT / 2;
  rotate_x += x_move;
  rotate_y += y_move;
  put_pixel32(s, rotate_x, rotate_y, RGB32(255, 255, 0));
}
  // Эпициклоида
  for (float phi = -50; phi < 50; phi += 0.01) {
    float x = (a + b) * cos(phi) - a * cos(((a + b) * phi) / a);
    float y = (a + b) * sin(phi) - a * sin(((a + b) * phi) / a);
    int pointX = round(x);
    pointX += SCREEN_WIDTH / 2 + x_move;
    int pointY = round(y);
    pointY += SCREEN_HEIGHT / 2 + y_move;
    float rotate_x = x * cos(alpha) + y * sin(alpha);
    float rotate_y = -x * sin(alpha) + y * cos(alpha);
    x = rotate_x;
    y = rotate_y;
    x += x_move;
    y += y_move;
    put_pixel32(s, x + SCREEN_WIDTH / 2, y + SCREEN_HEIGHT / 2, RGB32(255, 0, 255));
  }
}
