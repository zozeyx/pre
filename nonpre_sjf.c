#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

typedef struct {
    Node* front;
    Node* rear;
} Queue;

Queue* initQueue() {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    if (q == NULL) {
        perror("Queue initialization failed");
        exit(EXIT_FAILURE);
    }
    q->front = q->rear = NULL;
    return q;
}

void enqueue(Queue* q, int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    newNode->data = data;
    newNode->next = NULL;
    if (q->rear == NULL) {
        q->front = q->rear = newNode;
    } else {
        q->rear->next = newNode;
        q->rear = newNode;
    }
}

int dequeue(Queue* q) {
    if (q->front == NULL) {
        perror("Queue is empty");
        exit(EXIT_FAILURE);
    }
    Node* temp = q->front;
    int data = temp->data;
    q->front = q->front->next;
    if (q->front == NULL) {
        q->rear = NULL;
    }
    free(temp);
    return data;
}

typedef struct {
    int id;
    int arv;
    int exe;
} Process;

void *process(void *arg) {
    Process *p = (Process *)arg;
    for (int i = 1; i <= p->exe; i++) {
        printf("P%d: %d × %d = %d\n", p->id, i, p->id, i * p->id);
    }
    pthread_exit(NULL);
}

void enqueueProcesses(Queue* queue, Process* processes, int num_processes) {
    for (int i = 0; i < num_processes; i++) {
        if (processes[i].arv == 0) {
            enqueue(queue, i);
        }
    }
    // 나머지 프로세스를 큐에 넣기
    for (int i = 0; i < num_processes; i++) {
        if (processes[i].arv != 0) {
            enqueue(queue, i);
        }
    }
}


int main() {
    Queue* queue = initQueue();

    Process processes[] = {
        {1, 0, 10},
        {2, 1, 28},
        {3, 2, 6},
        {4, 3, 4},
        {5, 4, 14}
    };

    pthread_t threads[sizeof(processes) / sizeof(Process)];

    // 실행 시간(exe) 순으로 프로세스 정렬
    for (int i = 0; i < sizeof(processes) / sizeof(Process) - 1; i++) {
        for (int j = 0; j < sizeof(processes) / sizeof(Process) - i - 1; j++) {
            if (processes[j].exe > processes[j + 1].exe) {
                Process temp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp;
            }
        }
    }

    enqueueProcesses(queue, processes, sizeof(processes) / sizeof(Process));

    int end_time = 0;
    printf("간트 차트:\n");
    while (queue->front != NULL) {
    int gant= dequeue(queue);
    printf("P%d (%d-%d)\n", processes[gant].id, end_time, end_time + processes[gant].exe);
    end_time += processes[gant].exe;
    }
    printf("\n");

    enqueueProcesses(queue, processes, sizeof(processes) / sizeof(Process));

    // 큐에서 나온 순서대로 프로세스 실행
    while (queue->front != NULL) {
        int index = dequeue(queue);
        if (pthread_create(&threads[index], NULL, process, &processes[index]) != 0) {
            perror("Thread creation failed");
            exit(EXIT_FAILURE);
        }
        if (pthread_join(threads[index], NULL) != 0) {
            perror("Thread join failed");
            exit(EXIT_FAILURE);
        }
    }

    int return_time = 0;
    float avg_return_time = 0;

    enqueueProcesses(queue, processes, sizeof(processes) / sizeof(Process));

    while (queue->front != NULL) {
        int i = dequeue(queue);
        return_time = return_time + processes[i].exe;
        avg_return_time += return_time - processes[i].arv;
    }
    avg_return_time = avg_return_time / 5;
    printf("평균 반환시간: %.2f\n", avg_return_time);
    
    int wait_time = 0;
    float avg_wait_time = 0;

    enqueueProcesses(queue, processes, sizeof(processes) / sizeof(Process));

    while (queue->front != NULL) {
        int i = dequeue(queue);
        avg_wait_time += wait_time - processes[i].arv;
        wait_time = wait_time + processes[i].exe;
    }
    avg_wait_time = avg_wait_time / 5;
    printf("평균 대기시간: %.2f\n", avg_wait_time);

    return 0;
}
