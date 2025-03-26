#include <stdio.h>
#include <stdlib.h>

#define MAX_PROCESSES 100

typedef struct {
    int pid;
    int arrival_time;
    int burst_time;
    int waiting_time;
    int turnaround_time;
    int completed;
} Process;

void swap(Process *a, Process *b) {
    Process temp = *a;
    *a = *b;
    *b = temp;
}

void sort_by_arrival(Process processes[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (processes[i].arrival_time > processes[j].arrival_time) {
                swap(&processes[i], &processes[j]);
            }
        }
    }
}

void display_results(Process processes[], int n) {
    int current_time = 0;
    printf("Gantt Chart:\n| ");
    for (int i = 0; i < n; i++) {
        printf("P%d | ", processes[i].pid);
        current_time += processes[i].burst_time;
    }
    printf("\n0");
    current_time = 0;
    for (int i = 0; i < n; i++) {
        current_time += processes[i].burst_time;
        printf("   %d", current_time);
    }
    printf("\n\nPID\tWT\tTAT\n");
    float total_wt = 0, total_tat = 0;
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\n", processes[i].pid, processes[i].waiting_time, processes[i].turnaround_time);
        total_wt += processes[i].waiting_time;
        total_tat += processes[i].turnaround_time;
    }
    printf("\nAverage Waiting Time: %.2f\n", total_wt / n);
    printf("Average Turnaround Time: %.2f\n", total_tat / n);
}

void fcfs(Process processes[], int n) {
    printf("\n--- First-Come, First-Served (FCFS) ---\n");
    sort_by_arrival(processes, n);
    int current_time = 0;
    for (int i = 0; i < n; i++) {
        if (current_time < processes[i].arrival_time) {
            current_time = processes[i].arrival_time;
        }
        processes[i].waiting_time = current_time - processes[i].arrival_time;
        processes[i].turnaround_time = processes[i].waiting_time + processes[i].burst_time;
        current_time += processes[i].burst_time;
    }
    display_results(processes, n);
}

void sjf(Process processes[], int n) {
    printf("\n--- Shortest Job First (SJF) ---\n");
    int completed = 0, current_time = 0;
    int is_completed[MAX_PROCESSES] = {0};
    Process result[MAX_PROCESSES];

    while (completed != n) {
        int shortest_index = -1;
        int min_burst = __INT_MAX__;
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= current_time && !is_completed[i]) {
                if (processes[i].burst_time < min_burst) {
                    min_burst = processes[i].burst_time;
                    shortest_index = i;
                } else if (processes[i].burst_time == min_burst) {
                    if (processes[i].arrival_time < processes[shortest_index].arrival_time) {
                        shortest_index = i;
                    }
                }
            }
        }
        if (shortest_index == -1) {
            current_time++;
        } else {
            processes[shortest_index].waiting_time = current_time - processes[shortest_index].arrival_time;
            if (processes[shortest_index].waiting_time < 0) {
                processes[shortest_index].waiting_time = 0;
            }
            processes[shortest_index].turnaround_time = processes[shortest_index].waiting_time + processes[shortest_index].burst_time;
            current_time += processes[shortest_index].burst_time;
            is_completed[shortest_index] = 1;
            result[completed] = processes[shortest_index];
            completed++;
        }
    }
    for (int i = 0; i < n; i++) {
        processes[i] = result[i];
    }
    display_results(processes, n);
}

int read_process_data(Process processes[], const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Unable to open file %s\n", filename);
        exit(1);
    }
    int i = 0;
    char line[100];
    fgets(line, sizeof(line), file);
    while (fscanf(file, "%d %d %d %*d", &processes[i].pid, &processes[i].arrival_time, &processes[i].burst_time) != EOF) {
        processes[i].waiting_time = 0;
        processes[i].turnaround_time = 0;
        processes[i].completed = 0;
        i++;
    }
    fclose(file);
    return i;
}

int main() {
    Process processes[MAX_PROCESSES];
    int n = read_process_data(processes, "processes2.txt");
    fcfs(processes, n);
    n = read_process_data(processes, "processes2.txt");
    sjf(processes, n);
    return 0;
}
