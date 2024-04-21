#pragma once
#include <SDL.h>
#include <cassert>

const int INSIDE = 0;// 0000
const int LEFT = 1;// 0001
const int RIGHT = 2;// 0010
const int BOTTOM = 4;// 0100
const int TOP = 8;// 1000

// Определение окна прямоугольной области
const double X_MIN = 200;
const double X_MAX = 500;
const double Y_MIN = 100;
const double Y_MAX = 400;


//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 640;

struct Point
{
    float x;
    float y;
};

void affine_transform(struct Point *points, int num_vertex, float u, float d, float alpha);
void draw_nested_figures(SDL_Surface *s, float mouse_x, float mouse_y, int num_vertex, float r, float u, float d, int n, float alpha);
void draw_figure(SDL_Surface *s, struct Point* points, int num_vertex);
void convert_coordinates(float q, struct Point* points, int num_vertex);