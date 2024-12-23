#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/msg.h> 
 
#define MSG_SIZE 256 
 
typedef struct { 
    long mtype; 
    char mtext[MSG_SIZE]; 
} Message; 
 
int main(int argc, char *argv[]) { 
    if (argc < 3) { 
        fprintf(stderr, "Usage: %s <node_id> <parent_id>\n", argv[0]); 
        exit(1); 
    } 
 
    int node_id = atoi(argv[1]); 
    int parent_id = atoi(argv[2]); 
 
    int msg_queue_id = msgget(IPC_PRIVATE, 0666 | IPC_CREAT); 
    if (msg_queue_id < 0) { 
        perror("msgget failed"); 
        exit(1); 
    } 
 
    while (1) { 
        Message msg; 
        if (msgrcv(msg_queue_id, &msg, MSG_SIZE, node_id, 0) < 0) { 
            perror("msgrcv failed"); 
            exit(1); 
        } 
        printf("Node %d received command: %s\n", node_id, msg.mtext); 
 
        // Выполнение команды (например, суммирование чисел) 
        int sum = 0; 
        char *token = strtok(msg.mtext, " "); 
        while (token != NULL) { 
            sum += atoi(token); 
            token = strtok(NULL, " "); 
        } 
 
        // Отправка ответа 
        Message response; 
        response.mtype = 1; // Ответ управляющему узлу 
        sprintf(response.mtext, "Ok:%d: %d", node_id, sum); 
        msgsnd(msg_queue_id, &response, strlen(response.mtext) + 1, 0); 
    } 
 
    return 0; 
}
