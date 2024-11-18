#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>

// laba_1

int main() {
    int fd[2];
    // Создаем канал (pipe) для межпроцессного взаимодействия
    if (pipe(fd) == -1) {
        perror("pipe creation error");
        return 1;
    }

    std::string output_file;
    std::cout << "Name: ";
    // Получаем имя файла от пользователя
    std::getline(std::cin, output_file);

    pid_t pid = fork();
    // Создаем новый процесс
    if (pid == -1) {
        perror("Error!");
        return 1;
    }

    if (pid == 0) {
        // Этот блок выполняется в дочернем процессе

        // Закрываем записывающий конец канала, так как он не нужен в дочернем процессе
        close(fd[1]);

        char pipe_read_fd[10], file_name_arg[256];
        // Преобразуем дескриптор чтения канала в строку
        sprintf(pipe_read_fd, "%d", fd[0]);
        // Копируем имя файла в массив символов
        strcpy(file_name_arg, output_file.c_str());

        // Аргументы для вызова execve
        char *args[] = {"./child", pipe_read_fd, file_name_arg, NULL};

        // Заменяем текущий процесс новым процессом, запуская исполняемый файл ./child
        execve("./child", args, NULL);
        perror("execve");
        exit(1);
    } else {
        // Этот блок выполняется в родительском процессе

        // Закрываем читающий конец канала, так как он не нужен в родительском процессе
        close(fd[0]);

        std::string input_data;
        std::cout << "Numbers: ";
        // Получаем данные от пользователя
        std::getline(std::cin, input_data);

        // Записываем данные в канал
        write(fd[1], input_data.c_str(), input_data.size());
        // Закрываем записывающий конец канала
        close(fd[1]);

        // Ждем завершения дочернего процесса
        wait(NULL);
    }

    return 0;
}
