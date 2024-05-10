#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// 큐 노드 구조체 정의
typedef struct Node {
    int data;
    struct Node* next;
} Node;

// 큐 구조체 정의
typedef struct {
    Node* front;
    Node* rear;
} Queue;

// 큐 초기화 함수
Queue* initQueue() {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    if (q == NULL) {
        perror("Queue initialization failed");
        exit(EXIT_FAILURE);
    }
    q->front = q->rear = NULL;
    return q;
}

// 큐에 데이터 추가 함수
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

// 큐에서 데이터 제거 함수
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

// 구조체 정의
typedef struct {
    int id;
    int arv;
    int exe;
} Process;

// 각 프로세스에 대한 처리 함수
void *process(void *arg) {
    Process *p = (Process *)arg;
    for (int i = 1; i <= p->exe; i++) {
        printf("P%d: %d × %d = %d\n", p->id, i, p->id, i * p->id);
    }
    pthread_exit(NULL);
}

int main() {
    // 큐 초기화
    Queue* queue = initQueue();

    // 프로세스들의 정보
    Process processes[] = {
        {1, 0, 10},
        {2, 1, 28},
        {3, 2, 6},
        {4, 3, 4},
        {5, 4, 14}
    };

    pthread_t threads[sizeof(processes) / sizeof(Process)];

    // 큐에 프로세스들 추가
    for (int i = 0; i < sizeof(processes) / sizeof(Process); i++) {
        enqueue(queue, i);
    }

    // 칸트 차트 출력
    int end_time = 0;
    printf("간트 차트:\n");
    for (int i = 0; i < sizeof(processes) / sizeof(Process); i++) {
        printf("P%d (%d-%d)\n", processes[i].id, end_time, end_time + processes[i].exe);
        end_time += processes[i].exe;
    }
    printf("\n");

    // 큐에서 프로세스를 꺼내어 실행
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
    float avg_return_time =0;
    for (int i =0; i < sizeof(processes) / sizeof(Process); i++){
        return_time = return_time + processes[i].exe;
        avg_return_time += return_time - processes[i].arv;
    }
    avg_return_time = avg_return_time / 5;
    printf("평균 반환시간: %.2f\n", avg_return_time);
    
    int wait_time = 0;
    float avg_wait_time =0;
    for (int i =0; i < sizeof(processes) / sizeof(Process); i++){
        avg_wait_time += wait_time - processes[i].arv;
        wait_time = wait_time + processes[i].exe;
    }
    avg_wait_time = avg_wait_time / 5;
    printf("평균 대기시간: %.2f\n", avg_wait_time);

    return 0;
}
