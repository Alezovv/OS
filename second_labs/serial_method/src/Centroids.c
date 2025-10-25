#include "../include/Centroids.h"

void Create(Point point[], int count_points)
{
    srand(time(NULL));

    for (int i = 0; i < count_points; i++)
    {
        point[i].x = rand() % 100;
        point[i].y = rand() % 100;
        point[i].cluster = -1;
    }
}

Centroid *Initial_Centroids(Point point[], int count_points, int count_clusters)
{
    Centroid *centroids = malloc(count_clusters * sizeof(Centroid));
    if (centroids == NULL)
    {
        printf("Ошибка выделения памяти для центроидов!\n");
        return NULL;
    }

    for (int i = 0; i < count_clusters; i++)
    {
        int random_index = rand() % count_points;

        centroids[i].x = point[random_index].x;
        centroids[i].y = point[random_index].y;

        centroids[i].points = NULL;
        centroids[i].count_points = 0;
    }

    return centroids;
}

void print_centroids(Centroid *centroids, int count_clusters)
{
    printf("\nЦентроиды:\n");
    for (int i = 0; i < count_clusters; i++)
    {
        printf("Центроид №%d: (%d, %d), точек: %d\n",
               i, centroids[i].x, centroids[i].y, centroids[i].count_points);
    }
}

void add_point_to_centroid(Centroid *centroid, Point point)
{
    centroid->count_points++;
    Point *new_points = realloc(centroid->points,
                                centroid->count_points * sizeof(Point));
    if (new_points == NULL)
    {
        printf("Ошибка памяти!\n");
        centroid->count_points--;
        return;
    }
    centroid->points = new_points;
    centroid->points[centroid->count_points - 1] = point;
}

void set_centroid(Point points[], int count_points, Centroid centroids[], int count_centroids)
{
    for (int j = 0; j < count_centroids; j++)
    {
        free(centroids[j].points);
        centroids[j].points = NULL;
        centroids[j].count_points = 0;
    }

    for (int i = 0; i < count_points; i++)
    {
        int min_distance = INT_MAX;
        int closest_index = 0;

        for (int j = 0; j < count_centroids; j++)
        {
            int dist = distance(points[i],
                                (Point){centroids[j].x, centroids[j].y, -1});
            if (dist < min_distance)
            {
                min_distance = dist;
                closest_index = j;
            }
        }

        points[i].cluster = closest_index;

        add_point_to_centroid(&centroids[closest_index], points[i]);
    }
}

void Change_Centroids(Centroid *centroids, int count_centroids)
{
    for (int i = 0; i < count_centroids; i++)
    {
        if (centroids[i].count_points > 0)
        {
            int sum_x = 0;
            int sum_y = 0;

            for (int j = 0; j < centroids[i].count_points; j++)
            {
                sum_x += centroids[i].points[j].x;
                sum_y += centroids[i].points[j].y;
            }

            centroids[i].x = sum_x / centroids[i].count_points;
            centroids[i].y = sum_y / centroids[i].count_points;
        }
    }
}

int distance(Point a, Point b)
{
    return ((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

int find_min_index(int distance[], int count_point)
{
    if (count_point <= 0)
    {
        return -1;
    }

    int min_index = 0;

    for (int i = 1; i < count_point; i++)
    {
        if (distance[i] < distance[min_index])
        {
            min_index = i;
        }
    }

    return min_index;
}

void free_centroids(Centroid *centroids, int count_clusters)
{
    for (int i = 0; i < count_clusters; i++)
    {
        free(centroids[i].points);
    }
    free(centroids);
}

void print_point(Centroid *centroids, int count_centroids)
{
    printf("\nТочки центроидов: \n");
    for (int i = 0; i < count_centroids; i++)
    {
        printf("\nЦентроид №%d: \n\n", i);
        for (int j = 0; j < centroids[i].count_points; j++)
            printf("Точка %d: (%d, %d)\n", j,
                   centroids[i].points[j].x, centroids[i].points[j].y); 
    }
}