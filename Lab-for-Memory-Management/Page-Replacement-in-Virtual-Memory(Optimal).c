#include <stdio.h>

int main() {
    int i, j, k = 0, m, n;
    int rs[30], p[30];
    int flag, found, farthest, pos, maxDist, dist;
    // rs: lưu chuỗi trang tham chiếu
    // p: các frame trong RAM
    // n: số lượng frame
    // m: số lượng trang trong chuỗi tham chiếu
    // k: đếm số lần page fault
    // flag: cờ để kiểm tra page hit hay page fault, 1 là chưa có trang, 0 là đã có
    // found: cờ để kiểm tra nếu còn frame trống
    // dist: khoảng cách từ trang hiện tại đến lần xuất hiện tiếp theo của trang trong frame
    // farthest: khoảng cách lớn nhất tìm được (để xác định trang sẽ bị thay thế)
    // pos: vị trí của trang sẽ bị thay thế trong frame

    printf("\n--- Optimal Page Replacement Algorithm ---\n");

    printf("Nhap so luong frames: ");
    scanf("%d", &n);

    // Khoi tao tat ca frames = -1 (trong)
    for (j = 0; j < n; j++)
        p[j] = -1;

    printf("Nhap so luong trang trong chuoi: ");
    scanf("%d", &m);

    printf("Nhap chuoi trang:\n");
    for (i = 0; i < m; i++)
        scanf("%d", &rs[i]);

    printf("\n");

    // Xu ly tung trang trong chuoi
    for (i = 0; i < m; i++) {
        printf("Trang %d: ", rs[i]);

        // Kiem tra trang da co trong frame chua (page hit)
        flag = 0;
        for (j = 0; j < n; j++) {
            if (p[j] == rs[i]) {
                printf("da co trong bo nho (Hit)\n");
                flag = 1;
                break;
            }
        }

        if (flag == 1) {
            // In trang thai frame
            printf("  Frames: ");
            for (j = 0; j < n; j++) {
                if (p[j] != -1)
                    printf("[%d] ", p[j]);
                else
                    printf("[ ] ");
            }
            printf("\n\n");
            continue;
        }

        // Page Fault: can nap trang moi
        k++;

        // Kiem tra con frame trong
        found = 0;
        for (j = 0; j < n; j++) {
            if (p[j] == -1) {
                p[j] = rs[i]; // Nap vao frame trong
                found = 1;
                break;
            }
        }

        // Neu khong con frame trong: tim trang se dung xa nhat trong tuong lai
        if (found == 0) {
            farthest = -1;
            pos = 0;

            for (j = 0; j < n; j++) {
                // Tim vi tri tiep theo trang p[j] xuat hien trong chuoi
                dist = -1; // -1 nghia la khong xuat hien nua
                int l;
                for (l = i + 1; l < m; l++) {
                    if (rs[l] == p[j]) {
                        dist = l;
                        break;
                    }
                }

                // Neu trang nay khong xuat hien nua → chon ngay de thay the
                if (dist == -1) {
                    pos = j;
                    farthest = -1;
                    break;
                }

                // Chon trang co lan dung tiep theo xa nhat
                if (dist > farthest) {
                    farthest = dist;
                    pos = j;
                }
            }

            printf("Thay trang %d bang trang %d\n", p[pos], rs[i]);
            p[pos] = rs[i]; // Thay the trang tai vi tri pos
        }

        // In trang thai frame sau khi xu ly
        printf("  Frames: ");
        for (j = 0; j < n; j++) {
            if (p[j] != -1)
                printf("[%d] ", p[j]);
            else
                printf("[ ] ");
        }
        printf("  << Page Fault >>\n\n");
    }

    printf("Tong so lan Page Fault = %d\n", k);

    return 0;
}
// Nhap so luong frames: 3
// Nhap so luong trang trong chuoi: 13
// Nhap chuoi trang:
// 7 0 1 2 0 3 0 4 2 3 0 3 2

// Trang 7:   Frames: [7] [ ] [ ]   << Page Fault >>

// Trang 0:   Frames: [7] [0] [ ]   << Page Fault >>

// Trang 1:   Frames: [7] [0] [1]   << Page Fault >>

// Trang 2: Thay trang 7 bang trang 2
//   Frames: [2] [0] [1]   << Page Fault >>

// Trang 0: da co trong bo nho (Hit)
//   Frames: [2] [0] [1]

// Trang 3: Thay trang 1 bang trang 3
//   Frames: [2] [0] [3]   << Page Fault >>

// Trang 0: da co trong bo nho (Hit)
//   Frames: [2] [0] [3]

// Trang 4: Thay trang 0 bang trang 4
//   Frames: [2] [4] [3]   << Page Fault >>

// Trang 2: da co trong bo nho (Hit)
//   Frames: [2] [4] [3]

// Trang 3: da co trong bo nho (Hit)
//   Frames: [2] [4] [3] 

// Trang 0: Thay trang 4 bang trang 0
//   Frames: [2] [0] [3]   << Page Fault >>

// Trang 3: da co trong bo nho (Hit)
//   Frames: [2] [0] [3]

// Trang 2: da co trong bo nho (Hit)
//   Frames: [2] [0] [3]

// Tong so lan Page Fault = 7