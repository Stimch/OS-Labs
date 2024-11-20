#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Использование: %s <file_path> <int FILESIZE> <output_file>\n", argv[0]);
        return 1;
    }

    // Получаем аргументы
    char* filepath = argv[1];
    int FILESIZE = atoi(argv[2]);
    char* output_file = argv[3];

    // Открываем файл и отображаем его в память
    int fd = open(filepath, O_RDWR);
    if (fd == -1) {
        perror("Error opening file in child.");
        return 1;
    }

    char *mapped = (char*)mmap(NULL, FILESIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mapped == MAP_FAILED) {
        perror("Error mapping file in child.");
        close(fd);
        return 1;
    }
    close(fd);

    // Складываем числа
    int sum = 0;
    char *token = strtok(mapped, " ");
    while (token != NULL) {
        sum += atoi(token);
        token = strtok(NULL, " ");
    }

    printf("Сумма чисел: %d\n", sum);

    // Открываем файл для записи результата
    fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("Error opening output file.");
        munmap(mapped, FILESIZE);
        return 1;
    }

    // Записываем сумму в файл
    char sum_str[20];
    sprintf(sum_str, "%d", sum);
    if (write(fd, sum_str, strlen(sum_str)) == -1) {
        perror("Error writing to output file.");
        close(fd);
        munmap(mapped, FILESIZE);
        return 1;
    }

    // Закрываем файл
    close(fd);

    // Освобождаем память
    munmap(mapped, FILESIZE);
    return 0;
}
