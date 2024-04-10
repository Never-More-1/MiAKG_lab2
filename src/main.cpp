#include "draw.h"

#include <SDL.h>
#include <stdio.h>
#include <string>
#include <cassert>

bool init();
void close();

SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;
SDL_Texture *gTexture = NULL;
SDL_Surface *loadedSurface = NULL;

bool init()
{
  bool success = true;
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
    success = false;
  } else {
    gWindow = SDL_CreateWindow("AOKG Lab 1", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == NULL) {
      printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
      success = false;
    } else {
      gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_SOFTWARE);
      if (gRenderer == NULL) {
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        success = false;
      } else {
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
      }
    }
  }
  return success;
}

void close()
{
  SDL_DestroyTexture(gTexture);
  gTexture = NULL;
  SDL_DestroyRenderer(gRenderer);
  SDL_DestroyWindow(gWindow);
  gWindow = NULL;
  gRenderer = NULL;
  SDL_Quit();
}

int main(int argc, char *argv[])
{
  if (!init()) {
    printf("Failed to initialize!\n");
  } else {
    loadedSurface = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
      0x00FF0000,// R
      0x0000FF00,// G
      0x000000FF,// B
      0x00000000);// alpha
    float x_move = 0;
    float y_move = 0;
    double alpha = 0;
    float a = 10, b = 70;
    gTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);

    if (NULL == gTexture) {
      printf("Failed to load media!\n");
    } else {
      bool quit = false;
      SDL_Event e;

      while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
          if (SDL_QUIT == e.type) {
            quit = true;
          }
          if (SDL_KEYDOWN == e.type) {
            switch (e.key.keysym.scancode) {

            // масштабирование
            case SDL_SCANCODE_KP_PLUS:
              printf("SDL_SCANCODE_KP_PLUS have been presssed\n");
              a *= 1.1;
              b *= 1.1;
              SDL_FillRect(loadedSurface, NULL, 0x00000000);
              break;
            case SDL_SCANCODE_KP_MINUS:
              printf("SDL_SCANCODE_KP_MINUS have been presssed\n");
              a /= 1.1;
              b /= 1.1;
              SDL_FillRect(loadedSurface, NULL, 0x00000000);
              break;
            // изменение значений переменных a и b
            case SDL_SCANCODE_KP_1:
              printf("SDL_SCANCODE_KP_1 have been presssed\n");
              a = 10;
              b = 70;
              SDL_FillRect(loadedSurface, NULL, 0x00000000);
              break;
            case SDL_SCANCODE_KP_2:
              printf("SDL_SCANCODE_KP_2 have been presssed\n");
              a = 30;
              b = 50;
              SDL_FillRect(loadedSurface, NULL, 0x00000000);
              break;
            case SDL_SCANCODE_KP_3:
              printf("SDL_SCANCODE_KP_3 have been presssed\n");
              a = 75;
              b = sqrt(2378);
              SDL_FillRect(loadedSurface, NULL, 0x00000000);
              break;
            // перемещение графика вверх, вниз, вправо, влево
            case SDL_SCANCODE_UP:
              printf("SDL_SCANCODE_UP have been presssed\n");
              y_move -= 10;
              SDL_FillRect(loadedSurface, NULL, 0x00000000);
              break;
            case SDL_SCANCODE_DOWN:
              printf("SDL_SCANCODE_DOWN have been presssed\n");
              y_move += 10;
              SDL_FillRect(loadedSurface, NULL, 0x00000000);
              break;
            case SDL_SCANCODE_LEFT:
              printf("SDL_SCANCODE_LEFT have been presssed\n");
              x_move -= 10;
              SDL_FillRect(loadedSurface, NULL, 0x00000000);
              break;
            case SDL_SCANCODE_RIGHT:
              printf("SDL_SCANCODE_RIGHT have been presssed\n");
              x_move += 10;
              SDL_FillRect(loadedSurface, NULL, 0x00000000);
              break;
            // сдвиг с сохранением параллелизма
            case SDL_SCANCODE_Q:
              printf("SDL_SCANCODE_Q have been presssed\n");
              alpha += 0.26;
              SDL_FillRect(loadedSurface, NULL, 0x00000000);
              break;
            case SDL_SCANCODE_E:
              printf("SDL_SCANCODE_E have been presssed\n");
              alpha -= 0.26;
              SDL_FillRect(loadedSurface, NULL, 0x00000000);
              break;
            case SDL_SCANCODE_ESCAPE:
              quit = true;
              break;
            default:
              break;
            }
          }
        }

        SDL_RenderClear(gRenderer);

        draw(loadedSurface, a, b, x_move, y_move, alpha);

        SDL_UpdateTexture(gTexture, NULL, loadedSurface->pixels, loadedSurface->pitch);
        SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);
        SDL_RenderPresent(gRenderer);
      }
    }
  }
  close();
  return 0;
}
