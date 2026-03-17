#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int pid;       // mã tiến trình
    int atime;     // arrival time
    int btime;     // burst time ban đầu
    int remaining; // thời gian còn lại
    int pri;       // độ ưu tiên (số nhỏ hơn = ưu tiên cao hơn)
    int ctime;     // completion time
    int wtime;     // waiting time
    int ttime;     // turnaround time
} sp;

int main() {
    int i, n, ts;
    int time = 0, finished = 0;
    int total_wt = 0, total_tt = 0;

    printf("\nPriority with Round Robin Scheduling (with Arrival Time)\n");
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    printf("Enter the time quantum: ");
    scanf("%d", &ts);

    sp p[10];
    int added[10] = {0};   // đã đưa vào ready queue lần đầu chưa

    printf("Enter arrival time, burst time and priority:\n");
    for(i = 0; i < n; i++) {
        printf("Process %d (arrival burst priority): ", i + 1);
        scanf("%d %d %d", &p[i].atime, &p[i].btime, &p[i].pri);

        p[i].pid = i + 1;
        p[i].remaining = p[i].btime;
        p[i].ctime = 0;
        p[i].wtime = 0;
        p[i].ttime = 0;
    }

    // Ready queue
    int q[1000], front = 0, rear = 0;

    // Lưu các đoạn để in Gantt Chart
    int g_pid[1000];   // -1 = idle
    int g_start[1000];
    int g_end[1000];
    int g_count = 0;

    while(finished < n) {
        // Đưa các process đã tới vào queue
        for(i = 0; i < n; i++) {
            if(!added[i] && p[i].atime <= time) {
                q[rear++] = i;
                added[i] = 1;
            }
        }

        // Nếu queue rỗng => CPU idle tới process kế tiếp
        if(front == rear) {
            int next_arrival = 999999;
            for(i = 0; i < n; i++) {
                if(!added[i] && p[i].atime < next_arrival) {
                    next_arrival = p[i].atime;
                }
            }

            if(g_count > 0 && g_pid[g_count - 1] == -1) {
                g_end[g_count - 1] = next_arrival;
            } else {
                g_pid[g_count] = -1;
                g_start[g_count] = time;
                g_end[g_count] = next_arrival;
                g_count++;
            }

            time = next_arrival;
            continue;
        }

        // Tìm process có priority cao nhất trong queue
        int best_pos = -1;
        int best_pri = 999999;

        for(i = front; i < rear; i++) {
            int idx = q[i];
            if(p[idx].remaining > 0 && p[idx].pri < best_pri) {
                best_pri = p[idx].pri;
                best_pos = i;
            }
        }

        int idx = q[best_pos];

        // Xóa process được chọn khỏi queue
        for(i = best_pos; i < rear - 1; i++) {
            q[i] = q[i + 1];
        }
        rear--;

        int run = (p[idx].remaining >= ts) ? ts : p[idx].remaining;
        int start = time;
        int end = time + run;

        // Lưu Gantt Chart
        if(g_count > 0 && g_pid[g_count - 1] == p[idx].pid && g_end[g_count - 1] == start) {
            g_end[g_count - 1] = end;
        } else {
            g_pid[g_count] = p[idx].pid;
            g_start[g_count] = start;
            g_end[g_count] = end;
            g_count++;
        }

        // Chạy process
        time = end;
        p[idx].remaining -= run;

        // Trong lúc chạy, process mới tới thì đưa vào queue
        for(i = 0; i < n; i++) {
            if(!added[i] && p[i].atime <= time) {
                q[rear++] = i;
                added[i] = 1;
            }
        }

        // Nếu chưa xong thì đưa lại cuối queue
        if(p[idx].remaining > 0) {
            q[rear++] = idx;
        } else {
            p[idx].ctime = time;
            finished++;
        }
    }

    // Tính waiting time và turnaround time
    for(i = 0; i < n; i++) {
        p[i].ttime = p[i].ctime - p[i].atime;
        p[i].wtime = p[i].ttime - p[i].btime;
        total_wt += p[i].wtime;
        total_tt += p[i].ttime;
    }

    // In Gantt Chart
    printf("\nGantt Chart:\n");
    for(i = 0; i < g_count; i++) {
        if(g_pid[i] == -1)
            printf("[idle: %d-%d] ", g_start[i], g_end[i]);
        else
            printf("[P%d: %d-%d] ", g_pid[i], g_start[i], g_end[i]);
    }
    printf("\n");

    // In bảng kết quả
    printf("\nProcess\tArrival\tBurst\tPriority\tWaiting\tTurnaround\n");
    for(i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t\t%d\t%d\n",
               p[i].pid, p[i].atime, p[i].btime, p[i].pri, p[i].wtime, p[i].ttime);
    }

    printf("\nTotal waiting time     : %d", total_wt);
    printf("\nAverage waiting time   : %.2f", (float)total_wt / n);
    printf("\nTotal turnaround time  : %d", total_tt);
    printf("\nAverage turnaround time: %.2f\n", (float)total_tt / n);

    return 0;
}
// output example:
// Enter the number of processes: 4
// Enter the time quantum: 2
// Enter arrival time, burst time and priority:
// Process 1 (arrival burst priority): 0 5 2
// Process 2 (arrival burst priority): 0 4 1
// Process 3 (arrival burst priority): 1 3 1
// Process 4 (arrival burst priority): 2 2 3

// Gantt Chart:
// [P2: 0-2] [P3: 2-4] [P2: 4-6] [P3: 6-7] [P1: 7-12] [P4: 12-14]

// Process Arrival Burst   Priority        Waiting Turnaround
// P1      0       5       2               7       12
// P2      0       4       1               2       6
// P3      1       3       1               3       6
// P4      2       2       3               10      12

// Total waiting time     : 22
// Average waiting time   : 5.50
// Total turnaround time  : 36
// Average turnaround time: 9.00