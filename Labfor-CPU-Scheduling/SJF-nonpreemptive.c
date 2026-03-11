#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int pid;    // mã tiến trình
    int atime;  // thời gian đến
    int btime;  // thời gian thực thi
    int wtime;  // thời gian chờ
    int ttime;  // turnaround time
    int done;   // đã hoàn thành chưa
} sp;

int main() {
    int i, n, current_time = 0, finished = 0;
    int total_wtime = 0, total_ttime = 0;

    printf("\nSJF Non-Preemptive Scheduling (with Arrival Time)\n");
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    sp *p = (sp*)malloc(n * sizeof(sp));

    // Có thể có cả đoạn idle nên cấp phát lớn hơn n
    int *gantt_pid   = (int*)malloc(2 * n * sizeof(int));
    int *gantt_start = (int*)malloc(2 * n * sizeof(int));
    int *gantt_end   = (int*)malloc(2 * n * sizeof(int));
    int gantt_count = 0;

    printf("Enter arrival time and burst time:\n");
    for(i = 0; i < n; i++) {
        printf("Process %d (arrival burst): ", i + 1);
        scanf("%d %d", &p[i].atime, &p[i].btime);

        p[i].pid   = i + 1;
        p[i].wtime = 0;
        p[i].ttime = 0;
        p[i].done  = 0;
    }

    while(finished < n) {
        int min_idx = -1;
        int min_burst = 999999;

        // Tìm tiến trình có burst time nhỏ nhất trong số đã đến
        for(i = 0; i < n; i++) {
            if(!p[i].done && p[i].atime <= current_time) {
                if(p[i].btime < min_burst) {
                    min_burst = p[i].btime;
                    min_idx = i;
                }
                // Nếu burst time bằng nhau thì ưu tiên arrival nhỏ hơn
                else if(p[i].btime == min_burst) {
                    if(p[i].atime < p[min_idx].atime) {
                        min_idx = i;
                    }
                }
            }
        }

        // Nếu chưa có tiến trình nào tới thì thêm đoạn idle
        if(min_idx == -1) {
            int next = 999999;
            for(i = 0; i < n; i++) {
                if(!p[i].done && p[i].atime < next) {
                    next = p[i].atime;
                }
            }

            // Lưu đoạn idle vào Gantt Chart
            gantt_pid[gantt_count]   = 0;   // 0 nghĩa là idle
            gantt_start[gantt_count] = current_time;
            gantt_end[gantt_count]   = next;
            gantt_count++;

            current_time = next;
            continue;
        }

        // Lưu lại để in Gantt Chart
        gantt_pid[gantt_count]   = p[min_idx].pid;
        gantt_start[gantt_count] = current_time;

        // Tính waiting time và turnaround time
        p[min_idx].wtime = current_time - p[min_idx].atime;
        current_time += p[min_idx].btime;
        p[min_idx].ttime = current_time - p[min_idx].atime;
        p[min_idx].done = 1;

        gantt_end[gantt_count] = current_time;
        gantt_count++;
        finished++;

        total_wtime += p[min_idx].wtime;
        total_ttime += p[min_idx].ttime;
    }

    // In Gantt Chart
    printf("\nGantt Chart:\n");
    for(i = 0; i < gantt_count; i++) {
        if(gantt_pid[i] == 0)
            printf("[idle: %d-%d] ", gantt_start[i], gantt_end[i]);
        else
            printf("[P%d: %d-%d] ", gantt_pid[i], gantt_start[i], gantt_end[i]);
    }
    printf("\n");

    // In bảng kết quả
    printf("\nProcess\tArrival\tBurst\tWaiting\tTurnaround\n");
    for(i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\n",
               p[i].pid, p[i].atime, p[i].btime, p[i].wtime, p[i].ttime);
    }

    printf("\nTotal waiting time     : %d", total_wtime);
    printf("\nAverage waiting time   : %.2f", (float)total_wtime / n);
    printf("\nTotal turnaround time  : %d", total_ttime);
    printf("\nAverage turnaround time: %.2f\n", (float)total_ttime / n);

    free(gantt_pid);
    free(gantt_start);
    free(gantt_end);
    free(p);

    return 0;
}
// output example:
// Enter the number of processes: 4
// Enter arrival time and burst time:
// Process 1 (arrival burst): 0 6
// Process 2 (arrival burst): 1 4
// Process 3 (arrival burst): 2 2
// Process 4 (arrival burst): 3 1

// Gantt Chart:
// [P1: 0-6] [P4: 6-7] [P3: 7-9] [P2: 9-13]

// Process Arrival Burst   Waiting Turnaround
// P1      0       6       0       6
// P2      1       4       8       12
// P3      2       2       5       7
// P4      3       1       3       4

// Total waiting time     : 16
// Average waiting time   : 4.00
// Total turnaround time  : 29
// Average turnaround time: 7.25