#include <stdio.h>
#include <math.h>
#include <time.h>

#define BUILD_DLL
#include "../include/Pi.h"

EXPORT double calculate_pi_leibniz(int iterations)
{
    double pi_quarter = 0.0;
    int sign = 1;

    if (iterations <= 0)
    {
        printf("Ошибка: количество итераций должно быть положительным\n");
        return -1.0;
    }

    for (int i = 0; i < iterations; i++)
    {
        pi_quarter += sign * 1.0 / (2 * i + 1);
        sign = -sign;
    }

    return 4 * pi_quarter;
}

EXPORT double calculate_pi_wallis(int iterations)
{
    double pi_half = 1.0;

    if (iterations <= 0)
    {
        printf("Ошибка: количество итераций должно быть положительным\n");
        return -1.0;
    }

    for (int i = 1; i <= iterations; i++)
    {
        double numerator = 2.0 * i;
        pi_half *= (numerator / (numerator - 1)) * (numerator / (numerator + 1));
    }

    return 2 * pi_half;
}