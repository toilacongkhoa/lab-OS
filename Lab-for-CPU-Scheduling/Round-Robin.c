#include <stdio.h>
#include <stdlib.h>

struct rr {
    int pno;      // mã tiến trình
    int atime;    // thời gian đến
    int btime;    // burst time ban đầu
    int rem;      // burst time còn lại
    int ctime;    // completion time
    int wtime;    // waiting time
    int ttime;    // turnaround time
    int done;     // đã xong chưa
    int inqueue;  // đã vào queue chưa
} p[10];

int main() {
    int n, ts, i;
    int time = 0, finished = 0;
    int twt = 0, totttime = 0;

    printf("\nRound Robin Scheduling (with Arrival Time)\n");
    printf("Enter number of processes: ");
    scanf("%d", &n);

    printf("Enter the time slice: ");
    scanf("%d", &ts);

    printf("Enter arrival time and burst time:\n");
    for(i = 0; i < n; i++) {
        printf("Process %d (arrival burst): ", i + 1);
        scanf("%d %d", &p[i].atime, &p[i].btime);

        p[i].pno = i + 1;
        p[i].rem = p[i].btime;
        p[i].ctime = 0;
        p[i].wtime = 0;
        p[i].ttime = 0;
        p[i].done = 0;
        p[i].inqueue = 0;
    }

    // Queue cho Round Robin
    int q[1000], front = 0, rear = 0;

    // Mảng lưu Gantt Chart
    int g_pid[1000];    // -1 nghĩa là idle
    int g_start[1000];
    int g_end[1000];
    int g_count = 0;

    while(finished < n) {
        // Đưa các process đã tới vào queue
        for(i = 0; i < n; i++) {
            if(!p[i].done && !p[i].inqueue && p[i].atime <= time) {
                q[rear++] = i;
                p[i].inqueue = 1;
            }
        }

        // Nếu queue rỗng -> CPU idle
        if(front == rear) {
            int next_arrival = 999999;
            for(i = 0; i < n; i++) {
                if(!p[i].done && p[i].atime < next_arrival) {
                    next_arrival = p[i].atime;
                }
            }

            // Gộp segment idle
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

        // Lấy process đầu queue ra chạy
        int idx = q[front++];
        p[idx].inqueue = 0;

        int run = (p[idx].rem >= ts) ? ts : p[idx].rem;
        int start = time;
        int end = time + run;

        // Lưu Gantt Chart
        if(g_count > 0 && g_pid[g_count - 1] == p[idx].pno && g_end[g_count - 1] == start) {
            g_end[g_count - 1] = end;
        } else {
            g_pid[g_count] = p[idx].pno;
            g_start[g_count] = start;
            g_end[g_count] = end;
            g_count++;
        }

        time = end;
        p[idx].rem -= run;

        // Trong lúc process này chạy, có process mới tới thì đưa vào queue
        for(i = 0; i < n; i++) {
            if(!p[i].done && !p[i].inqueue && i != idx && p[i].atime <= time) {
                int already_in_queue = 0;
                int j;
                for(j = front; j < rear; j++) {
                    if(q[j] == i) {
                        already_in_queue = 1;
                        break;
                    }
                }
                if(!already_in_queue) {
                    q[rear++] = i;
                    p[i].inqueue = 1;
                }
            }
        }

        // Nếu process chưa xong thì đưa lại vào cuối queue
        if(p[idx].rem > 0) {
            q[rear++] = idx;
            p[idx].inqueue = 1;
        } else {
            p[idx].done = 1;
            p[idx].ctime = time;
            p[idx].ttime = p[idx].ctime - p[idx].atime;
            p[idx].wtime = p[idx].ttime - p[idx].btime;
            finished++;
        }
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
    printf("\nProcess\tArrival\tBurst\tWaiting\tTurnaround\n");
    for(i = 0; i < n; i++) {
        twt += p[i].wtime;
        totttime += p[i].ttime;
        printf("P%d\t%d\t%d\t%d\t%d\n",
               p[i].pno, p[i].atime, p[i].btime, p[i].wtime, p[i].ttime);
    }

    printf("\nTotal waiting time: %d", twt);
    printf("\nAverage waiting time: %.2f", (float)twt / n);
    printf("\nTotal turnaround time: %d", totttime);
    printf("\nAverage turnaround time: %.2f\n", (float)totttime / n);

    return 0;
}
// output example:
// Enter number of processes: 4
// Enter the time slice: 2
// Enter arrival time and burst time:
// Process 1 (arrival burst): 0 5 
// Process 2 (arrival burst): 1 4
// Process 3 (arrival burst): 2 2
// Process 4 (arrival burst): 4 1

// Gantt Chart:
// [P1: 0-2] [P2: 2-4] [P3: 4-6] [P1: 6-8] [P4: 8-9] [P2: 9-11] [P1: 11-12]

// Process Arrival Burst   Waiting Turnaround
// P1      0       5       7       12
// P2      1       4       6       10
// P3      2       2       2       4
// P4      4       1       4       5

// Total waiting time: 19
// Average waiting time: 4.75
// Total turnaround time: 31
// Average turnaround time: 7.75