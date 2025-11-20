#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

typedef double (*pi_func_t)(int);
typedef float (*area_func_t)(float, float, int);
typedef const char *(*info_func_t)(int);

typedef struct
{
    void *handle;
    pi_func_t leibniz;
    pi_func_t wallis;
    area_func_t area;
    info_func_t figure_name;
    const char *name;
} Library;

Library current_lib = {0};
int current_implementation = 1;

int load_library(int impl_number)
{
    const char *lib_paths[] = {
        "./lib/libarea.so",
        "./lib/libarea2.so"};

    if (impl_number < 1 || impl_number > 2)
    {
        printf("Ошибка: доступны реализации 1 и 2\n");
        return 0;
    }

    if (current_lib.handle)
    {
        dlclose(current_lib.handle);
        current_lib.handle = NULL;
    }

    current_lib.handle = dlopen(lib_paths[impl_number - 1], RTLD_LAZY);
    if (!current_lib.handle)
    {
        printf("Ошибка загрузки библиотеки %s: %s\n",
               lib_paths[impl_number - 1], dlerror());
        return 0;
    }

    current_lib.leibniz = (pi_func_t)dlsym(current_lib.handle, "calculate_pi_leibniz");
    current_lib.wallis = (pi_func_t)dlsym(current_lib.handle, "calculate_pi_wallis");
    current_lib.area = (area_func_t)dlsym(current_lib.handle, "area");
    current_lib.figure_name = (info_func_t)dlsym(current_lib.handle, "get_figure_name");

    char *error;
    if ((error = dlerror()) != NULL)
    {
        printf("Ошибка загрузки функций: %s\n", error);
        dlclose(current_lib.handle);
        current_lib.handle = NULL;
        return 0;
    }

    current_lib.name = lib_paths[impl_number - 1];
    current_implementation = impl_number;

    printf("Загружена реализация %d: %s\n", impl_number, lib_paths[impl_number - 1]);
    return 1;
}

void print_help()
{
    printf("Доступные команды:\n");
    printf("0 - переключить реализацию (текущая: %d)\n", current_implementation);
    printf("1 <итерации> - вычислить pi методом Лейбница\n");
    printf("2 <итерации> - вычислить pi методом Валлиса\n");
    printf("3 <a> <b> - вычислить площадь прямоугольника\n");
    printf("4 <a> <b> - вычислить площадь треугольника\n");
    printf("help - показать эту справку\n");
    printf("exit - выйти\n");
}

void handle_pi_leibniz(int iterations)
{
    if (!current_lib.handle || !current_lib.leibniz)
    {
        printf("Ошибка: библиотека не загружена\n");
        return;
    }
    if (iterations <= 0)
    {
        printf("Ошибка: количество итераций должно быть положительным\n");
        return;
    }
    double result = current_lib.leibniz(iterations);
    printf("pi (Лейбниц, %d итераций) = %.10f\n", iterations, result);
}

void handle_pi_wallis(int iterations)
{
    if (!current_lib.handle || !current_lib.wallis)
    {
        printf("Ошибка: библиотека не загружена\n");
        return;
    }
    if (iterations <= 0)
    {
        printf("Ошибка: количество итераций должно быть положительным\n");
        return;
    }
    double result = current_lib.wallis(iterations);
    printf("pi (Валлис, %d итераций) = %.10f\n", iterations, result);
}

void handle_rectangle_area(float a, float b)
{
    if (!current_lib.handle || !current_lib.area || !current_lib.figure_name)
    {
        printf("Ошибка: библиотека не загружена\n");
        return;
    }
    if (a <= 0 || b <= 0)
    {
        printf("Ошибка: стороны должны быть положительными\n");
        return;
    }
    float result = current_lib.area(a, b, 1);
    const char *figure_name = current_lib.figure_name(1);
    printf("Площадь %s (%.2f x %.2f) = %.2f\n", figure_name, a, b, result);
}

void handle_triangle_area(float a, float b)
{
    if (!current_lib.handle || !current_lib.area || !current_lib.figure_name)
    {
        printf("Ошибка: библиотека не загружена\n");
        return;
    }
    if (a <= 0 || b <= 0)
    {
        printf("Ошибка: стороны должны быть положительными\n");
        return;
    }
    float result = current_lib.area(a, b, 2);
    const char *figure_name = current_lib.figure_name(2);
    printf("Площадь %s (%.2f x %.2f) / 2 = %.2f\n", figure_name, a, b, result);
}

void switch_implementation()
{
    int new_impl = (current_implementation == 1) ? 2 : 1;
    if (load_library(new_impl))
    {
        printf("Реализация переключена на %d\n", new_impl);
    }
    else
    {
        printf("Не удалось переключить реализацию\n");
    }
}

int main()
{
    char input[256];
    char command[10];

    printf("=== Программа №2 - Динамическое связывание ===\n");
    printf("Библиотеки загружаются во время выполнения\n\n");

    if (!load_library(1))
    {
        printf("Не удалось загрузить библиотеку по умолчанию\n");
        return 1;
    }

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
            switch_implementation();
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

    if (current_lib.handle)
    {
        dlclose(current_lib.handle);
    }

    return 0;
}