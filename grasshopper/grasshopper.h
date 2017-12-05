#pragma once

#include "utils.h"

uint8_t poly_multiplication(uint8_t a, uint8_t b);
Block encoding(Block block, const Keys& keys);
Block X_function(Block block, const Block& key);
Block S_function(Block block);
Block L_function(const Block& block);
MainKey F_function(const Block& block_1, const Block& block_0, const Block& key);
Keys get_iteration_keys(MainKey key);
Block encoding(Block block, const Keys& keys);
Block S_i_function(Block block);
Block L_i_function(const Block& block);
Block decoding(Block block, const Keys& keys);
