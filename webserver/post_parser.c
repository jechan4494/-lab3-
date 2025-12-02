#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "post_parser.h"

// URL 디코딩
void url_decode(char* str) {
    char* src = str;
    char* dst = str;
    while(*src) {
        if(*src == '%' && src[1] && src[2]) {
            char hex[3] = {src[1], src[2], 0};
            *dst++ = (char) strtol(hex, NULL, 16);
            src += 3;
        } else if(*src == '+') {
            *dst++ = ' ';
            src++;
        } else {
            *dst++ = *src++;
        }
    }
    *dst = 0;
}

// x-www-form-urlencoded 처리
void parse_urlencoded(const char* body) {
    char tmp[2048];
    strncpy(tmp, body, sizeof(tmp)-1);
    tmp[sizeof(tmp)-1] = 0;
    url_decode(tmp);
    printf("x-www-form-urlencoded 본문: %s\n", tmp);
}

// multipart/form-data 처리 (간단 출력)
void parse_multipart(const char* body) {
    printf("multipart/form-data 본문:\n%s\n", body);
}

