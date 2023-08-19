#include "include/types.h"
#include "include/utils.h" 
#include "include/clib.h"

void* memset8(void* s, uint8_t c, uint32_t n) {
    uint8_t* z = s;

    while (n) {
        *z++ = c;
        n--;
    }

    return z;
}

void* memset32(void* s, uint32_t c, uint32_t n) {
    uint32_t* z = s;

    while (n) {
        *z++ = c;
        n -= 4;
    }

    return z;
}

void* memset(void* s, uint8_t c, uint32_t n) {
    if (((uint32_t)s | (uint32_t)n) & 3)
        return memset8(s, c, n);
    else
        return memset32(s, c | (c << 8) | (c << 16) | (c << 24), n);
}

void* memcpy(void* dest, const void* src, uint32_t n) {
    if (((uint32_t)src | (uint32_t)dest | (uint32_t)n) & 3) {
        const uint8_t* s = src;
        uint8_t* d = dest;

        while (n) {
            *d++ = *s++;
            n--;
        }
        
    } else {
        const uint32_t* s = src;
        uint32_t* d = dest;

        while (n) {
            *d++ = *s++;
            n -= 4;
        }
    }

    return dest;
}

int memcmp(const void* s1, const void* s2, uint32_t n) {
    if (((uint32_t)s1 | (uint32_t)s2 | (uint32_t)n) & 3) {
        const uint8_t* c1 = s1, *c2 = s2;
        uint8_t u1, u2;

        for (; n--; c1++, c2++) {
            u1 = *c1;
            u2 = *c2;
            if (u1 != u2)
                return u1 - u2;
        }
    } else {
        const uint32_t* l1 = s1, * l2 = s2;
        uint32_t u1, u2;

        for (; n; l1++, l2++) {
            u1 = *l1;
            u2 = *l2;
            if (u1 != u2)
                return u1 - u2;
            n -= 4;
        }
    }

    return 0;
}

uint32_t strlen(const char* str) {
    const char* s = str;

    while (*s)
        s++;

    return s - str;
}