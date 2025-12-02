#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_ITER 5
#define PARENT 0
#define CHILD 1

int turn = PARENT;
pthread_mutex_t mutex;
pthread_cond_t cond;

void *child_func(void *arg) {
    for (int i = 0; i < MAX_ITER; i++) {
        pthread_mutex_lock(&mutex);
        while (turn != CHILD)
            pthread_cond_wait(&cond, &mutex);

        printf("자식: hello parent\n");
        sleep(1);

        turn = PARENT;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    pthread_t tid;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    pthread_create(&tid, NULL, child_func, NULL);

    for (int i = 0; i < MAX_ITER; i++) {
        pthread_mutex_lock(&mutex);
        while (turn != PARENT)
            pthread_cond_wait(&cond, &mutex);

        printf("부모: hello child\n");
        sleep(1);

        turn = CHILD;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }

    pthread_join(tid, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    printf("--- 교대 출력 완료 (%d회) ---\n", MAX_ITER);
    return 0;
}

