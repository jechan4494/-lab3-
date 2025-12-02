#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 9001
#define BUFFER_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];
    socklen_t addr_len = sizeof(client_addr);

    // 1. 소켓 생성
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0) { perror("socket"); exit(1); }

    // 2. 서버 주소 설정
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // 3. 바인드
    if(bind(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind"); exit(1);
    }

    printf("UDP 서버 시작: 포트 %d\n", PORT);

    while(1) {
        // 메시지 수신
        int n = recvfrom(sock, buffer, sizeof(buffer)-1, 0,
                         (struct sockaddr*)&client_addr, &addr_len);
        if(n < 0) { perror("recvfrom"); continue; }

        buffer[n] = '\0';
        printf("받은 메시지: %s\n", buffer);

        // 클라이언트에게 응답
        char response[] = "UDP 서버 응답: 메시지 잘 받음!";
        sendto(sock, response, strlen(response), 0,
               (struct sockaddr*)&client_addr, addr_len);
    }

    close(sock);
    return 0;
}

