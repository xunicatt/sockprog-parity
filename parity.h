#ifndef __PARITY_H
#define __PARITY_H

#include <stddef.h>
#include <stdbool.h>

bool parity_check(const char* str, const size_t size) {
    bool res = 0;
    for(size_t i = 0; i < size; i++)
        res ^= (str[i] == '1');
    return res;
}

char parity_generate(const char* str, const size_t size) {
    bool res = parity_check(str, size);
    return (res * '0') + (!res * '1');
}

#endif
