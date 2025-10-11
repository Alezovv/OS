#ifndef CENTROIDS_H
#define CENTROIDS_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#define MAX_ITER 100

// Структуры данных
typedef struct
{
    int x, y;
    int cluster;
} Point;

typedef struct
{
    int x, y;
    Point *points;
    int count_points;
} Centroid;

typedef struct
{
    Point *points;
    int start;
    int end;
    Centroid *centroids;
    int count_centroids;
} ThreadData;

// Прототипы функций
void Create(Point points[], int count_points);
int distance(Point a, Point b);
void add_point(Centroid *centroid, Point point);
Centroid *Initial_Centroids(Point points[], int count_points, int count_clusters);
void set_centroid(Point points[], int count_points, Centroid centroids[], int count_centroids);
void print_centroids(Centroid *centroids, int count_clusters);
void print_points(Centroid *centroids, int count_centroids);
void Change_Centroids(Centroid *centroids, int count_centroids);
void parallel_set_centroid(Point points[], int count_points, Centroid centroids[], int count_centroids, int num_threads);
void parallel_change_centroids(Centroid centroids[], int count_centroids);

#endif // CENTROIDS_H