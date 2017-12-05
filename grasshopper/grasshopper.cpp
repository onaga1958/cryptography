#include <iostream>
#include <fstream>
#include <utility>
#include "grasshopper.h"
#include "LS_matrix.h"

std::array<uint8_t, N> get_reverse_array(const std::array<uint8_t, N>& array) {
    std::array<uint8_t, N> result;
    for (unsigned short i = 0; i < N; i++)
	result[array[i]] = i;
    return result;
}

uint8_t poly_multiplication(uint8_t a, uint8_t b) {
    unsigned short result = 0;
    for (uint8_t i = 0; i < 8; i++) {
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

Block X_function(Block block, const Block& key) {
    for (uint8_t i = 0; i < SECTIONS_NUMBER; i++)
	block[i] ^= key[i];
    return block;
}

Block S_function(Block block) {
    for (uint8_t i = 0; i < SECTIONS_NUMBER; i++)
	block[i] = PI_ARRAY[block[i]];
    return block;
}

Block L_function(const Block& block) {
    Block result = {0};
    for (uint8_t i = 0; i < SECTIONS_NUMBER; i++)
	for (uint8_t j = 0; j < SECTIONS_NUMBER; j++)
	    result[j] ^= poly_multiplication(L_MATRIX[i][j], block[i]);
    return result;
}

Block L_S_function(const Block& block) {
    Block result = {0};
    for (uint8_t i = 0; i < SECTIONS_NUMBER; i++)
	for (uint8_t j = 0; j < SECTIONS_NUMBER; j++)
	    result[j] ^= LS_MATRIX[i][block[i]][j];
    return result;
}

Block L_S_i_function(const Block& block) {
    Block result = {0};
    for (uint8_t i = 0; i < SECTIONS_NUMBER; i++)
	for (uint8_t j = 0; j < SECTIONS_NUMBER; j++)
	    result[j] ^= INVERSED_LS_MATRIX[i][block[i]][j];
    return result;
}

MainKey F_function(const Block& block_1, const Block& block_0, const Block& key) {
    Block first = X_function(L_function(S_function(X_function(key, block_1))), block_0);
    return MainKey(first, block_1);
}

Keys get_iteration_keys(MainKey key) {
    Keys keys;
    keys[0] = key.first;
    keys[1] = key.second;

    for (uint8_t i = 1; i <= ITERATIONS_NUM / 2 - 1; i++) {
	for (uint8_t j = 1; j <= 8; j++) {
	    Block constant_iter = {0};
	    constant_iter[SECTIONS_NUMBER - 1] = 8 * (i - 1) + j;
	    Block constant = L_function(constant_iter);
	    key = F_function(key.first, key.second, constant);
	}
	keys[i * 2] = key.first;
	keys[i * 2 + 1] = key.second;
    }
    return keys;
}

Block encoding(Block block, const Keys& keys) {
    for (uint8_t i = 0; i < ITERATIONS_NUM - 1; i++)
	block = L_S_function(X_function(block, keys[i]));
    return X_function(block, keys[ITERATIONS_NUM - 1]);
}

Block S_i_function(Block block) {
    for (uint8_t i = 0; i < SECTIONS_NUMBER; i++)
	block[i] = REVERSE_PI_ARRAY[block[i]];
    return block;
}

Block L_i_function(const Block& block) {
    Block result = {0};
    for (uint8_t i = 0; i < SECTIONS_NUMBER; i++)
	for (uint8_t j = 0; j < SECTIONS_NUMBER; j++)
	    result[j] ^= poly_multiplication(INVERSED_L_MATRIX[i][j], block[i]);
    return result;
}

Block decoding(Block block, const Keys& keys) {
    block = S_function(block);
    for (uint8_t i = ITERATIONS_NUM - 1; i > 0; i--)
	block = X_function(L_S_i_function(block), keys[i]);
    return X_function(S_i_function(block), keys[0]);
}
