#ifndef BITVECTOR_H
#define BITVECTOR_H

#include <stdint.h>
#include <cmath>
#include <iostream>
#include <memory>
#include "BitString.h"

class BitVector {
    private:
        std::shared_ptr<uint64_t[]> bits;
        size_t size;
        size_t bits_size;
    public:
        BitVector(size_t size) {
            this->size = size;
            this->bits_size = ceil((double)size / 64);
            this->bits = std::shared_ptr<uint64_t[]>(new uint64_t[this->bits_size]);

        }

        size_t array_size() {
            return this->bits_size;
        }

        uint64_t& operator[](size_t index) {
            return this->bits[index];
        }

        void reset() {
            for (size_t i=0; i<this->bits_size; i++) this->bits[i] = 0; 
        }

        void set(size_t index) {
            size_t arr_index = floor((double)index / 64);
            size_t bit_index = index % 64;
            this->bits[arr_index] |= 0x1 << bit_index;
        }

        bool get(size_t index) {
            size_t arr_index = floor((double)index / 64);
            size_t bit_index = index % 64;
            return this->bits[arr_index] & (0x1 << bit_index);
        }

        void intersect(BitVector* other, BitVector* output) {
            for (size_t i=0; i<this->bits_size; i++) {
                output->bits[i] = this->bits[i] & other->bits[i];
            }
        }

        void unite(BitVector* other, BitVector* output) {
            for (size_t i=0; i<this->bits_size; i++) {
                output->bits[i] = this->bits[i] | other->bits[i];
            }
        }

        void XOR(BitVector* other, BitVector* output) {
            for (size_t i=0; i<this->bits_size; i++) {
                output->bits[i] = this->bits[i] ^ other->bits[i];
            }
        }

        void addition(BitVector* other, BitVector* output) {
            for (size_t i=0; i<this->bits_size; i++) {
                output->bits[i] = this->bits[i] + other->bits[i];
                // check for overflow
                if (i + 1 < bits_size && (output->bits[i] < this->bits[i] || output->bits[i] < other->bits[i])) {
                    output->bits[i+1]++;
                    size_t j = i;
                    // check for further overflow
                    while (j < this->bits_size - 1 && this->bits[j] == 0) {
                        j++;
                        output->bits[j]++;
                    }
                }
            }
        }

        void complement(BitVector* output) {
            for (size_t i=0; i<this->bits_size; i++) {
                output->bits[i] = this->bits[i] ^ UINT64_MAX;
            }
        }

        BitVector operator&(BitVector other) {
            BitVector output(this->size);
            this->intersect(&other, &output);
            return output;
        }

        BitVector operator|(BitVector other) {
            BitVector output(this->size);
            this->unite(&other, &output);
            return output;
        }

        BitVector operator^(BitVector other) {
            BitVector output(this->size);
            this->XOR(&other, &output);
            return output;
        }

        BitVector operator+(BitVector other) {
            BitVector output(this->size);
            this->addition(&other, &output);
            return output;
        }

        BitVector operator~() {
            BitVector output(this->size);
            this->complement(&output);
            return output;
        }

        void operator&=(BitVector other) {
            this->intersect(&other, this);
        }

        void operator|=(BitVector other) {
            this->unite(&other, this);
        }

        void operator^=(BitVector other) {
            this->XOR(&other, this);
        }

        void operator+=(BitVector other) {
            this->addition(&other, this);
        }

        void shift_left(size_t n, BitVector* output) {
            for (size_t i = this->bits_size - 1; i > 0; i--) {
                output->bits[i] = this->bits[i] << n;
                output->bits[i] |= this->bits[i-1] >> (64 - n);
            }
            output->bits[0] = this->bits[0] << n;
        }

        void shift_right(size_t n, BitVector* output) {
            for (size_t i = 0; i < this->bits_size - 1; i++) {
                output->bits[i] = this->bits[i] >> n;
                output->bits[i] |= this->bits[i+1] << (64 - n);
            }
            output->bits[this->bits_size - 1] = this->bits[this->bits_size - 1] >> n;
        }

        BitVector operator<<(size_t n) {
            BitVector output(this->size);
            this->shift_left(n, &output);
            return output;
        }

        BitVector operator>>(size_t n) {
            BitVector output(this->size);
            this->shift_right(n, &output);
            return output;
        }

        void operator<<=(size_t n) {
            this->shift_left(n, this);
        }

        void operator>>=(size_t n) {
            this->shift_right(n, this);
        }

        void print_bits_left_to_right() {
            for (size_t i=0; i<this->size; i++) std::cout << (this->get(i) ? "1" : "0");
            std::cout << std::endl;
        }

        void get_binary_representation(char* s) const {
            bitstring(s, (char*) this->bits.get(), this->bits_size * sizeof(uint64_t), big_endian());
        }

        void print_binary_representation() const {
            char* buff = new char[this->bits_size * sizeof(uint64_t) * 8 + 1];
            bitstring(buff, (char*) this->bits.get(), this->bits_size * sizeof(uint64_t), big_endian());
            std::cout << buff << std::endl;
            delete[] buff;
        }

        void copyFrom(BitVector* other) {
            for (size_t i=0; i<this->bits_size; i++) {
                this->bits[i] = other->bits[i];
            }
        }
};

#endif