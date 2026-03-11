#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int pid;    // mã tiến trình
    int atime;  // arrival time
    int btime;  // burst time
    int pri;    // priority (số nhỏ hơn => ưu tiên cao hơn)
    int wtime;  // waiting time
    int ttime;  // turnaround time
    int done;   // đã hoàn thành chưa
} sp;

int main() {
    int i, n, current_time = 0, finished = 0;
    int total_wtime = 0, total_ttime = 0;

    printf("\nPriority Non-Preemptive Scheduling (with Arrival Time)\n");
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    sp *p = (sp*)malloc(n * sizeof(sp));
    if (p == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    // Mảng lưu các đoạn để in Gantt Chart
    int *g_pid = (int*)malloc((2 * n + 5) * sizeof(int));   // -1 = idle
    int *g_start = (int*)malloc((2 * n + 5) * sizeof(int));
    int *g_end = (int*)malloc((2 * n + 5) * sizeof(int));
    int g_count = 0;

    if (g_pid == NULL || g_start == NULL || g_end == NULL) {
        printf("Memory allocation failed!\n");
        free(p);
        return 1;
    }

    printf("Enter arrival time, burst time and priority (lower number = higher priority):\n");
    for(i = 0; i < n; i++) {
        printf("Process %d (arrival burst priority): ", i + 1);
        scanf("%d %d %d", &p[i].atime, &p[i].btime, &p[i].pri);

        p[i].pid = i + 1;
        p[i].wtime = 0;
        p[i].ttime = 0;
        p[i].done = 0;
    }

    while(finished < n) {
        int min_idx = -1;

        // Tìm tiến trình có priority cao nhất trong số đã đến và chưa xong
        for(i = 0; i < n; i++) {
            if(!p[i].done && p[i].atime <= current_time) {
                if(min_idx == -1) {
                    min_idx = i;
                } else if(p[i].pri < p[min_idx].pri) {
                    min_idx = i;
                } else if(p[i].pri == p[min_idx].pri) {
                    // Nếu cùng priority thì ưu tiên arrival time nhỏ hơn
                    if(p[i].atime < p[min_idx].atime) {
                        min_idx = i;
                    }
                    // Nếu arrival cũng bằng nhau thì ưu tiên PID nhỏ hơn
                    else if(p[i].atime == p[min_idx].atime && p[i].pid < p[min_idx].pid) {
                        min_idx = i;
                    }
                }
            }
        }

        // Nếu chưa có tiến trình nào sẵn sàng => CPU idle
        if(min_idx == -1) {
            int next = 999999;
            for(i = 0; i < n; i++) {
                if(!p[i].done && p[i].atime < next) {
                    next = p[i].atime;
                }
            }

            // Lưu đoạn idle vào Gantt Chart
            g_pid[g_count] = -1;
            g_start[g_count] = current_time;
            g_end[g_count] = next;
            g_count++;

            current_time = next;
            continue;
        }

        // Lưu đoạn process chạy vào Gantt Chart
        g_pid[g_count] = p[min_idx].pid;
        g_start[g_count] = current_time;
        g_end[g_count] = current_time + p[min_idx].btime;
        g_count++;

        // Tính waiting time và turnaround time
        p[min_idx].wtime = current_time - p[min_idx].atime;
        current_time += p[min_idx].btime;
        p[min_idx].ttime = current_time - p[min_idx].atime;
        p[min_idx].done = 1;
        finished++;

        total_wtime += p[min_idx].wtime;
        total_ttime += p[min_idx].ttime;
    }

    // In Gantt Chart
    printf("\nGantt Chart:\n");
    for(i = 0; i < g_count; i++) {
        if(g_pid[i] == -1) {
            printf("[idle: %d-%d] ", g_start[i], g_end[i]);
        } else {
            printf("[P%d: %d-%d] ", g_pid[i], g_start[i], g_end[i]);
        }
    }
    printf("\n");

    // In bảng kết quả
    printf("\nProcess\tArrival\tBurst\tPriority\tWaiting\tTurnaround\n");
    for(i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t\t%d\t%d\n",
               p[i].pid, p[i].atime, p[i].btime,
               p[i].pri, p[i].wtime, p[i].ttime);
    }

    printf("\nTotal waiting time     : %d", total_wtime);
    printf("\nAverage waiting time   : %.2f", (float)total_wtime / n);
    printf("\nTotal turnaround time  : %d", total_ttime);
    printf("\nAverage turnaround time: %.2f\n", (float)total_ttime / n);

    free(g_pid);
    free(g_start);
    free(g_end);
    free(p);

    return 0;
}
// output example:
// Enter the number of processes: 4
// Enter arrival time, burst time and priority (lower number = higher priority):
// Process 1 (arrival burst priority): 0 4 2
// Process 2 (arrival burst priority): 1 3 1
// Process 3 (arrival burst priority): 2 2 3
// Process 4 (arrival burst priority): 3 1 2

// Gantt Chart:
// [P1: 0-4] [P2: 4-7] [P4: 7-8] [P3: 8-10]

// Process Arrival Burst   Priority        Waiting Turnaround
// P1      0       4       2               0       4
// P2      1       3       1               3       6
// P3      2       2       3               6       8
// P4      3       1       2               4       5

// Total waiting time     : 13
// Average waiting time   : 3.25
// Total turnaround time  : 23
// Average turnaround time: 5.75