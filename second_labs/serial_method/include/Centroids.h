#ifndef CENTROIDS_H
#define CENTROIDS_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#define MAX_ITER 100

typedef struct Point
{
    int x, y;
    int cluster;
} Point;

typedef struct Centroid
{
    int x, y;
    Point *points;
    int count_points;
} Centroid;

void Create(Point point[], int count_points);

int distance(Point a, Point b);

void add_point(Centroid *centroid, Point point);

Centroid *Initial_Centroids(Point point[], int count_points, int count_clusters);

void set_centroid(Point point[], int count_points, Centroid centroids[], int count_centroids);

void print_centroids(Centroid *centroids, int count_clusters);

int find_min_index(int distance[], int count_point);

void free_centroids(Centroid *centroids, int count_clusters);

void print_point(Centroid *centroids, int count_centroids);

void Change_Centroids(Centroid *centroids, int count_centroids);

#endif // CENTROIDS_H
