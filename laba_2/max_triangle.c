#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define MAX_POINTS 1000

// Структура для хранения координат точки
typedef struct {
    double x, y, z;
} Point;

// Структура для передачи данных в поток
typedef struct {
    Point *points; // Указатель на массив точек
    int start;     // Начальный индекс для обработки
    int end;       // Конечный индекс для обработки
    double *max_area; // Указатель на максимальную площадь
    Point *max_points; // Указатель на массив точек с максимальной площадью
} ThreadData;

// Функция для вычисления площади треугольника по трем точкам
double calculate_area(Point p1, Point p2, Point p3) {
    double a = sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2) + pow(p1.z - p2.z, 2));
    double b = sqrt(pow(p2.x - p3.x, 2) + pow(p2.y - p3.y, 2) + pow(p2.z - p3.z, 2));
    double c = sqrt(pow(p3.x - p1.x, 2) + pow(p3.y - p1.y, 2) + pow(p3.z - p1.z, 2));
    double s = (a + b + c) / 2;
    return sqrt(s * (s - a) * (s - b) * (s - c));
}

// Функция для обработки данных в потоке
void *process_points(void *arg) {
    ThreadData *data = (ThreadData *)arg; // Приведение аргумента к типу ThreadData
    double local_max_area = 0; // Локальная максимальная площадь
    Point local_max_points[3]; // Локальные точки с максимальной площадью

    // Перебор всех возможных комбинаций точек в заданном диапазоне
    for (int i = data->start; i < data->end; i++) {
        for (int j = i + 1; j < data->end; j++) {
            for (int k = j + 1; k < data->end; k++) {
                double area = calculate_area(data->points[i], data->points[j], data->points[k]);
                if (area > local_max_area) {
                    local_max_area = area;
                    local_max_points[0] = data->points[i];
                    local_max_points[1] = data->points[j];
                    local_max_points[2] = data->points[k];
                }
            }
        }
    }

    // Сохранение локальных результатов в глобальные переменные
    if (local_max_area > *data->max_area) {
        *data->max_area = local_max_area;
        data->max_points[0] = local_max_points[0];
        data->max_points[1] = local_max_points[1];
        data->max_points[2] = local_max_points[2];
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <max_threads>\n", argv[0]);
        return 1;
    }

    int max_threads = atoi(argv[1]); // Получение максимального количества потоков из аргумента командной строки
    int num_points = MAX_POINTS; // Количество точек
    Point points[MAX_POINTS]; // Массив точек

    // Инициализация точек (для примера используем случайные значения)
    for (int i = 0; i < num_points; i++) {
        points[i].x = rand() % 100;
        points[i].y = rand() % 100;
        points[i].z = rand() % 100;
    }

    pthread_t threads[max_threads]; // Массив потоков
    ThreadData data[max_threads]; // Массив данных для потоков

    double max_area = 0; // Максимальная площадь
    Point max_points[3]; // Точки с максимальной площадью

    int points_per_thread = num_points / max_threads; // Количество точек на поток
    for (int i = 0; i < max_threads; i++) {
        data[i].points = points;
        data[i].start = i * points_per_thread;
        data[i].end = (i == max_threads - 1) ? num_points : (i + 1) * points_per_thread;
        data[i].max_area = &max_area;
        data[i].max_points = max_points;
        pthread_create(&threads[i], NULL, process_points, (void *)&data[i]); // Создание потока
    }

    // Ожидание завершения всех потоков
    for (int i = 0; i < max_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    // Вывод результата
    printf("Maximum area: %f\n", max_area);
    printf("Points: (%f, %f, %f), (%f, %f, %f), (%f, %f, %f)\n",
           max_points[0].x, max_points[0].y, max_points[0].z,
           max_points[1].x, max_points[1].y, max_points[1].z,
           max_points[2].x, max_points[2].y, max_points[2].z);

    // Вывод количества потоков, используемых программой
    int active_threads = 0;
    for (int i = 0; i < max_threads; i++) {
        if (pthread_kill(threads[i], 0) == 0) {
            active_threads++;
        }
    }
    printf("Number of threads used by the program: %d\n", active_threads);

    return 0;
}
