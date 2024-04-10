#pragma once
#include <SDL.h>
#include <cassert>

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 640;

void draw(SDL_Surface *s, float a, float b, float x_move, float y_move, float alpha);

void affine_transform(float *x, float *y, float x_move, float y_move, float alpha);
