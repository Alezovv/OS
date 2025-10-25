#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../include/Centroids.h"
#include <omp.h>

#define MAX_ITER 50

int input_number(const char *prompt, int min, int max)
{
    int value;
    printf("%s", prompt);
    if (scanf("%d", &value) != 1)
    {
        printf("Ошибка: введено не число!\n");
        exit(1);
    }
    if (value < min || value > max)
    {
        printf("Ошибка: значение должно быть от %d до %d\n", min, max);
        exit(1);
    }
    return value;
}

int main()
{
    int n_points = input_number("Введите число точек (1000-100000): ", 1000, 100000);
    int n_clusters = input_number("Введите число кластеров (1-100): ", 1, 100);

    Point *points = malloc(n_points * sizeof(Point));
    if (!points)
    {
        perror("malloc");
        exit(1);
    }
    initialize_points(points, n_points);
    Centroid *centroids = initialize_centroids(points, n_points, n_clusters);

    int threads_array[] = {1, 2, 4, 8, 16, 128, 1024};
    size_t num_experiments = sizeof(threads_array) / sizeof(int);
    double times[num_experiments];

    printf("Проверка производительности для разных количеств потоков:\n");

    for (size_t t = 0; t < num_experiments; t++)
    {
        int max_threads = threads_array[t];

        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start);

        for (int iter = 0; iter < MAX_ITER; iter++)
        {
            omp_set_num_threads(max_threads);
            assign_points_to_clusters(points, n_points, centroids, n_clusters, max_threads);
            update_centroids(points, n_points, centroids, n_clusters);
        }

        clock_gettime(CLOCK_MONOTONIC, &end);
        double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
        times[t] = elapsed;

        printf("Потоки=%d, время=%.6f секунд\n", max_threads, elapsed);
    }

    double T1 = times[0];
    printf("\nЧисло потоков | Время (с) | Ускорение | Эффективность\n");
    for (size_t t = 0; t < num_experiments; t++)
    {
        int p = threads_array[t];
        double Sp = T1 / times[t];
        double Ep = Sp / p;
        printf("%12d | %8.6f | %9.3f | %13.3f\n", p, times[t], Sp, Ep);
    }

    free(centroids);
    free(points);
    return 0;
}
