#include <stdio.h>
#include <stdlib.h>

int main() {
    int i, n, time, total_wt = 0, total_tt = 0;

    printf("\nPriority Preemptive Scheduling (with Arrival Time)\n");
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    int pid[n], atime[n], btime[n], pri[n], remaining[n], wtime[n], ttime[n], done[n];

    int sum_burst = 0, max_atime = 0;

    printf("Enter arrival time, burst time and priority (lower number = higher priority):\n");
    for(i = 0; i < n; i++) {
        printf("Process %d (arrival burst priority): ", i + 1);
        scanf("%d %d %d", &atime[i], &btime[i], &pri[i]);

        pid[i] = i + 1;
        remaining[i] = btime[i];
        wtime[i] = 0;
        ttime[i] = 0;
        done[i] = 0;

        sum_burst += btime[i];
        if(atime[i] > max_atime) max_atime = atime[i];
    }

    /* Lưu các đoạn của Gantt Chart
       seg_pid = -1 nghĩa là idle
    */
    int max_segments = sum_burst + max_atime + 5;
    int seg_pid[max_segments];
    int seg_start[max_segments];
    int seg_end[max_segments];
    int seg_count = 0;

    int finished = 0;
    int current_seg_pid = -999;   // giá trị đặc biệt: chưa có segment nào
    int current_seg_start = 0;

    for(time = 0; finished < n; time++) {
        int min_idx = -1;

        // Tìm tiến trình có priority cao nhất (số nhỏ hơn là ưu tiên cao hơn)
        for(i = 0; i < n; i++) {
            if(!done[i] && atime[i] <= time && remaining[i] > 0) {
                if(min_idx == -1) {
                    min_idx = i;
                }
                else if(pri[i] < pri[min_idx]) {
                    min_idx = i;
                }
                else if(pri[i] == pri[min_idx]) {
                    // Nếu cùng priority thì ưu tiên arrival nhỏ hơn
                    if(atime[i] < atime[min_idx]) {
                        min_idx = i;
                    }
                    // Nếu arrival cũng bằng nhau thì ưu tiên PID nhỏ hơn
                    else if(atime[i] == atime[min_idx] && pid[i] < pid[min_idx]) {
                        min_idx = i;
                    }
                }
            }
        }

        // Xác định trạng thái đang chạy: process nào hoặc idle
        int running_pid;
        if(min_idx == -1) running_pid = -1;   // idle
        else running_pid = pid[min_idx];

        // Nếu đổi trạng thái thì đóng segment cũ, mở segment mới
        if(current_seg_pid == -999) {
            current_seg_pid = running_pid;
            current_seg_start = time;
        }
        else if(running_pid != current_seg_pid) {
            seg_pid[seg_count] = current_seg_pid;
            seg_start[seg_count] = current_seg_start;
            seg_end[seg_count] = time;
            seg_count++;

            current_seg_pid = running_pid;
            current_seg_start = time;
        }

        // Nếu CPU idle thì sang đơn vị thời gian tiếp theo
        if(min_idx == -1) {
            continue;
        }

        // Các tiến trình khác đã đến nhưng chưa chạy thì phải chờ thêm 1 đơn vị
        for(i = 0; i < n; i++) {
            if(i != min_idx && !done[i] && atime[i] <= time && remaining[i] > 0) {
                wtime[i]++;
            }
        }

        // Cho tiến trình được chọn chạy 1 đơn vị thời gian
        remaining[min_idx]--;

        // Nếu tiến trình vừa hoàn thành
        if(remaining[min_idx] == 0) {
            ttime[min_idx] = (time + 1) - atime[min_idx];
            done[min_idx] = 1;
            finished++;
        }
    }

    // Đóng segment cuối cùng
    seg_pid[seg_count] = current_seg_pid;
    seg_start[seg_count] = current_seg_start;
    seg_end[seg_count] = time;
    seg_count++;

    // In Gantt Chart đúng format mới
    printf("\nGantt Chart:\n");
    for(i = 0; i < seg_count; i++) {
        if(seg_pid[i] == -1)
            printf("[idle: %d-%d] ", seg_start[i], seg_end[i]);
        else
            printf("[P%d: %d-%d] ", seg_pid[i], seg_start[i], seg_end[i]);
    }
    printf("\n");

    // In bảng kết quả
    printf("\nProcess\tArrival\tBurst\tPriority\tWaiting\tTurnaround\n");
    for(i = 0; i < n; i++) {
        total_wt += wtime[i];
        total_tt += ttime[i];
        printf("P%d\t%d\t%d\t%d\t\t%d\t%d\n",
               pid[i], atime[i], btime[i], pri[i], wtime[i], ttime[i]);
    }

    printf("\nTotal waiting time     : %d", total_wt);
    printf("\nAverage waiting time   : %.2f", (float)total_wt / n);
    printf("\nTotal turnaround time  : %d", total_tt);
    printf("\nAverage turnaround time: %.2f\n", (float)total_tt / n);

    return 0;
}
// output example:
// Enter the number of processes: 4
// Enter arrival time, burst time and priority (lower number = higher priority):
// Process 1 (arrival burst priority): 0 7 3
// Process 2 (arrival burst priority): 2 4 1
// Process 3 (arrival burst priority): 4 1 2
// Process 4 (arrival burst priority): 5 4 2

// Gantt Chart:
// [P1: 0-2] [P2: 2-6] [P3: 6-7] [P4: 7-11] [P1: 11-16]        

// Process Arrival Burst   Priority        Waiting Turnaround  
// P1      0       7       3               9       16
// P2      2       4       1               0       4
// P3      4       1       2               2       3
// P4      5       4       2               2       6

// Total waiting time     : 13
// Average waiting time   : 3.25
// Total turnaround time  : 29
// Average turnaround time: 7.25