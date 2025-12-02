#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "client_handler.h"

#define PORT 8080

int main() {
    int server_fd, client_fd;
    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd < 0) { perror("socket"); exit(1); }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) { perror("bind"); exit(1); }
    if(listen(server_fd, 5) < 0) { perror("listen"); exit(1); }

    printf("모듈화된 POST 처리 웹서버 시작: 포트 %d\n", PORT);

    while(1) {
        client_fd = accept(server_fd, (struct sockaddr*)&addr, &addrlen);
        if(client_fd < 0) { perror("accept"); continue; }
        handle_client(client_fd);
    }

    close(server_fd);
    return 0;
}

