#include <stdio.h>
#include <stdlib.h>

struct fcfs {
    int pid;    // mã tiến trình
    int btime;  // burst time
    int wtime;  // waiting time
    int ttime;  // turnaround time
    int atime;  // arrival time
} p[10];

int main() {
    int i, j, n;
    int total_wtime = 0, total_ttime = 0;
    int current_time = 0;

    printf("\nFCFS Scheduling\n");
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    // Nhập arrival time và burst time
    for (i = 0; i < n; i++) {
        p[i].pid = i + 1;
        printf("Process %d (arrival burst): ", i + 1);
        scanf("%d %d", &p[i].atime, &p[i].btime);
    }

    // Sắp xếp theo arrival time
    struct fcfs tmp;
    for (i = 0; i < n - 1; i++) {
        for (j = i + 1; j < n; j++) {
            if (p[i].atime > p[j].atime) {
                tmp = p[i];
                p[i] = p[j];
                p[j] = tmp;
            }
        }
    }

    // Tính waiting time và turnaround time
    for (i = 0; i < n; i++) {
        int start_time;
        int finish_time;

        if (current_time < p[i].atime)
            current_time = p[i].atime; // CPU rảnh đến khi tiến trình này tới

        start_time = current_time;
        p[i].wtime = start_time - p[i].atime; // đợi = bắt đầu - tới
        finish_time = start_time + p[i].btime; 
        p[i].ttime = finish_time - p[i].atime; // hoàn thành = kết thúc - tới

        total_wtime += p[i].wtime;
        total_ttime += p[i].ttime;

        current_time = finish_time;
    }

    // In Gantt chart
    printf("\nGantt Chart:\n");
    current_time = 0; // Reset lại thời gian hiện tại
    for (i = 0; i < n; i++) {
        if (current_time < p[i].atime) {
            printf("[idle: %d-%d] ", current_time, p[i].atime);
            current_time = p[i].atime;
        }

        printf("[P%d: %d-%d] ", p[i].pid, current_time, current_time + p[i].btime);
        current_time += p[i].btime;
    }
    printf("\n");

    // In bảng kết quả
    printf("\nProcess\tArrival\tBurst Time\tWaiting Time\tTurnaround Time\n");
    for (i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t\t%d\t\t%d\n",
               p[i].pid, p[i].atime, p[i].btime, p[i].wtime, p[i].ttime);
    }

    // In tổng và trung bình
    printf("\nTotal waiting time: %d", total_wtime);
    printf("\nAverage waiting time: %.2f", (float) total_wtime / n);
    printf("\nTotal turnaround time: %d", total_ttime);
    printf("\nAverage turnaround time: %.2f\n", (float) total_ttime / n);

    return 0;
}
// output example:
// Enter the number of processes: 4
// Process 1 (arrival burst): 0 5
// Process 2 (arrival burst): 2 3
// Process 3 (arrival burst): 4 1
// Process 4 (arrival burst): 6 2

// Gantt Chart:
// [P1: 0-5] [P2: 5-8] [P3: 8-9] [P4: 9-11]

// Process Arrival Burst Time      Waiting Time    Turnaround Time
// P1      0       5               0               5
// P2      2       3               3               6
// P3      4       1               4               5
// P4      6       2               3               5

// Total waiting time: 10
// Average waiting time: 2.50
// Total turnaround time: 21
// Average turnaround time: 5.25