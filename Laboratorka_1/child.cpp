#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>

// laba_1

int main(int argc, char *argv[]) {
    // Проверяем, что передано достаточное количество аргументов
    if (argc < 3) {
        fprintf(stderr, "Использование: %s <pipe_read_fd> <output_file>\n", argv[0]);
        return 1;
    }

    // Преобразуем первый аргумент в дескриптор канала для чтения
    int pipe_read_fd = atoi(argv[1]);

    // Открываем файл для записи, создаем его, если он не существует, и обрезаем его содержимое
    int file = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (file == -1) {
        perror("open");
        return 1;
    }

    // Буфер для чтения данных из канала
    char buffer[256];
    // Читаем данные из канала в буфер
    read(pipe_read_fd, buffer, sizeof(buffer));
    // Закрываем дескриптор канала для чтения
    close(pipe_read_fd);

    // Инициализируем переменную для хранения суммы чисел
    int sum = 0;
    // Разбиваем строку на токены (числа) по пробелам
    char *token = strtok(buffer, " ");
    while (token != NULL) {
        // Преобразуем токен в число и добавляем его к сумме
        sum += atoi(token);
        // Получаем следующий токен
        token = strtok(NULL, " ");
    }

    // Записываем сумму в файл
    dprintf(file, "%d", sum);
    // Закрываем файл
    close(file);

    return 0;
}
