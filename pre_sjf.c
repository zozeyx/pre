#include <stdio.h>

struct Process {
    int pid;            // Process ID
    int arrival_time;   // Arrival time of the process
    int burst_time;     // Burst time of the process
    int remaining_time; // Remaining burst time of the process
    int plus_time;
    int start_time;
    int wait_time;
    int return_time;
};

void SRTF(struct Process processes[], int n) {
    int total_time = 0;
    int completed = 0;
    int shortest = -1;
    int shortest_index;
    int time_quantum = 1; // Considering preemption after 1 unit of time

    printf("Gantt Chart:\n");
    while (completed != n) {
        shortest_index = -1;
        shortest = -1;
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= total_time && processes[i].remaining_time > 0) {
                if (shortest == -1 || processes[i].remaining_time < shortest) {
                    shortest = processes[i].remaining_time;
                    shortest_index = i;
                }
            }
        }

        if (shortest_index == -1) {
            total_time++;
            continue;
        }

        if (processes[shortest_index].start_time == 0) {
            processes[shortest_index].start_time = total_time;
            processes[shortest_index].wait_time = total_time - processes[shortest_index].arrival_time;
        }

        processes[shortest_index].plus_time++;
        printf("P%d: %d x %d = %d\n", processes[shortest_index].pid,processes[shortest_index].plus_time, processes[shortest_index].pid,processes[shortest_index].plus_time * processes[shortest_index].pid);
       
        // Execute the process for 1 unit of time
        processes[shortest_index].remaining_time--;
        total_time++;

        // If process completes its execution
        if (processes[shortest_index].remaining_time == 0) {
            processes[shortest_index].return_time = total_time - processes[shortest_index].arrival_time;
            completed++;
        }
    }

    printf("\n\n");

    float avg_wait_time = 0;
    float avg_return_time = 0;

    for (int i = 0; i < n; i ++) {
        avg_wait_time += processes[i].wait_time;
    }
    avg_wait_time = avg_wait_time / 5;
    printf("평균 대기시간: %.2f\n", avg_wait_time);

    for (int i = 0; i < n; i ++) {
        avg_return_time += processes[i].return_time;
    }
    avg_return_time = avg_return_time / 5;
    printf("평균 반환시간: %.2f\n", avg_return_time);

}

int main() {
    // Processes with ID, arrival time, and burst time
    struct Process processes[] = {
        {1, 0, 10, 10, 0, 0, 0, 0}, 
        {2, 1, 28, 28, 0, 0, 0, 0}, 
        {3, 2, 6, 6, 0, 0, 0, 0}, 
        {4, 3, 4, 4, 0, 0, 0, 0}, 
        {5, 4, 14, 14, 0, 0, 0, 0}
    };

    int n = sizeof(processes) / sizeof(processes[0]);

    printf("Shortest Remaining Time First (SRTF) Scheduling Algorithm\n");
    SRTF(processes, n);

    return 0;
}
