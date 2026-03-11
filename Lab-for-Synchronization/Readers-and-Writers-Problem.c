#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_READERS 3
#define NUM_WRITERS 2
#define READER_TURNS 4
#define WRITER_TURNS 3

int data = 0;          // dữ liệu dùng chung
int read_count = 0;    // số reader đang đọc

sem_t mutex;           // bảo vệ read_count
sem_t rw_mutex;        // khóa dữ liệu chung giữa reader và writer

void print_status(const char *msg) {
    printf("%s | data=%d | read_count=%d\n", msg, data, read_count);
}

void* reader(void *arg) {
    int id = *(int*)arg;
    int i;

    for(i = 0; i < READER_TURNS; i++) {
        usleep((id * 200 + i * 150) * 1000);  // làm lệch thời điểm để dễ thấy đồng bộ

        printf("Reader %d wants to read\n", id);

        // Entry section
        sem_wait(&mutex);
        read_count++;

        if(read_count == 1) {
            sem_wait(&rw_mutex);   // reader đầu tiên khóa writer
        }

        printf("Reader %d enters reading section\n", id);
        print_status("   [Reader status]");
        sem_post(&mutex);

        // Reading
        printf("Reader %d reads data = %d\n", id, data);
        usleep(400000);   // giả lập thời gian đọc

        // Exit section
        sem_wait(&mutex);
        read_count--;
        printf("Reader %d leaves reading section\n", id);

        if(read_count == 0) {
            sem_post(&rw_mutex);   // reader cuối cùng mở cho writer
        }

        print_status("   [Reader status]");
        sem_post(&mutex);
    }

    pthread_exit(NULL);
}

void* writer(void *arg) {
    int id = *(int*)arg;
    int i;

    for(i = 0; i < WRITER_TURNS; i++) {
        usleep((id * 500 + i * 700) * 1000);  // làm lệch thời điểm để dễ thấy đồng bộ

        printf("Writer %d wants to write\n", id);

        sem_wait(&rw_mutex);   // chiếm quyền ghi độc quyền
        printf("Writer %d enters writing section\n", id);

        data++;
        printf("Writer %d writes data = %d\n", id, data);
        usleep(500000);        // giả lập thời gian ghi

        printf("Writer %d leaves writing section\n", id);
        print_status("   [Writer status]");
        sem_post(&rw_mutex);
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t readers[NUM_READERS], writers[NUM_WRITERS];
    int r_id[NUM_READERS], w_id[NUM_WRITERS];
    int i;

    sem_init(&mutex, 0, 1);
    sem_init(&rw_mutex, 0, 1);

    printf("Readers-Writers Problem Using Semaphores\n");
    printf("Initial data = %d\n\n", data);

    // Tạo reader
    for(i = 0; i < NUM_READERS; i++) {
        r_id[i] = i + 1;
        pthread_create(&readers[i], NULL, reader, &r_id[i]);
    }

    // Tạo writer
    for(i = 0; i < NUM_WRITERS; i++) {
        w_id[i] = i + 1;
        pthread_create(&writers[i], NULL, writer, &w_id[i]);
    }

    // Chờ reader xong
    for(i = 0; i < NUM_READERS; i++) {
        pthread_join(readers[i], NULL);
    }

    // Chờ writer xong
    for(i = 0; i < NUM_WRITERS; i++) {
        pthread_join(writers[i], NULL);
    }

    sem_destroy(&mutex);
    sem_destroy(&rw_mutex);

    printf("\nAll readers and writers finished.\n");
    printf("Final data = %d\n", data);

    return 0;
}