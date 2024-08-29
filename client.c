#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "parity.h"

#define PORT (uint16_t)3000
#define BUFFER_SIZE 512
#define CHECK(err_code, exit_func) { \
    if((err_code) == -1) { \
        perror(strerror(errno)); \
        exit_func; \
        exit(EXIT_FAILURE); \
    } \
} \

void input(char* str, const size_t size) {
    printf("data_word> ");
    fgets(str, size, stdin);

    uint8_t choice = 0;
    do{
        printf("Parity: 1.Odd 2.Even 3.Generate\nchoice> ");
        scanf("%hhu", &choice);
    }while(choice > 3 && choice <= 0);

    size_t offset = strlen(str);
    str[offset - 1] = ((choice == 1 )*'0') + ((choice == 2) *'1') + ((choice == 3)*parity_generate(str, offset));
    str[offset] = '\0';
}

void output(const char* str, const size_t size) {
    printf("code_word=%s\n", str);
    bool res = parity_check(str, size);
    printf("generated code_word is %s\n", res?"correct":"incorrect");
}

int main(void) {
    int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    CHECK(sockfd, {});

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(PORT),
        .sin_addr.s_addr = inet_addr("127.0.0.1")
    };

    char buffer[BUFFER_SIZE] = {0};
    int err = connect(sockfd, (struct sockaddr*)&addr, sizeof(addr));
    CHECK(err, {close(sockfd);})

    input(buffer, BUFFER_SIZE);
    output(buffer, strlen(buffer));

    send(sockfd, buffer, BUFFER_SIZE, 0);
    recv(sockfd, buffer, BUFFER_SIZE, 0);
    printf("server_respone> %s\n", buffer);
    close(sockfd);
}
