#include <stdio.h>

int main() {
    int frames[20], rs[50];
    int n, f, i, j;
    int pageFaults = 0; // cờ để kiểm tra page hit hay page fault, 1 là chưa có trang, 0 là đã có
    int frameCount = 0;   // So frame dang duoc dung
    int lastUsed[20];     
    // rs: lưu chuỗi trang tham chiếu
    // frames: các frame trong RAM
    // n: số lượng frame
    // lastUsed[j] = thời điểm gần nhất mà trang đang nằm ở frame j được sử dụng

    printf("\n--- LRU Page Replacement Algorithm ---\n");

    printf("Nhap so luong trang trong chuoi: ");
    scanf("%d", &n);

    printf("Nhap chuoi trang:\n");
    for (i = 0; i < n; i++)
        scanf("%d", &rs[i]);

    printf("Nhap so luong frames: ");
    scanf("%d", &f);

    // Khoi tao frames = -1 (trong)
    for (j = 0; j < f; j++) {
        frames[j] = -1;
        lastUsed[j] = 0;
    }

    printf("\n");

    // Xu ly tung trang trong chuoi
    for (i = 0; i < n; i++) {
        int found = 0;

        printf("Trang %d: ", rs[i]);

        // Kiem tra trang da co trong frame chua (Hit)
        for (j = 0; j < f; j++) {
            if (frames[j] == rs[i]) {
                found = 1;
                lastUsed[j] = i; // Cap nhat thoi diem dung gan nhat
                printf("Hit  ");
                break;
            }
        }

        // Page Fault: can nap trang moi
        if (found == 0) {
            pageFaults++;

            // Con frame trong: nap vao truc tiep
            if (frameCount < f) {
                frames[frameCount] = rs[i];
                lastUsed[frameCount] = i;
                frameCount++;
            }
            // Khong con frame trong: thay the trang LRU
            else {
                // Tim frame co lastUsed nho nhat (dung lau nhat)
                int lruPos = 0;
                for (j = 1; j < f; j++) {
                    if (lastUsed[j] < lastUsed[lruPos]) {
                        lruPos = j;
                    }
                }
                printf("(Thay trang %d) ", frames[lruPos]);
                frames[lruPos] = rs[i];
                lastUsed[lruPos] = i;
            }
            printf("Fault");
        }

        // In trang thai cac frame
        printf("\n  Frames: ");
        for (j = 0; j < f; j++) {
            if (frames[j] != -1)
                printf("[%d] ", frames[j]);
            else
                printf("[ ] ");
        }
        printf("\n\n");
    }

    printf("Tong so lan Page Fault = %d\n", pageFaults);

    return 0;
}
// Nhap so luong trang trong chuoi: 8
// Nhap chuoi trang:
// 7 0 1 2 0 3 0 4
// Nhap so luong frames: 3

// Trang 7: Fault
//   Frames: [7] [ ] [ ]

// Trang 0: Fault
//   Frames: [7] [0] [ ]

// Trang 1: Fault
//   Frames: [7] [0] [1]

// Trang 2: (Thay trang 7) Fault
//   Frames: [2] [0] [1]

// Trang 0: Hit
//   Frames: [2] [0] [1]

// Trang 3: (Thay trang 1) Fault
//   Frames: [2] [0] [3]

// Trang 0: Hit
//   Frames: [2] [0] [3]

// Trang 4: (Thay trang 2) Fault
//   Frames: [4] [0] [3]

// Tong so lan Page Fault = 6