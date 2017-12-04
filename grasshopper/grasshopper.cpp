#include <iostream>
#include <fstream>
#include <utility>
#include "grasshopper.h"

std::array<unsigned char, N> get_reverse_array(const std::array<unsigned char, N>& array) {
    std::array<unsigned char, N> result;
    for (unsigned short i = 0; i < N; i++)
	result[array[i]] = i;
    return result;
}

unsigned char poly_multiplication(unsigned char a, unsigned char b) {
    unsigned short result = 0;
    for (unsigned char i = 0; i < 8; i++) {
	if ((a >> i) % 2)
	    result ^= b << i;
    }

    while (true) {
	unsigned char i = 15;
	while (!((result >> i) % 2) && i >= 8)
	    i--;

	if (i < 8)
	    break;

	result ^= DIVIDER << (i - 8);
    }

    return result;
}

Block X_function(Block block, const Block& key) {
    for (unsigned char i = 0; i < SECTIONS_NUMBER; i++)
	block[i] ^= key[i];
    return block;
}

Block S_function(Block block) {
    /*
    block[0] = PI_ARRAY[block[0]];
    block[1] = PI_ARRAY[block[1]];
    block[2] = PI_ARRAY[block[2]];
    block[3] = PI_ARRAY[block[3]];
    block[4] = PI_ARRAY[block[4]];
    block[5] = PI_ARRAY[block[5]];
    block[6] = PI_ARRAY[block[6]];
    block[7] = PI_ARRAY[block[7]];
    block[8] = PI_ARRAY[block[8]];
    block[9] = PI_ARRAY[block[9]];
    block[10] = PI_ARRAY[block[10]];
    block[11] = PI_ARRAY[block[11]];
    block[12] = PI_ARRAY[block[12]];
    block[13] = PI_ARRAY[block[13]];
    block[14] = PI_ARRAY[block[14]];
    block[15] = PI_ARRAY[block[15]];
    */
    for (unsigned char i = 0; i < SECTIONS_NUMBER; i++)
	block[i] = PI_ARRAY[block[i]];
    return block;
}

Block L_function(const Block& block) {
    Block result = {0};
    for (unsigned char i = 0; i < SECTIONS_NUMBER; i++)
	for (unsigned char j = 0; j < SECTIONS_NUMBER; j++)
	    result[j] ^= poly_multiplication(LTransformationMatrix[i][j], block[i]);
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

    for (unsigned char i = 1; i <= ITERATIONS_NUM / 2 - 1; i++) {
	for (unsigned char j = 1; j <= 8; j++) {
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
    for (unsigned char i = 0; i < ITERATIONS_NUM - 1; i++)
	block = L_function(S_function(X_function(block, keys[i])));
    return X_function(block, keys[ITERATIONS_NUM - 1]);
}

Block S_i_function(Block block) {
    for (unsigned char i = 0; i < SECTIONS_NUMBER; i++)
	block[i] = REVERSE_PI_ARRAY[block[i]];
    return block;
}

Block L_i_function(const Block& block) {
    Block result = {0};
    for (unsigned char i = 0; i < SECTIONS_NUMBER; i++)
	for (unsigned char j = 0; j < SECTIONS_NUMBER; j++)
	    result[j] ^= poly_multiplication(inversedLTransformationMatrix[i][j], block[i]);
    return result;
}

Block decoding(Block block, const Keys& keys) {
    for (unsigned char i = ITERATIONS_NUM - 1; i > 0; i--)
	block = S_i_function(L_i_function(X_function(block, keys[i])));
    return X_function(block, keys[0]);
}
