#ifndef POST_PARSER_H
#define POST_PARSER_H

void parse_urlencoded(const char* body);
void parse_multipart(const char* body);

#endif

