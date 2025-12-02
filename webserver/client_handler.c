#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "client_handler.h"
#include "post_parser.h"

// HTTP 응답 전송
void send_response(int client, const char* status, const char* body) {
    char header[2048];
    snprintf(header, sizeof(header),
             "HTTP/1.1 %s\r\n"
             "Content-Length: %zu\r\n"
             "Content-Type: text/plain\r\n"
             "Connection: close\r\n\r\n",
             status, strlen(body));
    write(client, header, strlen(header));
    write(client, body, strlen(body));
}

// POST 요청 처리
void handle_post(int client, const char* buffer) {
    const char* content_type = strstr(buffer, "Content-Type:");
    const char* body_start = strstr(buffer, "\r\n\r\n");
    if(!body_start) {
        send_response(client, "400 Bad Request", "POST 본문 없음");
        return;
    }
    body_start += 4;

    if(content_type && strstr(content_type, "application/x-www-form-urlencoded")) {
        parse_urlencoded(body_start);
        send_response(client, "200 OK", "폼 데이터 수신 완료");
    } else if(content_type && strstr(content_type, "multipart/form-data")) {
        parse_multipart(body_start);
        send_response(client, "200 OK", "멀티파트 데이터 수신 완료");
    } else {
        send_response(client, "415 Unsupported Media Type", "지원하지 않는 Content-Type");
    }
}

// GET 요청 처리
void handle_get(int client, const char* path) {
    send_response(client, "200 OK", "GET 요청 처리 완료");
}

// 클라이언트 요청 처리
void handle_client(int client) {
    char buffer[2048];
    int n = read(client, buffer, sizeof(buffer)-1);
    if(n <= 0) { close(client); return; }
    buffer[n] = '\0';

    char method[8], path[128];
    sscanf(buffer, "%s %s", method, path);

    printf("요청: %s %s\n", method, path);

    if(strcmp(method, "GET") == 0) handle_get(client, path);
    else if(strcmp(method, "POST") == 0) handle_post(client, buffer);
    else send_response(client, "405 Method Not Allowed", "지원하지 않는 메소드");

    close(client);
}

