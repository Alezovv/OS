#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/Area.h"
#include "../include/Pi.h"

void print_help()
{
    printf("Доступные команды:\n");
    printf("0 - переключить реализацию (только для динамической версии)\n");
    printf("1 <итерации> - вычислить pi методом Лейбница\n");
    printf("2 <итерации> - вычислить pi методом Валлиса\n");
    printf("3 <a> <b> - вычислить площадь прямоугольника\n");
    printf("4 <a> <b> - вычислить площадь треугольника\n");
    printf("help - показать эту справку\n");
    printf("exit - выйти\n");
}

void handle_pi_leibniz(int iterations)
{
    if (iterations <= 0)
    {
        printf("Ошибка: количество итераций должно быть положительным\n");
        return;
    }
    double result = calculate_pi_leibniz(iterations);
    printf("pi (Лейбниц, %d итераций) = %.10f\n", iterations, result);
}

void handle_pi_wallis(int iterations)
{
    if (iterations <= 0)
    {
        printf("Ошибка: количество итераций должно быть положительным\n");
        return;
    }
    double result = calculate_pi_wallis(iterations);
    printf("pi (Валлис, %d итераций) = %.10f\n", iterations, result);
}

void handle_rectangle_area(float a, float b)
{
    if (a <= 0 || b <= 0)
    {
        printf("Ошибка: стороны должны быть положительными\n");
        return;
    }
    float result = area(a, b, RECTANGLE);
    printf("Площадь прямоугольника (%.2f x %.2f) = %.2f\n", a, b, result);
}

void handle_triangle_area(float a, float b)
{
    if (a <= 0 || b <= 0)
    {
        printf("Ошибка: стороны должны быть положительными\n");
        return;
    }
    float result = area(a, b, TRIANGLE);
    printf("Площадь треугольника (%.2f x %.2f) / 2 = %.2f\n", a, b, result);
}

int main()
{
    char input[256];
    char command[10];

    printf("=== Программа №1 - Статическое связывание ===\n");
    printf("Библиотеки загружены на этапе компиляции\n\n");
    print_help();

    while (1)
    {
        printf("\n> ");

        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            break;
        }

        input[strcspn(input, "\n")] = 0;

        if (strlen(input) == 0)
        {
            continue;
        }

        int args_parsed = sscanf(input, "%9s", command);

        if (args_parsed != 1)
        {
            continue;
        }

        if (strcmp(command, "exit") == 0)
        {
            printf("Выход...\n");
            break;
        }
        else if (strcmp(command, "help") == 0)
        {
            print_help();
        }
        else if (strcmp(command, "0") == 0)
        {
            printf("В статической версии переключение реализаций недоступно\n");
            printf("Используйте динамическую версию программы\n");
        }
        else if (strcmp(command, "1") == 0)
        {
            int iterations;
            if (sscanf(input, "%*s %d", &iterations) == 1)
            {
                handle_pi_leibniz(iterations);
            }
            else
            {
                printf("Ошибка: укажите количество итераций\n");
                printf("Использование: 1 <итерации>\n");
            }
        }
        else if (strcmp(command, "2") == 0)
        {
            int iterations;
            if (sscanf(input, "%*s %d", &iterations) == 1)
            {
                handle_pi_wallis(iterations);
            }
            else
            {
                printf("Ошибка: укажите количество итераций\n");
                printf("Использование: 2 <итерации>\n");
            }
        }
        else if (strcmp(command, "3") == 0)
        {
            float a, b;
            if (sscanf(input, "%*s %f %f", &a, &b) == 2)
            {
                handle_rectangle_area(a, b);
            }
            else
            {
                printf("Ошибка: укажите две стороны\n");
                printf("Использование: 3 <a> <b>\n");
            }
        }
        else if (strcmp(command, "4") == 0)
        {
            float a, b;
            if (sscanf(input, "%*s %f %f", &a, &b) == 2)
            {
                handle_triangle_area(a, b);
            }
            else
            {
                printf("Ошибка: укажите две стороны\n");
                printf("Использование: 4 <a> <b>\n");
            }
        }
        else
        {
            printf("Неизвестная команда: %s\n", command);
            printf("Введите 'help' для справки\n");
        }
    }

    return 0;
}