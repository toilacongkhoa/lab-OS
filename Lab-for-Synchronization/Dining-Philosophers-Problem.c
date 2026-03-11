#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 5
#define THINKING 0
#define HUNGRY   1
#define EATING   2
#define ROUNDS   3   // mỗi philosopher ăn 3 lần

int state[N];
sem_t mutex;       // tại 1 thời điểm  chỉ 1 thread đc đổi trạng thái
sem_t self[N];     // semaphore riêng cho từng philosopher
char *state_name[] = {"THINKING", "HUNGRY", "EATING"};

void print_status() {
    int i;
    printf("   [Status] ");
    for(i = 0; i < N; i++) {
        printf("P%d:%s  ", i, state_name[state[i]]);
    }
    printf("\n");
}

void test(int i) {
    int left  = (i + N - 1) % N;
    int right = (i + 1) % N;

    if(state[i] == HUNGRY &&
       state[left] != EATING &&
       state[right] != EATING) {
        state[i] = EATING;
        printf("Philosopher %d picks up chopsticks %d and %d -> EATING\n",
               i, left, i);
        sem_post(&self[i]);   // cho philosopher i được ăn
    }
}

void pickup(int i) {
    sem_wait(&mutex);

    state[i] = HUNGRY;
    printf("Philosopher %d is HUNGRY\n", i);
    test(i);
    print_status();

    sem_post(&mutex);

    sem_wait(&self[i]);   // nếu chưa được ăn thì chờ ở đây
}

void putdown(int i) {
    sem_wait(&mutex);

    state[i] = THINKING;
    printf("Philosopher %d puts down chopsticks -> THINKING\n", i);

    // ăn xong thì kiểm tra 2 hàng xóm có được ăn không
    test((i + N - 1) % N);
    test((i + 1) % N);

    print_status();
    sem_post(&mutex);
}

void* philosopher(void *arg) {
    int id = *(int*)arg;
    int round;

    for(round = 1; round <= ROUNDS; round++) {
        printf("Philosopher %d is THINKING (round %d)\n", id, round);
        usleep((200 + id * 150) * 1000);  // ngủ khác nhau để dễ thấy đồng bộ

        pickup(id);

        printf("Philosopher %d is EATING (round %d)\n", id, round);
        usleep((300 + id * 120) * 1000);

        putdown(id);

        usleep(150000);
    }

    printf("Philosopher %d finished all rounds.\n", id);
    pthread_exit(NULL);
}

int main() {
    pthread_t phil[N];
    int id[N];
    int i;

    // Khởi tạo
    sem_init(&mutex, 0, 1);
    for(i = 0; i < N; i++) {
        state[i] = THINKING;
        sem_init(&self[i], 0, 0);
    }

    printf("Dining Philosophers Problem (Auto Sync Demo)\n");
    printf("Each philosopher runs concurrently.\n\n");
    print_status();
    printf("\n");

    // Tạo 5 philosopher chạy song song
    for(i = 0; i < N; i++) {
        id[i] = i;
        pthread_create(&phil[i], NULL, philosopher, &id[i]);
    }

    // Chờ tất cả chạy xong
    for(i = 0; i < N; i++) {
        pthread_join(phil[i], NULL);
    }

    // Giải phóng semaphore
    sem_destroy(&mutex);
    for(i = 0; i < N; i++) {
        sem_destroy(&self[i]);
    }

    printf("\nAll philosophers finished.\n");
    return 0;
}