#ifndef BITSTRING_H
#define BITSTRING_H

#include <cstdint>
#include <cstdio>

bool big_endian();

void bitstring(char* str, char* bytes, size_t size, bool big_endian);
#endif // BITSTRING_H