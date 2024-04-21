#include "draw.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#define RGB32(r, g, b) static_cast<uint32_t>((((static_cast<uint32_t>(b) << 8) | g) << 8) | r)

void put_pixel32(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
  if((x > 0 && x < SCREEN_WIDTH) && (y > 0 && y < SCREEN_HEIGHT)){
  Uint32 *pixels = (Uint32 *)surface->pixels;
  pixels[(y * surface->w) + x] = pixel;
}
}

Uint32 get_pixel32(SDL_Surface *surface, int x, int y)
{
  assert(NULL != surface);
  assert(x < SCREEN_WIDTH);
  assert(y < SCREEN_HEIGHT);

  Uint32 *pixels = (Uint32 *)surface->pixels;
  return pixels[(y * surface->w) + x];
}

void draw_axis(SDL_Surface *s)
{
  for (int i = 0; i < SCREEN_WIDTH; i++)
    put_pixel32(s, i, SCREEN_HEIGHT / 2, RGB32(105, 105, 105));

  for (int j = 0; j < SCREEN_HEIGHT; j++)
    put_pixel32(s, SCREEN_WIDTH / 2, j, RGB32(105, 105, 105));
}

void draw_square(SDL_Surface *s)
{
  // Размер стороны квадрата
  int size = 250;

  // Центр экрана
  int centerX = SCREEN_WIDTH / 2;
  int centerY = SCREEN_HEIGHT / 2;

  // Вычисление координат вершин квадрата
  int halfSize = size / 2;
  int x1 = centerX - halfSize; // левая граница квадрата
  int x2 = centerX + halfSize; // правая граница квадрата
  int y1 = centerY - halfSize; // верхняя граница квадрата
  int y2 = centerY + halfSize; // нижняя граница квадрата

  // Отрисовка верхней грани квадрата
  for (int i = x1; i < x2; i++)
    put_pixel32(s, i, y1, RGB32(0, 0, 255));

  // Отрисовка нижней грани квадрата
  for (int i = x1; i < x2; i++)
    put_pixel32(s, i, y2 - 1, RGB32(0, 0, 255));

  // Отрисовка левой грани квадрата
  for (int j = y1; j < y2; j++)
    put_pixel32(s, x1, j, RGB32(0, 0, 255));

  // Отрисовка правой грани квадрата
  for (int j = y1; j < y2; j++)
    put_pixel32(s, x2 - 1, j, RGB32(0, 0, 255));
}


void convert_coordinates(float q, struct Point *points, int num_vertex)
{
  struct Point *new_points = (struct Point*)malloc(num_vertex * sizeof(struct Point));

  for(int i = 0; i < num_vertex; i++)
  {
    new_points[i].x = (1 - q) * points[i % num_vertex].x + q * points[(i + 1) % num_vertex].x; 
    new_points[i].y = (1 - q) * points[i % num_vertex].y + q * points[(i + 1) % num_vertex].y; 
  }

  for(int i = 0; i < num_vertex; i++)
  {
    points[i].x = new_points[i].x;
    points[i].y = new_points[i].y;
  }
}

void affine_transform(struct Point* points, int num_vertex, float u, float d, float alpha)
{
  for(int i = 0; i < num_vertex; i++)
  {
    float rotate_x = points[i].x * cos(alpha) + points[i].y * sin(alpha);
    float rotate_y = -points[i].x * sin(alpha) + points[i].y * cos(alpha);
    points[i].x = rotate_x;
    points[i].y = rotate_y;
    points[i].x += u;
    points[i].y += d;
  }
}

void draw_figure(SDL_Surface *s, struct Point* points, int num_vertex)
{
  for(int i = 0; i < num_vertex; i++)
  {
    for (float t = 0; t <= 1; t += 0.001)
    {
      float x = (points[(i + 1) % num_vertex].x - points[i % num_vertex].x) * t + points[i % num_vertex].x;
      float y = (points[(i + 1) % num_vertex].y - points[i % num_vertex].y) * t + points[i % num_vertex].y;
      put_pixel32(s, x + SCREEN_HEIGHT / 2, SCREEN_WIDTH / 2 + y, RGB32(0, 0, 0));
    }
  }
}

