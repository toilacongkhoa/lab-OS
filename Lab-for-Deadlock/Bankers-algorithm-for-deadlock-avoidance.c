#include<stdio.h>

int max[100][100]; // max[i][j]: số lượng resource j mà process i có thể yêu cầu tối đa
int alloc[100][100]; // alloc[i][j]: số lượng resource j mà process i đã được cấp
int need[100][100]; // need[i][j]: số lượng resource j mà process i còn cần
int avail[100]; // avail[j]: số lượng resource j còn available
int n, r;
// n: số process, r: số loại resource

void input() {
    int i, j;
    printf("Enter the number of processes: ");
    scanf("%d", &n);
    printf("Enter the number of resource instances: ");
    scanf("%d", &r);

    printf("Enter the Max Matrix (%d x %d):\n", n, r);
    for(i = 0; i < n; i++)
        for(j = 0; j < r; j++)
            scanf("%d", &max[i][j]);

    printf("Enter the Allocation Matrix (%d x %d):\n", n, r);
    for(i = 0; i < n; i++)
        for(j = 0; j < r; j++)
            scanf("%d", &alloc[i][j]);

    printf("Enter the Available Resources (%d values):\n", r);
    for(j = 0; j < r; j++)
        scanf("%d", &avail[j]);
}

void show() {
    int i, j;
    printf("\nProcess\t Allocation\t Max\t\t Need\t\t Available");
    for(i = 0; i < n; i++) {
        printf("\nP%d\t ", i + 1);
        for(j = 0; j < r; j++) printf("%d ", alloc[i][j]);
        printf("\t\t");
        for(j = 0; j < r; j++) printf("%d ", max[i][j]);
        printf("\t\t");
        for(j = 0; j < r; j++) printf("%d ", need[i][j]);
        if(i == 0) {
            printf("\t\t");
            for(j = 0; j < r; j++) printf("%d ", avail[j]);
        }
    }
    printf("\n");
}

void cal() {
    int finish[100] = {0}; // finish[i] = 1 nếu process i đã hoàn thành, 0 nếu chưa
    int safe[100]; // lưu thứ tự của các process trong safe sequence
    int flag, i, j, k, c1 = 0, idx = 0;
    // flag: Dùng để kiểm tra trong một lượt quét có tìm được process nào chạy được không
    // c1: Đếm số tiến trình đã hoàn thành.
    // idx: index để lưu vị trí của process trong safe sequence

    // tính need matrix: need[i][j] = max[i][j] - alloc[i][j]
    for(i = 0; i < n; i++)
        for(j = 0; j < r; j++)
            need[i][j] = max[i][j] - alloc[i][j];

    show();

    printf("\nSafe Sequence: ");

    flag = 1; // để có thể vào vòng lặp
    while(flag) {
        flag = 0; // nếu không tìm được process nào chạy được thì flag vẫn = 0 và thoát vòng lặp
        for(i = 0; i < n; i++) {
            if(finish[i] == 0) { // nếu process i chưa hoàn thành
                int c = 0; // có bao nhiêu tài nguyên thỏa điều kiện
                for(j = 0; j < r; j++)
                    if(need[i][j] <= avail[j]) c++;

                if(c == r) {
                    // Process can finish - release its resources
                    for(k = 0; k < r; k++)
                        avail[k] += alloc[i][k];
                    finish[i] = 1;
                    safe[idx++] = i + 1;
                    flag = 1;
                }
            }
        }
    }

    // Count finished processes
    for(i = 0; i < n; i++)
        if(finish[i] == 1) c1++;

    // Print safe sequence
    for(i = 0; i < idx; i++)
        printf("P%d -> ", safe[i]);
    printf("\b\b\b\b   ");

    if(c1 == n)
        printf("\n\nThe system is in SAFE STATE");
    else {
        printf("\n\nProcess(es) in DEADLOCK: ");
        for(i = 0; i < n; i++)
            if(finish[i] == 0) printf("P%d ", i + 1);
        printf("\nSystem is in UNSAFE STATE");
    }
    printf("\n");
}

int main() {
    printf("********** Banker's Algorithm **********\n");
    input();
    cal();
    return 0;
}
// output example:
// Enter the number of processes: 5
// Enter the number of resource instances: 3
// Enter the Max Matrix (5 x 3):
// 7 5 3
// 3 2 2
// 9 0 2
// 2 2 2
// 4 3 3
// Enter the Allocation Matrix (5 x 3):
// 0 1 0
// 2 0 0
// 3 0 2
// 2 1 1
// 0 0 2
// Enter the Available Resources (3 values):
// 3 3 2

// Process  Allocation      Max             Need            Available
// P1       0 1 0          7 5 3           7 4 3           3 3 2
// P2       2 0 0          3 2 2           1 2 2
// P3       3 0 2          9 0 2           6 0 0
// P4       2 1 1          2 2 2           0 1 1
// P5       0 0 2          4 3 3           4 3 1 

// Safe Sequence: P2 -> P4 -> P5 -> P1 -> P3

// The system is in SAFE STATE