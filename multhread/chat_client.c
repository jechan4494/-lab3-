#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 9000
#define BUFFER_SIZE 1024

int sock;

void* recv_msg(void* arg) {
    char buffer[BUFFER_SIZE];
    int n;
    while(1) {
        n = read(sock, buffer, sizeof(buffer)-1);
        if(n <= 0) break;
        buffer[n] = '\0';
        printf("%s", buffer);
    }
    return NULL;
}

int main() {
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    pthread_t tid;

    // 1. 소켓 생성
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0) { perror("socket"); exit(EXIT_FAILURE); }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    // 2. 서버 연결
    if(connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect"); exit(EXIT_FAILURE);
    }

    // 3. 수신 쓰레드 생성
    pthread_create(&tid, NULL, recv_msg, NULL);

    // 4. 메시지 입력 및 전송
    while(1) {
        if(fgets(buffer, sizeof(buffer), stdin) != NULL) {
            write(sock, buffer, strlen(buffer));
        }
    }

    close(sock);
    return 0;
}

