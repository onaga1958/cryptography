#include <array>
#include <fstream>
#include <utility>
#include "grasshopper.h"

unsigned char poly_multiplication(unsigned char a, unsigned char b) {
    unsigned short result = 0;
    for (unsigned char i = 0; i < 8; i++) {
	if ((a >> i) % 2)
	    result += b << i;
    }

    while (true) {
	unsigned char i = 15;
	while (!((a >> i) % 2))
	    i--;

	if (i < 8)
	    break;

	result -= DIVIDER << (i - 8);
    }

    return result;
}

Block X_function(const Block& block, const Block& key) {
    Block result;
    for (unsigned char i; i < SECTIONS_NUMBER; i++)
	result[i] = block[i] ^ key[i];
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
    for (unsigned char i = 0; i < ITERATIONS_NUM - 1; i++) {
	block = L_function(S_function(X_function(block, keys[i])));
    }
    return X_function(block, keys[ITERATIONS_NUM - 1]);
}
