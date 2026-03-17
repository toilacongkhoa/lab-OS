#include <stdio.h>
#define MAX 25

int main() {
    int b[MAX], f[MAX], bf[MAX], ff[MAX], frag[MAX];
    // b: lưu kích thước các block bộ nhớ
    // f: lưu kích thước các process 
    // bf: đánh dấu block đã dùng (1) hay chưa (0)
    // ff: lưu kết quả cấp phát, ff[i] = j nghĩa là process i được cấp block j, = 0 nghĩa là chưa được cấp
    // frag: lưu phần nhớ dư sau khi cấp phát
    int nb, nf, i, j, temp;
    // nb: số lượng block
    // nf: số lượng process
    int highest = 0;

    // Khởi tạo mảng bf và ff về 0
    for (i = 0; i < MAX; i++) {
        bf[i] = 0;
        ff[i] = 0;
    }

    printf("\n--- Worst Fit Memory Allocation ---\n");

    printf("Nhap so luong khoi nho (blocks): ");
    scanf("%d", &nb);

    printf("Nhap so luong tien trinh (processes): ");
    scanf("%d", &nf);

    printf("\nNhap kich thuoc tung khoi nho:\n");
    for (i = 1; i <= nb; i++) {
        printf("  Block %d: ", i);
        scanf("%d", &b[i]);
    }

    printf("\nNhap kich thuoc tung tien trinh:\n");
    for (i = 1; i <= nf; i++) {
        printf("  Process %d: ", i);
        scanf("%d", &f[i]);
    }

    // Thuat toan Worst Fit
    for (i = 1; i <= nf; i++) {
        ff[i] = 0;    // Chua duoc cap phat
        highest = 0;  // Reset gia tri lon nhat

        for (j = 1; j <= nb; j++) {
            if (bf[j] != 1) {            // Neu block chua duoc dung
                temp = b[j] - f[i];
                if (temp >= 0) {          // Block du lon
                    if (highest < temp) { // Tim block co phan du lon nhat
                        ff[i] = j;
                        highest = temp;
                    }
                }
            }
        }

        // Sau khi duyet het, cap phat block lon nhat
        if (ff[i] != 0) {
            frag[i] = highest;
            bf[ff[i]] = 1;   // Danh dau block da duoc dung
        }
        highest = 0;         // Reset cho process tiep theo
    }

    // In ket qua
    printf("\n%-12s %-15s %-12s %-12s %-12s\n",
           "Process No", "Process Size", "Block No", "Block Size", "Fragment");
    printf("------------------------------------------------------------\n");

    for (i = 1; i <= nf; i++) {
        if (ff[i] != 0) {
            printf("%-12d %-15d %-12d %-12d %-12d\n",
                   i, f[i], ff[i], b[ff[i]], frag[i]);
        } else {
            printf("%-12d %-15d %-12s %-12s %-12s\n",
                   i, f[i], "N/A", "N/A", "Khong du bo nho");
        }
    }

    return 0;
}
// Nhap so luong khoi nho (blocks): 5
// Nhap so luong tien trinh (processes): 1

// Nhap kich thuoc tung khoi nho:
//   Block 1: 100
//   Block 2: 500
//   Block 3: 200
//   Block 4: 300
//   Block 5: 600

// Nhap kich thuoc tung tien trinh:
//   Process 1: 212

// Process No   Process Size    Block No     Block Size   Fragment
// ------------------------------------------------------------
// 1            212             5            600          388