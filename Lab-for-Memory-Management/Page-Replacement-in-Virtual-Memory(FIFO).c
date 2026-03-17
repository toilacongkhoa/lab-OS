#include <stdio.h>

int main() {
    int i, j, k = 0, i1 = 0, m, n, flag;
    int rs[30], p[30];
    // rs: lưu chuỗi trang tham chiếu
    // p: các frame trong RAM
    // n: số lượng frame
    // m: số lượng trang trong chuỗi tham chiếu
    // k: đếm số lần page fault
    // i1: con trỏ FIFO để xác định vị trí thay thế tiếp theo
    // flag: cờ để kiểm tra page hit hay page fault, 1 là chưa có trang, 0 là đã có

    printf("\n--- FIFO Page Replacement Algorithm ---\n");

    printf("Nhap so luong frames: ");
    scanf("%d", &n);

    // Khoi tao tat ca frames = 0
    for (j = 0; j < n; j++)
        p[j] = 0;

    printf("Nhap chuoi trang (ket thuc bang 0):\n");
    i = 0;
    while (1) {
        scanf("%d", &rs[i]);
        if (rs[i] == 0)
            break;
        i++;
    }
    m = i; // Tong so trang trong chuoi

    printf("\n");

    // Xu ly tung trang trong chuoi
    for (i = 0; i < m; i++) {
        flag = 1;

        // Kiem tra trang da co trong frame chua
        for (j = 0; j < n; j++) {
            if (p[j] == rs[i]) {
                printf("Trang %d da co trong bo nho\n", rs[i]);
                flag = 0;
                break;
            }
        }

        // Neu page fault: nap trang moi vao frame
        if (flag == 1) {
            p[i1] = rs[i];  // Ghi de len vi tri FIFO (cu nhat)
            i1++;            // Di chuyen con tro FIFO
            k++;             // Tang so lan page fault
            if (i1 == n)
                i1 = 0;      // Quay vong

            // In trang thai cac frame
            for (j = 0; j < n; j++) {
                printf("Frame %d: %d", j + 1, p[j]);
                if (p[j] == rs[i])
                    printf(" *"); // Danh dau trang vua duoc nap
                printf("\n");
            }
            printf("\n");
        }
    }

    printf("Tong so lan Page Fault = %d\n", k);

    return 0;
}
// Nhap so luong frames: 3
// Nhap chuoi trang (ket thuc bang 0):
// 1 2 3 4 1 2 5 1 2 3 4 5
// 0

// Frame 1: 1 *
// Frame 2: 0
// Frame 3: 0

// Frame 1: 1
// Frame 2: 2 *
// Frame 3: 0

// Frame 1: 1
// Frame 2: 2
// Frame 3: 3 *

// Frame 1: 4 *
// Frame 2: 2
// Frame 3: 3

// Frame 1: 4
// Frame 2: 1 *
// Frame 3: 3

// Frame 1: 4
// Frame 2: 1
// Frame 3: 2 *

// Frame 1: 5 *
// Frame 2: 1
// Frame 3: 2

// Trang 1 da co trong bo nho
// Trang 2 da co trong bo nho
// Frame 1: 5
// Frame 2: 3 *
// Frame 3: 2

// Frame 1: 5
// Frame 2: 3
// Frame 3: 4 *

// Trang 5 da co trong bo nho
// Tong so lan Page Fault = 9