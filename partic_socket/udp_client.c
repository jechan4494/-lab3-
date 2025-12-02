#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9001
#define BUFFER_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    socklen_t addr_len = sizeof(server_addr);

    // 1. 소켓 생성
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0) { perror("socket"); exit(1); }

    // 2. 서버 주소 설정
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    // 3. 메시지 전송
    char msg[] = "UDP 클라이언트에서 보낸 메시지입니다.";
    sendto(sock, msg, strlen(msg), 0, (struct sockaddr*)&server_addr, addr_len);

    // 4. 서버 응답 수신
    int n = recvfrom(sock, buffer, sizeof(buffer)-1, 0,
                     (struct sockaddr*)&server_addr, &addr_len);
    if(n < 0) { perror("recvfrom"); close(sock); exit(1); }

    buffer[n] = '\0';
    printf("서버 응답: %s\n", buffer);

    close(sock);
    return 0;
}

