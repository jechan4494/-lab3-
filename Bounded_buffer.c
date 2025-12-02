#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 5
#define NUM_PRODUCERS 3
#define NUM_CONSUMERS 3
#define TOTAL_ITEMS 15

typedef struct {
    int buf[BUFFER_SIZE];
    int in, out, count;

    pthread_mutex_t mutex;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;
} Buffer;

Buffer buffer;
int produced = 0;
int consumed = 0;
int done = 0;

void *producer(void *arg) {
    int id = (intptr_t)arg;

    while (1) {
        pthread_mutex_lock(&buffer.mutex);

        if (produced >= TOTAL_ITEMS) {
            done = 1;
            pthread_cond_broadcast(&buffer.not_empty);
            pthread_mutex_unlock(&buffer.mutex);
            break;
        }

        while (buffer.count == BUFFER_SIZE) {
            pthread_cond_wait(&buffer.not_full, &buffer.mutex);
        }

        int item = rand() % 100;
        buffer.buf[buffer.in] = item;
        buffer.in = (buffer.in + 1) % BUFFER_SIZE;

        produced++;
        buffer.count++;

        printf("[P%d] 생산: %d (총 %d)\n", id, item, produced);

        pthread_cond_signal(&buffer.not_empty);
        pthread_mutex_unlock(&buffer.mutex);

        usleep(50000);
    }
    return NULL;
}

void *consumer(void *arg) {
    int id = (intptr_t)arg;

    while (1) {
        pthread_mutex_lock(&buffer.mutex);

        if (done && buffer.count == 0) {
            pthread_mutex_unlock(&buffer.mutex);
            break;
        }

        while (buffer.count == 0) {
            if (done) {
                pthread_mutex_unlock(&buffer.mutex);
                return NULL;
            }
            pthread_cond_wait(&buffer.not_empty, &buffer.mutex);
        }

        int item = buffer.buf[buffer.out];
        buffer.out = (buffer.out + 1) % BUFFER_SIZE;

        consumed++;
        buffer.count--;

        printf("[C%d] 소비: %d (총 %d)\n", id, item, consumed);

        pthread_cond_signal(&buffer.not_full);
        pthread_mutex_unlock(&buffer.mutex);

        usleep(70000);
    }
    return NULL;
}

int main() {
    pthread_t prod[NUM_PRODUCERS];
    pthread_t cons[NUM_CONSUMERS];

    pthread_mutex_init(&buffer.mutex, NULL);
    pthread_cond_init(&buffer.not_empty, NULL);
    pthread_cond_init(&buffer.not_full, NULL);

    printf("=== Producer %d명, Consumer %d명 시작 ===\n", 
           NUM_PRODUCERS, NUM_CONSUMERS);

    for (int i = 0; i < NUM_PRODUCERS; i++)
        pthread_create(&prod[i], NULL, producer, (void *)(intptr_t)(i + 1));

    for (int i = 0; i < NUM_CONSUMERS; i++)
        pthread_create(&cons[i], NULL, consumer, (void *)(intptr_t)(i + 1));

    for (int i = 0; i < NUM_PRODUCERS; i++)
        pthread_join(prod[i], NULL);

    for (int i = 0; i < NUM_CONSUMERS; i++)
        pthread_join(cons[i], NULL);

    printf("=== 종료: 총 생산 %d, 총 소비 %d ===\n", produced, consumed);

    pthread_mutex_destroy(&buffer.mutex);
    pthread_cond_destroy(&buffer.not_empty);
    pthread_cond_destroy(&buffer.not_full);

    return 0;
}