void cohenSutherlandPolygon(struct Point *points, int num_vertex, int xMin, int xMax, int yMin, int yMax)
{
    // Определим коды областей для границ красного квадрата
    int INSIDE = 0; // 0000
    int LEFT = 1;   // 0001
    int RIGHT = 2;  // 0010
    int BOTTOM = 4; // 0100
    int TOP = 8;    // 1000

    // Функция определения кода области для каждой вершины многоугольника
    auto computeCode = [&](float x, float y) -> int {
        int code = INSIDE; // Изначально считаем точку внутри окна

        if (x < xMin) // слева от окна
            code |= LEFT;
        else if (x > xMax) // справа от окна
            code |= RIGHT;
        if (y < yMin) // ниже окна
            code |= BOTTOM;
        else if (y > yMax) // выше окна
            code |= TOP;

        return code;
    };

    // Применяем алгоритм отсечения Коэна-Сазерленда для каждой вершины
    for (int i = 0; i < num_vertex; ++i)
    {
        int code = computeCode(points[i].x, points[i].y);

        // Если вершина полностью внутри красного квадрата (code == INSIDE), оставляем без изменений
        // Если вершина полностью за пределами красного квадрата (code != INSIDE && (code & codePrev) != 0), отсекаем
        // Иначе, вершина частично внутри - обрезаем до ближайшей границы

        if (code != INSIDE)
        {
            bool done = false;
            while (!done)
            {
                if ((code & LEFT) && points[i].x < xMin)
                {
                    points[i].y += (xMin - points[i].x) * (points[(i + 1) % num_vertex].y - points[i].y) / (points[(i + 1) % num_vertex].x - points[i].x);
                    points[i].x = xMin;
                }
                else if ((code & RIGHT) && points[i].x > xMax)
                {
                    points[i].y += (xMax - points[i].x) * (points[(i + 1) % num_vertex].y - points[i].y) / (points[(i + 1) % num_vertex].x - points[i].x);
                    points[i].x = xMax;
                }
                else if ((code & BOTTOM) && points[i].y < yMin)
                {
                    points[i].x += (yMin - points[i].y) * (points[(i + 1) % num_vertex].x - points[i].x) / (points[(i + 1) % num_vertex].y - points[i].y);
                    points[i].y = yMin;
                }
                else if ((code & TOP) && points[i].y > yMax)
                {
                    points[i].x += (yMax - points[i].y) * (points[(i + 1) % num_vertex].x - points[i].x) / (points[(i + 1) % num_vertex].y - points[i].y);
                    points[i].y = yMax;
                }

                // Пересчитываем код области для обрезанной вершины
                code = computeCode(points[i].x, points[i].y);

                // Проверяем, завершена ли обработка для текущей вершины
                if (code == INSIDE || (code & (LEFT | RIGHT | BOTTOM | TOP)) == 0)
                    done = true;
            }
        }
    }
}

void draw_nested_figures(SDL_Surface *s, float mouse_x, float mouse_y, int num_vertex, float r, float u, float d, int n, float alpha)
{
  draw_axis(s);

  draw_square(s);

  struct Point *points = (struct Point*)malloc(num_vertex * sizeof(struct Point));

  for(int i = 0; i < num_vertex; i++)
  {
    points[i].x = r * cos(2 * M_PI * i / num_vertex);
    points[i].y = r * sin(2 * M_PI * i / num_vertex);
  }

  affine_transform(points, num_vertex, u, d, alpha);

  draw_figure(s, points, num_vertex);

  for (int i = 1; i <= n; i++)
  {
    float q = tan((i - 1) * M_PI / (4 * (n - 1))) / (tan((i - 1) * M_PI / (4 * (n - 1))) + 1);
    convert_coordinates(q, points, num_vertex);

    draw_figure(s, points, num_vertex);
  }
}