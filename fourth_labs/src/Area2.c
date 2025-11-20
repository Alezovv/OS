#include <stdio.h>
#include <math.h>

#define BUILD_DLL
#include "../include/Area.h"

static float rectangle_area_alt(float a, float b)
{
    if (a <= 0 || b <= 0)
    {
        printf("Ошибка: стороны должны быть положительными\n");
        return -1.0f;
    }

    if (a == b)
    {
        printf("(Это квадрат!)\n");
    }

    return a * b;
}

static float triangle_area_alt(float a, float b)
{
    if (a <= 0 || b <= 0)
    {
        printf("Ошибка: стороны должны быть положительными\n");
        return -1.0f;
    }

    if (a == b)
    {
        printf("(Это равнобедренный прямоугольный треугольник!)\n");
    }

    return (a * b) / 2.0f;
}

EXPORT float area(float a, float b, int figure_type)
{
    printf("[Реализация 2] ");

    switch (figure_type)
    {
    case RECTANGLE:
        return rectangle_area_alt(a, b);
    case TRIANGLE:
        return triangle_area_alt(a, b);
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
        return "прямоугольника (реализация 2)";
    case TRIANGLE:
        return "треугольника (реализация 2)";
    default:
        return "неизвестной фигуры";
    }
}