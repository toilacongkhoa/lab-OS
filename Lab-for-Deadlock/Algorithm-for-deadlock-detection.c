#include<stdio.h>
#include<stdlib.h>

int max[100][100]; // max[i][j]: số lượng resource j mà process i có thể yêu cầu tối đa
int alloc[100][100]; // alloc[i][j]: số lượng resource j mà process i đã được cấp
int need[100][100]; // need[i][j]: số lượng resource j mà process i còn cần
int avail[100]; // avail[j]: số lượng resource j còn available
int n, r; // n: số process, r: số loại resource

void input() {
    int i, j;
    printf("Enter the number of processes: ");
    scanf("%d", &n);
    printf("Enter the number of resource instances: ");
    scanf("%d", &r);

    printf("Enter the Max Matrix:\n");
    for(i = 0; i < n; i++)
        for(j = 0; j < r; j++)
            scanf("%d", &max[i][j]);

    printf("Enter the Allocation Matrix:\n");
    for(i = 0; i < n; i++)
        for(j = 0; j < r; j++)
            scanf("%d", &alloc[i][j]);

    printf("Enter the Available Resources:\n");
    for(j = 0; j < r; j++)
        scanf("%d", &avail[j]);
}

void show() {
    int i, j;
    printf("\nProcess\tAllocation\t\tMax\t\tAvailable");
    for(i = 0; i < n; i++) {
        printf("\nP%d\t", i + 1);
        for(j = 0; j < r; j++) printf("%d ", alloc[i][j]);
        printf("\t\t");
        for(j = 0; j < r; j++) printf("%d ", max[i][j]);
        printf("\t\t");
        if(i == 0)
            for(j = 0; j < r; j++) printf("%d ", avail[j]);
    }
}

void cal() {
    int finish[100], dead[100], safe[100];
    // finish[i] = 1 nếu process i đã hoàn thành, 0 nếu chưa
    // dead[i] = i nếu process i bị deadlock, -1 nếu không
    // safe[i] = index của process trong safe sequence
    int flag = 1, k, c1 = 0, i, j;
    // flag: Dùng để kiểm tra trong một lượt quét có tìm được process nào chạy được không
    // c1: Đếm số tiến trình đã hoàn thành.

    for(i = 0; i < n; i++) finish[i] = 0; // khởi tạo finish = 0

    // Calculate need matrix
    for(i = 0; i < n; i++)
        for(j = 0; j < r; j++)
            need[i][j] = max[i][j] - alloc[i][j];

    // Print need matrix
    printf("\n\nNeed Matrix:");
    for(i = 0; i < n; i++) {
        printf("\nP%d\t", i + 1);
        for(j = 0; j < r; j++) printf("%d ", need[i][j]);
    }

    // Banker's algorithm to find safe sequence
    int safe_count = 0; // đếm số process đã hoàn thành, cũng là index để lưu vị trí của process trong safe sequence
    while(flag) {
        flag = 0;
        for(i = 0; i < n; i++) {
            int c = 0;
            for(j = 0; j < r; j++)
                if(finish[i] == 0 && need[i][j] <= avail[j]) c++;
            if(c == r) {
                for(k = 0; k < r; k++)
                    avail[k] += alloc[i][k];
                finish[i] = 1;
                safe[safe_count++] = i;
                flag = 1;
            }
        }
    }

    // đến số tiến trình chưa hoàn thành, tức là bị deadlock
    int dead_count = 0;
    for(i = 0; i < n; i++)
        if(finish[i] == 0)
            dead[dead_count++] = i;

    if(dead_count > 0) {
        printf("\n\nSystem is in DEADLOCK!");
        printf("\nDeadlocked processes: ");
        for(i = 0; i < dead_count; i++)
            printf("P%d ", dead[i] + 1);
    } else {
        printf("\n\nNo Deadlock! System is in Safe State.");
        printf("\nSafe Sequence: ");
        for(i = 0; i < safe_count; i++)
            printf("P%d ", safe[i] + 1);
    }
    printf("\n");
}

int main() {
    printf("\n********** Deadlock Detection Algorithm **********\n");
    input();
    show();
    cal();
    return 0;
}
// output example:
// Enter the number of processes: 3
// Enter the number of resource instances: 3
// Enter the Max Matrix:
// 3 2 2
// 1 2 2
// 1 1 3
// Enter the Allocation Matrix:
// 1 0 0
// 0 1 1
// 1 0 1
// Enter the Available Resources:
// 0 0 0

// Process Allocation              Max             Available
// P1      1 0 0           3 2 2           0 0 0
// P2      0 1 1           1 2 2
// P3      1 0 1           1 1 3

// Need Matrix:
// P1      2 2 2
// P2      1 1 1
// P3      0 1 2

// System is in DEADLOCK!
// Deadlocked processes: P1 P2 P3