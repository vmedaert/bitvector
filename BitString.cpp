#include "BitString.h"
#include <cstdint>
#include <cstdlib>

bool big_endian() {
    uint64_t x = 1;
    return ((char*) &x)[7] & 0x1;
}

void bitstring(char* str, char* bytes, size_t size, bool big_endian) {
    uint8_t mask;
    if (big_endian) {
        for (size_t i=0; i<size; i++) {
            mask = 0x80;
            for (size_t j=0; j<8; j++) {
                *str = (bytes[i] & mask) ? '1' : '0';
                str++;
                mask >>= 1;
            }
        }
    } else {
        for (int i = size - 1; i>=0; i--) {
            mask = 0x80;
            for (size_t j=0; j<8; j++) {
                *str = (bytes[i] & mask) ? '1' : '0';
                str++;
                mask >>= 1;
            }
        }
    }
    *str = '\0';
}