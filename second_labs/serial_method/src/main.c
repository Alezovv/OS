#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../include/Centroids.h"

int input_number(const char *prompt, int min, int max, const char *error_msg)
{
    int value;
    printf("%s", prompt);

    if (scanf("%d", &value) != 1)
    {
        printf("Ошибка: введено не число!\n");
        return -1;
    }

    if (value < min || value > max)
    {
        printf("Ошибка: %s\n", error_msg);
        printf("Допустимый диапазон: от %d до %d\n", min, max);
        return -1;
    }
    return value;
}

int main(void)
{
    int count_points = input_number(
        "Введите число точек (2 - 10000): ",
        2, 10000,
        "число точек должно быть от 2 до 10000");

    if (count_points == -1)
        return 1;

    int count_clusters = input_number(
        "Введите число кластеров (1 - 1000): ",
        1, 1000,
        "число кластеров должно быть от 1 до 1000");

    if (count_clusters == -1)
        return 1;

    if (count_clusters > count_points)
    {
        printf("Ошибка: кластеров не может быть больше чем точек!\n");
        return 1;
    }

    clock_t start = clock();

    Point matrix[count_points];
    Create(matrix, count_points);

    Centroid *centroids = Initial_Centroids(matrix, count_points, count_clusters);

    set_centroid(matrix, count_points, centroids, count_clusters);

    print_centroids(centroids, count_clusters);
    print_point(centroids, count_clusters);

    for (int i = 0; i < MAX_ITER; i++)
    {
        Change_Centroids(centroids, count_clusters);
        set_centroid(matrix, count_points, centroids, count_clusters);
    }

    print_centroids(centroids, count_clusters);
    print_point(centroids, count_clusters);

    free_centroids(centroids, count_clusters);

    clock_t end = clock();

    double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
    printf("\nВремя выполнения программы: %.6f секунд\n", elapsed);

    return 0;
}
