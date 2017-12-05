#include <iostream>
#include "utils.h"

uint8_t _to_uchar(char hex_symb) {
    if ('0' <= hex_symb && hex_symb <= '9')
	return hex_symb - '0';
    else
	return hex_symb - 'a' + 10;
}

uint8_t to_uchar(char * hex_symb_pair) {
    return (_to_uchar(hex_symb_pair[0]) << 4) + _to_uchar(hex_symb_pair[1]);
}

uint8_t* string_to_block(char * str) {
    uint8_t* result = new uint8_t[SECTIONS_NUMBER];
    for (uint8_t i = 0; i < SECTIONS_NUMBER; i++) {
	result[i] = to_uchar(str + i * 2);
    }
    return result;
}

char _uchar_to_hex(uint8_t ch) {
    if (ch < 10)
	return '0' + ch;
    else
	return 'a' - 10 + ch;
}

std::string uchar_to_hex_string(uint8_t uchar) {
    std::string result = {_uchar_to_hex(uchar >> 4), _uchar_to_hex(uchar & 15)};
    return result;
}

void print_block(uint8_t* block) {
    for (uint8_t i = 0; i < SECTIONS_NUMBER; i++)
	std::cout << uchar_to_hex_string(block[i]);
    std::cout << std::endl;
}
