#include <stdio.h>

#define BUILD_DLL
#include "../include/Area.h"

static float rectangle_area(float a, float b)
{
    if (a <= 0 || b <= 0)
    {
        printf("Ошибка: стороны должны быть положительными\n");
        return -1.0f;
    }
    return a * b;
}

static float triangle_area(float a, float b)
{
    if (a <= 0 || b <= 0)
    {
        printf("Ошибка: стороны должны быть положительными\n");
        return -1.0f;
    }
    return (a * b) / 2.0f;
}

EXPORT float area(float a, float b, int figure_type)
{
    switch (figure_type)
    {
    case RECTANGLE:
        return rectangle_area(a, b);
    case TRIANGLE:
        return triangle_area(a, b);
    default:
        printf("Ошибка: неизвестный тип фигуры (%d)\n", figure_type);
        return -1.0f;
    }
}

EXPORT const char *get_figure_name(int figure_type)
{
    switch (figure_type)
    {
    case RECTANGLE:
        return "прямоугольника";
    case TRIANGLE:
        return "треугольника";
    default:
        return "неизвестной фигуры";
    }
}