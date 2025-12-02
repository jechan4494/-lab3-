#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9002

int main() {
    int server_fd, client_fd;
    struct sockaddr_in addr;
    char buffer[1024];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd < 0) { perror("socket"); exit(1); }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    if(bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) { perror("bind"); exit(1); }
    listen(server_fd, 5);
    printf("멀티 클라이언트 TCP 서버 시작: 포트 %d\n", PORT);

    while(1) {
        client_fd = accept(server_fd, NULL, NULL);
        if(fork() == 0) { // 자식 프로세스 처리
            read(client_fd, buffer, sizeof(buffer));
            printf("클라이언트 메시지: %s\n", buffer);
            write(client_fd, "서버 응답", 11);
            close(client_fd);
            exit(0);
        }
        close(client_fd); // 부모는 닫기
    }

    close(server_fd);
    return 0;
}

