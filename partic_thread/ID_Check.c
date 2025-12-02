#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void *id_check_thread(void *arg) {
    long thread_num = (long)arg;
    pthread_t self_id = pthread_self();
    
    printf("Thread %ld (ID: %lu): 값을 전달합니다.\n", 
           thread_num, (unsigned long)self_id);
    
    return NULL;
}

int main() {
    pthread_t tid1, tid2;
    long arg1 = 1, arg2 = 2;
    
    pthread_create(&tid1, NULL, id_check_thread, (void *)arg1); // 인자 전달 
    pthread_create(&tid2, NULL, id_check_thread, (void *)arg2); 

    // 생성된 두 쓰레드의 ID 비교 합니다
    if (pthread_equal(tid1, tid2)) { 
        printf("메인: 쓰레드 1번과 쓰레드 2번의 ID가 같습니다.\n");
    } else {
        printf("메인: 쓰레드 1번과 쓰레드 2번의 ID가 다릅니다.\n");
    }
    
    // 종료를 기다립니다
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    return 0;
}
