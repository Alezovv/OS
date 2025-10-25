#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include "../include/Centroids.h"

typedef struct
{
    Point *points;
    size_t start, end;
    Centroid *centroids;
    size_t k;
} ThreadData;

// Генерация случайных точек
void initialize_points(Point *points, size_t n)
{
    for (size_t i = 0; i < n; i++)
    {
        points[i].x = (double)rand() / RAND_MAX * 100.0;
        points[i].y = (double)rand() / RAND_MAX * 100.0;
        points[i].cluster = -1;
    }
}

// Инициализация центроидов случайно
Centroid *initialize_centroids(Point *points, size_t n, size_t k)
{
    Centroid *centroids = malloc(k * sizeof(Centroid));
    for (size_t i = 0; i < k; i++)
    {
        centroids[i].x = points[i % n].x;
        centroids[i].y = points[i % n].y;
    }
    return centroids;
}

void free_centroids(Centroid *centroids)
{
    free(centroids);
}

// Потоковая функция назначения точек к ближайшему центроиду
void *assign_block(void *arg)
{
    ThreadData *data = (ThreadData *)arg;
    for (size_t i = data->start; i < data->end; i++)
    {
        double min_dist = INFINITY;
        int cluster = 0;
        for (size_t j = 0; j < data->k; j++)
        {
            double dx = data->points[i].x - data->centroids[j].x;
            double dy = data->points[i].y - data->centroids[j].y;
            double dist = dx * dx + dy * dy;
            if (dist < min_dist)
            {
                min_dist = dist;
                cluster = j;
            }
        }
        data->points[i].cluster = cluster;
    }
    return NULL;
}

void assign_points_to_clusters(Point *points, size_t n, Centroid *centroids, size_t k, int max_threads)
{
    pthread_t threads[max_threads];
    ThreadData thread_data[max_threads];

    size_t block = n / max_threads;
    for (int t = 0; t < max_threads; t++)
    {
        thread_data[t].points = points;
        thread_data[t].centroids = centroids;
        thread_data[t].k = k;
        thread_data[t].start = t * block;
        thread_data[t].end = (t == max_threads - 1) ? n : (t + 1) * block;
        pthread_create(&threads[t], NULL, assign_block, &thread_data[t]);
    }

    for (int t = 0; t < max_threads; t++)
    {
        pthread_join(threads[t], NULL);
    }
}

// Обновление центроидов
void update_centroids(Point *points, size_t n, Centroid *centroids, size_t k)
{
    double *sum_x = calloc(k, sizeof(double));
    double *sum_y = calloc(k, sizeof(double));
    size_t *count = calloc(k, sizeof(size_t));

    for (size_t i = 0; i < n; i++)
    {
        int c = points[i].cluster;
        sum_x[c] += points[i].x;
        sum_y[c] += points[i].y;
        count[c]++;
    }

    for (size_t j = 0; j < k; j++)
    {
        if (count[j] != 0)
        {
            centroids[j].x = sum_x[j] / count[j];
            centroids[j].y = sum_y[j] / count[j];
        }
    }

    free(sum_x);
    free(sum_y);
    free(count);
}
