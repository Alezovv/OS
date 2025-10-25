#ifndef KMEANS_H
#define KMEANS_H

#include <stddef.h>

typedef struct
{
    double x;
    double y;
    int cluster;
} Point;

typedef struct
{
    double x;
    double y;
} Centroid;

// Функции k-средних
void initialize_points(Point *points, size_t n);
Centroid *initialize_centroids(Point *points, size_t n, size_t k);
void free_centroids(Centroid *centroids);
void assign_points_to_clusters(Point *points, size_t n, Centroid *centroids, size_t k, int max_threads);
void update_centroids(Point *points, size_t n, Centroid *centroids, size_t k);

#endif
