#include <pthread.h>
#include <stdio.h>

void *test_thread (void *arg) {
    printf("Thread: Hello, World!\n"); 
    return arg; 
}

int main() {
    pthread_t tid;
    int status;

    // 쓰레드 생성
    status = pthread_create(&tid, NULL, test_thread,NULL);
    if (status != 0) {
        perror("Create thread"); 
    }

    // 메인 쓰레드만 종료
    pthread_exit(NULL);
}
