#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *attr_thread(void *arg) {
    printf("Attribute Thread: 실행 완료.\n");
    return NULL;
}

int main() {
    pthread_attr_t attr; 

    pthread_t tid;
    int status;
    
    // 1. 속성 객체 초기화
    pthread_attr_init(&attr);

    // 2. 속성 설정: 분리 상태(Detached)로 설정
    status = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    if (status != 0) {
        perror("Setdetachstate 실패");
        exit(1);
    }

    // 3. 속성 적용하여 쓰레드 생성
    pthread_create(&tid, &attr, attr_thread, NULL); 

    // 4. 속성 객체 파괴
    pthread_attr_destroy(&attr); 

    printf("Main: Detached 쓰레드 생성 완료. join 시도하지 않습니다.\n");
    sleep(2); // 쓰레드가 실행될 시간을 줍니다.

    return 0;
}
