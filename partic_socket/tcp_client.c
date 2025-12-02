#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9000
#define BUFFER_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in addr;
    char buffer[BUFFER_SIZE];

    // 1. 소켓 생성
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0) { perror("socket"); exit(1); }

    // 2. 서버 주소 설정
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    // 3. 서버 연결
    if(connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("connect"); exit(1);
    }

    // 4. 메시지 전송
    char msg[] = "클라이언트에서 보낸 메시지입니다.";
    write(sock, msg, strlen(msg));

    // 5. 서버 응답 읽기
    int n = read(sock, buffer, sizeof(buffer)-1);
    if(n < 0) { perror("read"); close(sock); exit(1); }

    buffer[n] = '\0';  // 문자열 끝 처리
    printf("서버 응답: %s\n", buffer);

    close(sock);
    return 0;
}


