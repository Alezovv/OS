#include "../include/Centroids.h"

void *assign_points_to_centroids(void *arg)
{
    ThreadData *data = (ThreadData *)arg;

    for (int i = data->start; i < data->end; i++)
    {
        int min_dist = INT_MAX;
        int closest_index = 0;
        for (int j = 0; j < data->count_centroids; j++)
        {
            int dist = distance(data->points[i], (Point){data->centroids[j].x, data->centroids[j].y, -1});
            if (dist < min_dist)
            {
                min_dist = dist;
                closest_index = j;
            }
        }
        data->points[i].cluster = closest_index;
        add_point(&data->centroids[closest_index], data->points[i]);
    }
    return NULL;
}

// Функция для пересчета центроидов (для потоков)
void *update_centroid(void *arg)
{
    Centroid *centroid = (Centroid *)arg;
    int sum_x = 0, sum_y = 0;

    for (int i = 0; i < centroid->count_points; i++)
    {
        sum_x += centroid->points[i].x;
        sum_y += centroid->points[i].y;
    }

    if (centroid->count_points > 0)
    {
        centroid->x = sum_x / centroid->count_points;
        centroid->y = sum_y / centroid->count_points;
    }

    return NULL;
}

// Основная функция для распределения точек по центроидам с использованием потоков
void parallel_set_centroid(Point points[], int count_points, Centroid centroids[], int count_centroids, int num_threads)
{
    pthread_t threads[num_threads];
    ThreadData data[num_threads];

    // Разделяем работу по потокам
    int chunk_size = count_points / num_threads;
    for (int i = 0; i < num_threads; i++)
    {
        data[i].points = points;
        data[i].start = i * chunk_size;
        data[i].end = (i == num_threads - 1) ? count_points : (i + 1) * chunk_size;
        data[i].centroids = centroids;
        data[i].count_centroids = count_centroids;

        pthread_create(&threads[i], NULL, assign_points_to_centroids, (void *)&data[i]);
    }

    // Ждем завершения всех потоков
    for (int i = 0; i < num_threads; i++)
    {
        pthread_join(threads[i], NULL);
    }
}

// Основная функция для параллельного пересчета центроидов
void parallel_change_centroids(Centroid centroids[], int count_centroids)
{
    pthread_t threads[count_centroids];

    for (int i = 0; i < count_centroids; i++)
    {
        pthread_create(&threads[i], NULL, update_centroid, (void *)&centroids[i]);
    }

    // Ждем завершения всех потоков
    for (int i = 0; i < count_centroids; i++)
    {
        pthread_join(threads[i], NULL);
    }
}

// Функция для создания случайных точек
void Create(Point points[], int count_points)
{
    srand(time(NULL));

    for (int i = 0; i < count_points; i++)
    {
        points[i].x = rand() % 100;
        points[i].y = rand() % 100;
        points[i].cluster = -1;
    }
}

// Функция для вычисления расстояния между точками
int distance(Point a, Point b)
{
    return ((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

// Функция для добавления точки в кластер (центроид)
void add_point(Centroid *centroid, Point point)
{
    centroid->count_points++;
    Point *new_points = realloc(centroid->points, centroid->count_points * sizeof(Point));
    if (new_points == NULL)
    {
        printf("Ошибка памяти!\n");
        centroid->count_points--;
        return;
    }
    centroid->points = new_points;
    centroid->points[centroid->count_points - 1] = point;
}

// Функция для инициализации центроидов
Centroid *Initial_Centroids(Point points[], int count_points, int count_clusters)
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
        centroids[i].x = points[random_index].x;
        centroids[i].y = points[random_index].y;
        centroids[i].points = NULL;
        centroids[i].count_points = 0;
    }

    return centroids;
}

// Функция для распределения точек по центроидам (непараллельная версия)
void set_centroid(Point points[], int count_points, Centroid centroids[], int count_centroids)
{
    for (int i = 0; i < count_points; i++)
    {
        int min_dist = INT_MAX;
        int closest_index = 0;
        for (int j = 0; j < count_centroids; j++)
        {
            int dist = distance(points[i], (Point){centroids[j].x, centroids[j].y, -1});
            if (dist < min_dist)
            {
                min_dist = dist;
                closest_index = j;
            }
        }
        points[i].cluster = closest_index;
        add_point(&centroids[closest_index], points[i]);
    }
}

// Функция для вывода центроидов
void print_centroids(Centroid *centroids, int count_clusters)
{
    printf("Центроиды:\n");
    for (int i = 0; i < count_clusters; i++)
    {
        printf("Центроид %d: (%d, %d), точек: %d\n", i, centroids[i].x, centroids[i].y, centroids[i].count_points);
    }
}

// Функция для вывода точек в каждом кластере
void print_points(Centroid *centroids, int count_centroids)
{
    printf("Точки в кластерах:\n");
    for (int i = 0; i < count_centroids; i++)
    {
        printf("Центроид %d: \n", i);
        for (int j = 0; j < centroids[i].count_points; j++)
        {
            printf("Точка %d: (%d, %d)\n", j, centroids[i].points[j].x, centroids[i].points[j].y);
        }
    }
}

// Функция для пересчета центроидов
void Change_Centroids(Centroid *centroids, int count_centroids)
{
    for (int i = 0; i < count_centroids; i++)
    {
        if (centroids[i].count_points > 0)
        {
            int sum_x = 0, sum_y = 0;
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