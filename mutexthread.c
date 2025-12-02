#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define NUM_THREADS 3
pthread_mutex_t mutex;
int sum = 0; // 공유 변수

void *mutex_thread(void *arg) {
    intptr_t value = (intptr_t)arg;
    
    // 임계 영역 시작: 뮤텍스 잠금
    pthread_mutex_lock(&mutex); 
    
    // 공유 자원(sum) 변경
    sum += value;
    printf("쓰레드: 추가 %ld. 합 : %d\n", value, sum);
    
    // 임계 영역 종료: 뮤텍스 해제
    pthread_mutex_unlock(&mutex); 
    
    return arg; 
}

int main(int argc, char *argv[]) {
    pthread_t tid[NUM_THREADS];
    int status;
    int input_args[] = {10, 20, 30}; 

    // 1. 뮤텍스 초기화
    pthread_mutex_init(&mutex, NULL);
    
    // 2. 쓰레드 생성 및 인자 전달
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&tid[i], NULL, 
                       mutex_thread, 
                       (void*)(intptr_t)input_args[i]);
    }
    
    // 3. 모든 쓰레드 종료 대기 (pthread_join)
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(tid[i], NULL); 
    }

    // 4. 뮤텍스 파괴
    status = pthread_mutex_destroy(&mutex); 
    if (status != 0) perror("Destroy mutex");

    printf("최종 합 :  %d\n", sum); 
    return 0;
}
