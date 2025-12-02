#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> // for intptr_t

void *join_thread(void *arg) {
    intptr_t ret_val = (intptr_t)arg;
    printf("Joinable Thread: 종료하며 %ld를 반환합니다.\n", ret_val);
    pthread_exit((void*)ret_val); // 반환 값 설정 
}

void *detach_thread(void *arg) {
    printf("Detached Thread: 실행 후 자원을 즉시 해제합니다.\n");
    pthread_detach(pthread_self()); // 스스로를 분리 
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t tid_join, tid_detach;
    void *result;
    int status;
    int input_val = 42; // 전달 및 반환할 값

    // 1. Joinable 쓰레드 생성
    pthread_create(&tid_join, NULL, join_thread, (void*)(intptr_t)input_val);
    
    // 2. Detached 쓰레드 생성
    pthread_create(&tid_detach, NULL, detach_thread, NULL);

    // Joinable 쓰레드의 종료 대기 및 반환 값 획득     status = pthread_join(tid_join, &result)
    if (status == 0) {
        printf("Main: Join 성공. 반환 값: %ld\n", (intptr_t)result);
    } else {
        perror("Main: Join 실패");
    }


    return 0;
}
