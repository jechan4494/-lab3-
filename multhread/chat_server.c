#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 9000
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

int main() {
    int server_fd, client_fd, max_fd, activity, i, valread;
    int client_sockets[MAX_CLIENTS] = {0};
    struct sockaddr_in address;
    char buffer[BUFFER_SIZE];
    fd_set readfds;

    // 1. 서버 소켓 생성
    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket"); exit(EXIT_FAILURE);
    }

    // 포트 재사용 옵션
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // 2. 주소 바인딩
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if(bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind"); exit(EXIT_FAILURE);
    }

    // 3. 리슨
    if(listen(server_fd, 5) < 0) {
        perror("listen"); exit(EXIT_FAILURE);
    }
    printf("채팅 서버 시작: 포트 %d\n", PORT);

    socklen_t addrlen = sizeof(address);

    while(1) {
        // 4. fd_set 초기화
        FD_ZERO(&readfds);
        FD_SET(server_fd, &readfds);
        max_fd = server_fd;

        // 클라이언트 소켓 추가
        for(i=0; i<MAX_CLIENTS; i++) {
            if(client_sockets[i] > 0) FD_SET(client_sockets[i], &readfds);
            if(client_sockets[i] > max_fd) max_fd = client_sockets[i];
        }

        // 5. select 호출
        activity = select(max_fd + 1, &readfds, NULL, NULL, NULL);
        if(activity < 0) { perror("select"); exit(EXIT_FAILURE); }

        // 6. 새로운 연결 요청 처리
        if(FD_ISSET(server_fd, &readfds)) {
            if((client_fd = accept(server_fd, (struct sockaddr*)&address, &addrlen)) < 0) {
                perror("accept"); exit(EXIT_FAILURE);
            }
            printf("새 클라이언트 접속: fd=%d\n", client_fd);

            // 빈 슬롯에 저장
            for(i=0; i<MAX_CLIENTS; i++) {
                if(client_sockets[i] == 0) {
                    client_sockets[i] = client_fd;
                    break;
                }
            }
        }

        // 7. 클라이언트 메시지 처리
        for(i=0; i<MAX_CLIENTS; i++) {
            int sd = client_sockets[i];
            if(sd > 0 && FD_ISSET(sd, &readfds)) {
                valread = read(sd, buffer, sizeof(buffer)-1);
                if(valread <= 0) {
                    // 클라이언트 종료
                    printf("클라이언트 종료: fd=%d\n", sd);
                    close(sd);
                    client_sockets[i] = 0;
                } else {
                    buffer[valread] = '\0';
                    printf("클라이언트[%d]: %s", sd, buffer);

                    // 다른 클라이언트에게 메시지 전달
                    for(int j=0; j<MAX_CLIENTS; j++) {
                        if(client_sockets[j] > 0 && client_sockets[j] != sd) {
                            write(client_sockets[j], buffer, strlen(buffer));
                        }
                    }
                }
            }
        }
    }

    return 0;
}

