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
#include <netinet/in.h>

#define PORT 9000
#define BUFFER_SIZE 1024

int main() {
    int server_fd, client_fd;
    struct sockaddr_in addr;
    char buffer[BUFFER_SIZE];

    // 1. 소켓 생성
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) { perror("socket"); exit(1); }

    // 2. 포트 재사용 옵션
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // 3. 서버 주소 설정
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    // 4. 바인드
    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind"); exit(1);
    }

    // 5. 리슨
    if (listen(server_fd, 5) < 0) {
        perror("listen"); exit(1);
    }
    printf("TCP 서버 시작: 포트 %d\n", PORT);

    // 6. 클라이언트 처리 (하나씩)
    while(1) {
        client_fd = accept(server_fd, NULL, NULL);
        if (client_fd < 0) { perror("accept"); continue; }

        // 클라이언트 메시지 읽기
        int n = read(client_fd, buffer, sizeof(buffer)-1);
        if (n < 0) { perror("read"); close(client_fd); continue; }

        buffer[n] = '\0';  // 문자열 끝 처리
        printf("받은 메시지: %s\n", buffer);

        // 응답 전송
        char response[] = "서버에서 응답: 메시지 잘 받음!";
        write(client_fd, response, strlen(response));

        close(client_fd); // 클라이언트 연결 종료
    }

    close(server_fd);
    return 0;
}

