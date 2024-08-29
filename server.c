#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
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

int main(void) {
    int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    CHECK(sockfd, {});

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(PORT),
        .sin_addr.s_addr = inet_addr("127.0.0.1")
    };

    int err = bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));
    CHECK(err, {close(sockfd);});

    err = listen(sockfd, 5);
    CHECK(err, {close(sockfd);});

    char buffer[BUFFER_SIZE] = {0};
    socklen_t size = sizeof(addr);

    while(true) {
        int acceptfd = accept(sockfd, (struct sockaddr*)&addr, &size);
        CHECK(acceptfd, {close(sockfd);});

        err = read(acceptfd, buffer, BUFFER_SIZE - 1);
        CHECK(err, {close(acceptfd); close(sockfd);})

        bool res = false;
        printf("code_word=%s\n", buffer);
        if((res = parity_check(buffer, strlen(buffer)))){
            size_t size = strlen(buffer);
            buffer[size - 1] = '\0';
            printf("data_word=%s\n", buffer);
        } else {
            printf("error: received data is incorrect\n");
        }

        strncpy(buffer, res?"correct":"incorrect", BUFFER_SIZE - 1);
        err = send(acceptfd, buffer, BUFFER_SIZE - 1, 0);
        CHECK(err, {close(acceptfd); close(sockfd);})

        close(acceptfd);
    }

    close(sockfd);
}
