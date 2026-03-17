#include <stdio.h>

int main() {
    int f, p;
    int pages[50], frame[10];
    int count[50], time[50];
    int i, j, hit = 0, flag, most, minTime, temp;
    // f: số lượng frame
    // p: số lượng trang trong chuỗi tham chiếu
    // pages: chuỗi trang tham chiếu
    // frame: các frame trong RAM
    // count[i]: tần suất sử dụng của trang i
    // time[i]: thời điểm gần nhất mà trang i được sử dụng
    // hit: đếm số lần page hit
    // flag: cờ để kiểm tra page hit hay page fault, 1 là chưa có trang, 0 là đã có
    // most: trang có tần suất sử dụng cao nhất (để thay thế khi page fault)
    // minTime: thời điểm gần nhất của trang có tần suất cao nhất (để tie-break khi có nhiều trang cùng tần suất cao nhất)

    printf("\n--- MFU Page Replacement Algorithm ---\n");

    printf("Nhap so luong frames: ");
    scanf("%d", &f);

    printf("Nhap so luong trang trong chuoi: ");
    scanf("%d", &p);

    // Khoi tao frames = -1 (trong)
    for (i = 0; i < f; i++)
        frame[i] = -1;

    // Khoi tao count = 0
    for (i = 0; i < 50; i++)
        count[i] = 0;

    printf("Nhap chuoi trang:\n");
    for (i = 0; i < p; i++)
        scanf("%d", &pages[i]);

    printf("\n");

    // Xu ly tung trang trong chuoi
    for (i = 0; i < p; i++) {
        count[pages[i]]++;   // Tang tan suat su dung trang nay
        time[pages[i]] = i;  // Cap nhat thoi diem dung gan nhat
        flag = 1;
        most = frame[0];

        printf("Trang %d: ", pages[i]);

        // Duyet tung frame
        for (j = 0; j < f; j++) {
            // TH1: Frame trong hoac trang da co san
            if (frame[j] == -1 || frame[j] == pages[i]) {
                if (frame[j] != -1) {
                    // Trang da co → Hit
                    hit++;
                    printf("Hit  ");
                }
                flag = 0;
                frame[j] = pages[i];
                break;
            }

            // Tim trang co tan suat CAO NHAT (de thay the sau)
            if (count[most] < count[frame[j]])
                most = frame[j];
        }

        // TH2: Page Fault - can thay the
        if (flag == 1) {
            printf("Fault  ");

            // Neu co nhieu trang cung tan suat → chon trang dung cu nhat (LRU tie-break)
            minTime = 50;
            for (j = 0; j < f; j++) {
                if (count[frame[j]] == count[most] && time[frame[j]] < minTime) {
                    temp = j;
                    minTime = time[frame[j]];
                }
            }

            // Reset count va thay the trang
            count[frame[temp]] = 0;
            frame[temp] = pages[i];
        }

        // In trang thai cac frame
        printf("\n  Frames: ");
        for (j = 0; j < f; j++) {
            if (frame[j] != -1)
                printf("[%d] ", frame[j]);
            else
                printf("[ ] ");
        }

        // In count cua tung frame
        printf("\n  Count:  ");
        for (j = 0; j < f; j++) {
            if (frame[j] != -1)
                printf(" %d  ", count[frame[j]]);
            else
                printf(" 0  ");
        }
        printf("\n\n");
    }

    printf("Tong so lan Page Hit   = %d\n", hit);
    printf("Tong so lan Page Fault = %d\n", p - hit);

    return 0;
}
// Nhap so luong frames: 3
// Nhap so luong trang trong chuoi: 10
// Nhap chuoi trang:
// 1 2 3 4 1 2 5 1 2 3

// Trang 1:
//   Frames: [1] [ ] [ ]
//   Count:   1   0   0

// Trang 2:
//   Frames: [1] [2] [ ]
//   Count:   1   1   0

// Trang 3:
//   Frames: [1] [2] [3]
//   Count:   1   1   1

// Trang 4: Fault
//   Frames: [4] [2] [3]
//   Count:   1   1   1

// Trang 1: Fault
//   Frames: [4] [1] [3]
//   Count:   1   1   1

// Trang 2: Fault
//   Frames: [4] [1] [2]
//   Count:   1   1   1

// Trang 5: Fault
//   Frames: [5] [1] [2]
//   Count:   1   1   1  

// Trang 1: Hit
//   Frames: [5] [1] [2]
//   Count:   1   2   1

// Trang 2: Hit
//   Frames: [5] [1] [2]
//   Count:   1   2   2

// Trang 3: Fault
//   Frames: [5] [3] [2]
//   Count:   1   1   2

// Tong so lan Page Hit   = 2
// Tong so lan Page Fault = 8