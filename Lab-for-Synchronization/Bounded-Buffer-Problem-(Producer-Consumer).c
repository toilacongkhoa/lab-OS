#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 3
#define NUM_PRODUCERS 2
#define NUM_CONSUMERS 2
#define ITEMS_PER_PRODUCER 5

int buffer[BUFFER_SIZE];
int in = 0, out = 0; // vị trí để producer thêm vào và consumer lấy ra
int item_id = 1;

sem_t empty, full, mutex; // semaphore đếm số ô trống, số ô đầy và mutex cho vùng tới hạn

void print_buffer_status() {
    int i;
    printf(" | Buffer: [");
    for (i = 0; i < BUFFER_SIZE; i++) {
        if (buffer[i] == 0)
            printf("_");
        else
            printf("%d", buffer[i]);

        if (i < BUFFER_SIZE - 1) printf(" ");
    }
    printf("]\n");
}

void* producer(void* arg) {
    int id = *(int*)arg; // lấy id của producer từ tham số
    int i;

    for (i = 0; i < ITEMS_PER_PRODUCER; i++) {
        sem_wait(&empty);   // chờ nếu buffer đầy
        sem_wait(&mutex);   // vào vùng tới hạn

        int item = item_id++;
        buffer[in] = item;
        printf("Producer %d produces item %d at position %d", id, item, in);
        in = (in + 1) % BUFFER_SIZE;

        print_buffer_status();

        sem_post(&mutex);   // rời vùng tới hạn
        sem_post(&full);    // báo có thêm 1 ô đầy

        sleep(1); // làm chậm để dễ quan sát
    }

    pthread_exit(NULL);
}

void* consumer(void* arg) {
    int id = *(int*)arg;
    int i;

    for (i = 0; i < ITEMS_PER_PRODUCER; i++) {
        sem_wait(&full);    // chờ nếu buffer rỗng
        sem_wait(&mutex);   // vào vùng tới hạn

        int item = buffer[out];
        buffer[out] = 0;
        printf("Consumer %d consumes item %d from position %d", id, item, out);
        out = (out + 1) % BUFFER_SIZE;

        print_buffer_status();

        sem_post(&mutex);   // rời vùng tới hạn
        sem_post(&empty);   // báo có thêm 1 ô trống

        sleep(2); // làm chậm để dễ quan sát
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t prod[NUM_PRODUCERS], cons[NUM_CONSUMERS];
    int prod_id[NUM_PRODUCERS], cons_id[NUM_CONSUMERS];
    int i;

    for (i = 0; i < BUFFER_SIZE; i++)
        buffer[i] = 0;

    sem_init(&empty, 0, BUFFER_SIZE); // ban đầu có 3 ô trống
    sem_init(&full, 0, 0);            // ban đầu chưa có ô nào đầy
    sem_init(&mutex, 0, 1);           // mutex = 1, vùng tới hạn đang rảnh

    printf("Producer Consumer Problem Using Semaphores\n");
    printf("Buffer size = %d\n\n", BUFFER_SIZE);

    // Tạo producer
    for (i = 0; i < NUM_PRODUCERS; i++) {
        prod_id[i] = i + 1;
        pthread_create(&prod[i], NULL, producer, &prod_id[i]);
    }

    // Tạo consumer
    for (i = 0; i < NUM_CONSUMERS; i++) {
        cons_id[i] = i + 1;
        pthread_create(&cons[i], NULL, consumer, &cons_id[i]);
    }

    // Chờ các thread chạy xong
    for (i = 0; i < NUM_PRODUCERS; i++)
        pthread_join(prod[i], NULL);

    for (i = 0; i < NUM_CONSUMERS; i++)
        pthread_join(cons[i], NULL);

    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&mutex);

    printf("\nAll processes finished.\n");
    return 0;
}