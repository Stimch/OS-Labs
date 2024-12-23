#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <time.h>
#include <errno.h>  // Добавленный заголовок

#define MAX_NODES 100
#define MSG_SIZE 256

typedef struct {
    long mtype;
    char mtext[MSG_SIZE];
} Message;

typedef struct {
    int id;
    pid_t pid;
    int parent_id;
    int available;
} Node;

Node nodes[MAX_NODES];
int node_count = 0;
int msg_queue_id;

void create_node(int id, int parent_id) { 
    if (id < 0) { 
        printf("Error: Invalid node ID\n"); 
        return; 
    } 
 
    for (int i = 0; i < node_count; i++) { 
        if (nodes[i].id == id) { 
            printf("Error: Already exists\n"); 
            return; 
        } 
    } 
 
    if (parent_id != -1) { 
        int found = 0; 
        for (int i = 0; i < node_count; i++) { 
            if (nodes[i].id == parent_id) { 
                found = 1; 
                if (!nodes[i].available) { 
                    printf("Error: Parent is unavailable\n"); 
                    return; 
                } 
                break; 
            } 
        } 
        if (!found) { 
            printf("Error: Parent not found\n"); 
            return; 
        } 
    } 
 
    pid_t pid = fork(); 
    if (pid < 0) { 
        perror("Fork failed"); 
        exit(1); 
    } else if (pid == 0) { 
        // Запуск вычислительного узла 
        char id_str[16], parent_str[16]; 
        sprintf(id_str, "%d", id); 
        sprintf(parent_str, "%d", parent_id); 
        execl("./compute_node", "compute_node", id_str, parent_str, NULL); 
        perror("execl failed"); 
        exit(1); 
    } else { 
        // Управляющий узел 
        nodes[node_count].id = id; 
        nodes[node_count].pid = pid; 
        nodes[node_count].parent_id = parent_id; 
        nodes[node_count].available = 1; 
        node_count++; 
        printf("Ok: %d\n", pid); 
    } 
} 
 
void exec_command(int id, char *params) { 
    int found = 0; 
    for (int i = 0; i < node_count; i++) { 
        if (nodes[i].id == id) { 
            found = 1; 
            if (!nodes[i].available) { 
                printf("Error:%d: Node is unavailable\n", id); 
                return; 
            } 
 
            Message msg; 
            msg.mtype = id; 
            sprintf(msg.mtext, "%s", params); 
            msgsnd(msg_queue_id, &msg, strlen(msg.mtext) + 1, 0); 
 
            // Получение ответа 
            Message response; 
            msgrcv(msg_queue_id, &response, MSG_SIZE, 1, 0); 
            printf("%s\n", response.mtext); 
            return; 
        } 
    } 
 
    if (!found) { 
        printf("Error:%d: Not found\n", id); 
    } 
} 

void heartbit(int time) {
    while (1) {
        sleep(time / 1000);
        for (int i = 0; i < node_count; i++) {
            if (kill(nodes[i].pid, 0) == -1) {
                if (errno == ESRCH) {  // Теперь это корректно
                    printf("Heartbit: node %d is unavailable now\n", nodes[i].id);
                    nodes[i].available = 0;
                }
            } else {
                nodes[i].available = 1;
            }
        }
    }
}

int main() {
    msg_queue_id = msgget(IPC_PRIVATE, 0666 | IPC_CREAT);
    if (msg_queue_id < 0) {
        perror("msgget failed");
        exit(1);
    }

    // Инициализация узлов
    create_node(10, -1);
    create_node(20, 10);
    create_node(15, 20);
    create_node(1, -1);
    create_node(7, 1);
    create_node(9, -1);
    create_node(12, 9);
    create_node(5, 12);

    // Пример команд
    exec_command(10, "3 1 2 3");
    exec_command(20, "2 4 5");

    // Проверка доступности узлов
    heartbit(2000);

    return 0;
}
