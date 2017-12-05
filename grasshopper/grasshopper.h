#pragma once

#include "utils.h"
#include <cstring>

uint8_t poly_multiplication(uint8_t a, uint8_t b);
void encoding(uint8_t* block, const Keys& keys);
void X_function(uint8_t* block, uint8_t* key);
void S_function(uint8_t* block);
void L_function(uint8_t* block);
MainKey F_function(uint8_t* block_1, uint8_t* block_0, uint8_t* key);
Keys get_iteration_keys(MainKey key);
void encoding(uint8_t* block, const Keys& keys);
void S_i_function(uint8_t* block);
void L_i_function(uint8_t* block);
void decoding(uint8_t* block, const Keys& keys);
