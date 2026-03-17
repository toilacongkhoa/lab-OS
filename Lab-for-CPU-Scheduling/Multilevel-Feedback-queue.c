#include<stdio.h>
#include<stdlib.h>

typedef struct {
    int pid; // mã tiến trình
    int atime;   // arrival time: thời gian đến
    int btime; // burst time ban đầu
    int remaining; // thời gian còn lại chưa chạy xong
    int wtime; // waiting time: thời gian chờ
    int ttime; // turnaround time: thời gian hoàn thành
    int level;   // current queue level (0, 1, 2), -1 chưa vào hệ thống
    int done; // đã hoàn thành chưa
} sp;

int main() {
    int i, j, n;
    int tq0, tq1;
    int time = 0, finished = 0;
    int total_wt = 0, total_tt = 0;
    // tq0: time quantum của Q0
    // tq1: time quantum của Q1
    // time: thời gian hiện tại
    // finished: số process đã xong
    // total_wt: tổng waiting time
    // total_tt: tổng turnaround time

    printf("\n Multilevel Feedback Queue Scheduling\n");
    printf(" Queue 0: Round Robin - highest priority\n");
    printf(" Queue 1: Round Robin - medium priority\n");
    printf(" Queue 2: FCFS        - lowest priority\n\n");

    printf("Enter number of processes: ");
    scanf("%d", &n);
    printf("Enter time quantum for Queue 0: ");
    scanf("%d", &tq0);
    printf("Enter time quantum for Queue 1: ");
    scanf("%d", &tq1);

    sp p[10];
    printf("Enter arrival time and burst time:\n");
    for(i = 0; i < n; i++) {
        printf("Process %d (arrival burst): ", i + 1);
        scanf("%d %d", &p[i].atime, &p[i].btime);
        p[i].pid     = i + 1;
        p[i].remaining = p[i].btime;
        p[i].wtime   = 0;
        p[i].ttime   = 0;
        p[i].level   = -1; // not arrived yet
        p[i].done    = 0;
    }

    printf("\nGantt Chart:\n");

    while(finished < n) {
        // Mọi process mới đến đều bắt đầu ở hàng đợi ưu tiên cao nhất
        for(i = 0; i < n; i++)
            if(!p[i].done && p[i].level == -1 && p[i].atime <= time)
                p[i].level = 0;
            
        int selected = -1; // lưu vị trí của tiến trình được chọn
        int best_level = 3; // lưu mức queue tốt nhất tìm được
        
        // Chọn tiến trình ở mức queue ưu tiên cao nhất có thể chạy
        for(i = 0; i < n; i++) {
            if(!p[i].done && p[i].level >= 0 && p[i].atime <= time) {
                if(p[i].level < best_level) {
                    best_level = p[i].level;
                    selected = i;
                }
            }
        }

        // Nếu chưa có process nào sẵn sàng
        if(selected == -1) {
            int next = 999999;
            for(i = 0; i < n; i++)
                if(!p[i].done && p[i].atime > time && p[i].atime < next)
                    next = p[i].atime;
            printf("[IDLE: %d-%d] ", time, next);
            time = next; //nhảy thời gian đến lúc có process đến tiếp theo
            continue;
        }

        int idx = selected; // inedx của process được chọn chạy
        int run, tq; // run: thời gian process sẽ chạy lượt này, tq: time quantum của queue hiện tại

        if(p[idx].level == 0)      tq = tq0;
        else if(p[idx].level == 1) tq = tq1;
        else                        tq = p[idx].remaining; // FCFS: run to completion

        // trong lúc process hiện tại đang chạy, có process mới nào sắp đến không
        // Nếu có, thì process hiện tại không được chạy hết cả quantum ngay, mà chỉ chạy tới lúc process mới đó xuất hiện để hệ thống xét lại ưu tiên.
        int next_arrival = time + tq;
        for(i = 0; i < n; i++)
            if(!p[i].done && p[i].level == -1 && p[i].atime > time && p[i].atime < next_arrival)
                next_arrival = p[i].atime;

        run = next_arrival - time; // thời gian thực tế process sẽ chạy trong lượt này
        if(run > p[idx].remaining) run = p[idx].remaining;

        //In Gantt chart
        printf("[Q%d-P%d: %d-%d] ", p[idx].level, p[idx].pid, time, time + run);

        //Cộng waiting time cho process khác
        for(i = 0; i < n; i++)
            if(i != idx && !p[i].done && p[i].level >= 0 && p[i].atime <= time)
                p[i].wtime += run;

        time += run; // tăng thời gian hiện tại
        p[idx].remaining -= run; // giảm thời gian còn lại của process

        //Nếu process chạy xong
        if(p[idx].remaining == 0) {
            p[idx].ttime = time;
            p[idx].done  = 1;
            finished++;
        } else if(run == tq) {
            // Nếu chưa xong nhưng dùng hết quantum
            if(p[idx].level < 2)
                p[idx].level++; // xuống queue thấp hơn
        }
        // If run < tq: process chưa dùng hết quantum, bị ngắt, ở nguyên queue cũ, lần sau vẫn chạy quantum đầy đủ
    }

    printf("\n\nProcess\tArrival\tBurst\tQueue\tWaiting Time\tTurnaround Time\n");
    for(i = 0; i < n; i++) {
        total_wt += p[i].wtime;
        total_tt += p[i].ttime - p[i].atime;
        printf("P%d\t%d\t%d\tQ%d\t%d\t\t%d\n",
               p[i].pid, p[i].atime, p[i].btime, p[i].level,
               p[i].wtime, p[i].ttime - p[i].atime);
    }

    printf("\nTotal waiting time     : %d", total_wt);
    printf("\nAverage waiting time   : %.2f", (float)total_wt / n);
    printf("\nTotal turnaround time  : %d", total_tt);
    printf("\nAverage turnaround time: %.2f\n", (float)total_tt / n);

    return 0;
}
// input example:
// Enter number of processes: 3
// Enter time quantum for Queue 0: 4
// Enter time quantum for Queue 1: 8
// Enter arrival time and burst time:
// Process 1 (arrival burst): 0 2
// Process 2 (arrival burst): 1 6
// Process 3 (arrival burst): 2 10

// Gantt Chart:
// [Q0-P1: 0-1] [Q0-P1: 1-2] [Q0-P2: 2-6] [Q0-P3: 6-10] [Q1-P2: 10-12] [Q1-P3: 12-18]      

// Process Arrival Burst   Queue   Waiting Time    Turnaround Time
// P1      0       2       Q0      0               2
// P2      1       6       Q1      5               11
// P3      2       10      Q1      6               16

// Total waiting time     : 11
// Average waiting time   : 3.67
// Total turnaround time  : 29
// Average turnaround time: 9.67