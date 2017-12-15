#include <iostream>
#include <fstream>
#include <utility>
#include "grasshopper.h"

uint8_t LSX_MATRIX[9][256][16];
uint8_t INVERSED_LSX_MATRIX[9][256][16];

std::array<uint8_t, N> get_reverse_array(const std::array<uint8_t, N>& array) {
    std::array<uint8_t, N> result;
    for (unsigned short i = 0; i < N; ++i)
	result[array[i]] = i;
    return result;
}

uint8_t poly_multiplication(uint8_t a, uint8_t b) {
    uint16_t result = 0;
    for (uint8_t i = 0; i < 8; ++i) {
	if ((a >> i) % 2)
	    result ^= b << i;
    }

    while (true) {
	uint8_t i = 15;
	while (!((result >> i) % 2) && i >= 8)
	    i--;

	if (i < 8)
	    break;

	result ^= DIVIDER << (i - 8);
    }

    return result;
}

void X_function(uint8_t* block, const uint8_t* key) {
    for (uint8_t i = 0; i < 2; ++i)
	((uint64_t*)block)[i] ^= ((uint64_t*)key)[i];
}

void S_function(uint8_t* block) {
    for (uint8_t i = 0; i < SECTIONS_NUMBER; ++i)
	block[i] = PI_ARRAY[block[i]];
}

void L_function(uint8_t* block) {
    uint8_t result[SECTIONS_NUMBER] = {0};

    for (uint8_t i = 0; i < SECTIONS_NUMBER; ++i)
	for (uint8_t j = 0; j < SECTIONS_NUMBER; ++j)
	    result[j] ^= poly_multiplication(L_MATRIX[i][j], block[i]);
    memcpy(block, result, SECTIONS_NUMBER);
}

void L_S_function(uint8_t* block) {
    uint8_t result[SECTIONS_NUMBER] = {0};
    for (uint8_t i = 0; i < SECTIONS_NUMBER; ++i)
	for (uint8_t j = 0; j < SECTIONS_NUMBER; ++j)
	    result[j] ^= LS_MATRIX[i][block[i]][j];
    memcpy(block, result, SECTIONS_NUMBER);
}

void L_S_i_function(uint8_t* block) {
    uint8_t result[SECTIONS_NUMBER] = {0};
    for (uint8_t i = 0; i < SECTIONS_NUMBER; ++i)
	for (uint8_t j = 0; j < SECTIONS_NUMBER; ++j)
	    result[j] ^= INVERSED_LS_MATRIX[i][block[i]][j];
    memcpy(block, result, SECTIONS_NUMBER);
}

void L_S_X_function(uint8_t* block, uint8_t iteration) {
    uint8_t result[SECTIONS_NUMBER] = {0};
    X_function(result, LSX_MATRIX[iteration][block[0]]);
    for (uint8_t i = 1; i < SECTIONS_NUMBER; ++i)
	X_function(result, LS_MATRIX[i][block[i]]);
    memcpy(block, result, SECTIONS_NUMBER);
}

void L_S_X_i_function(uint8_t* block, uint8_t iteration) {
    uint8_t result[SECTIONS_NUMBER] = {0};
    X_function(result, INVERSED_LSX_MATRIX[ITERATIONS_NUM - iteration - 2][block[0]]);
    for (uint8_t i = 1; i < SECTIONS_NUMBER; ++i)
	X_function(result, INVERSED_LS_MATRIX[i][block[i]]);
    memcpy(block, result, SECTIONS_NUMBER);
}

MainKey F_function(uint8_t* block_1, uint8_t* block_0, uint8_t* key) {
    uint8_t* tmp = new uint8_t[SECTIONS_NUMBER];
    memcpy(tmp, block_1, SECTIONS_NUMBER);
    X_function(tmp, key);
    L_S_function(tmp);
    X_function(tmp, block_0);

    MainKey main_key(tmp, block_1);
    return main_key;
}

Keys get_iteration_keys(MainKey key) {
    Keys keys;
    keys[0] = key.first;
    keys[1] = key.second;

    for (uint8_t i = 1; i <= ITERATIONS_NUM / 2 - 1; ++i) {
	for (uint8_t j = 1; j <= 8; ++j) {
	    uint8_t constant[SECTIONS_NUMBER] = {0};
	    constant[SECTIONS_NUMBER - 1] = 8 * (i - 1) + j;
	    L_function(constant);

	    key = F_function(key.first, key.second, constant);
	}
	keys[i * 2] = key.first;
	keys[i * 2 + 1] = key.second;
    }
    return keys;
}

void set_tables(Keys keys) {
    for (uint8_t i = 0; i < ITERATIONS_NUM - 1; ++i)
	for (uint16_t k = 0; k < N; ++k) {
	    memcpy(LSX_MATRIX[i][k], LS_MATRIX[0][k], SECTIONS_NUMBER);
	    X_function(LSX_MATRIX[i][k], keys[i + 1]);
	}

    for (uint8_t i = ITERATIONS_NUM - 1; i > 0; --i)
	L_i_function(keys[i]);

    for (uint8_t i = 0; i < ITERATIONS_NUM - 1; ++i)
	for (uint16_t k = 0; k < N; ++k) {
	    memcpy(INVERSED_LSX_MATRIX[i][k], INVERSED_LS_MATRIX[0][k], SECTIONS_NUMBER);
	    X_function(INVERSED_LSX_MATRIX[i][k], keys[i + 1]);
	}
}

void encoding(uint8_t* block, const Keys& keys) {
    X_function(block, keys[0]);
    for (uint8_t i = 0; i < ITERATIONS_NUM - 1; ++i) {
	L_S_X_function(block, i);
    }
}

void S_i_function(uint8_t* block) {
    for (uint8_t i = 0; i < SECTIONS_NUMBER; ++i)
	block[i] = REVERSE_PI_ARRAY[block[i]];
}

void L_i_function(uint8_t* block) {
    uint8_t result[SECTIONS_NUMBER] = {0};
    for (uint8_t i = 0; i < SECTIONS_NUMBER; ++i)
	for (uint8_t j = 0; j < SECTIONS_NUMBER; ++j)
	    result[j] ^= poly_multiplication(INVERSED_L_MATRIX[i][j], block[i]);
    memcpy(block, result, SECTIONS_NUMBER);
}

void decoding(uint8_t* block, const Keys& keys) {
    S_function(block);
    for (uint8_t i = 0; i < ITERATIONS_NUM - 1; ++i)
	L_S_X_i_function(block, i);
    S_i_function(block);
    X_function(block, keys[0]);
}
