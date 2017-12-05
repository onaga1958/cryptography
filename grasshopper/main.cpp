#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <array>
#include <fstream>
#include <utility>
#include "grasshopper.h"
#include "utils.h"
#include "LS_matrix.h"

int main() {
    Keys keys = {
	string_to_block("8899aabbccddeeff0011223344556677"),
	string_to_block("fedcba98765432100123456789abcdef"),
	string_to_block("db31485315694343228d6aef8cc78c44"),
	string_to_block("3d4553d8e9cfec6815ebadc40a9ffd04"),
	string_to_block("57646468c44a5e28d3e59246f429f1ac"),
	string_to_block("bd079435165c6432b532e82834da581b"),
	string_to_block("51e640757e8745de705727265a0098b1"),
	string_to_block("5a7925017b9fdd3ed72a91a22286f984"),
	string_to_block("bb44e25378c73123a5f32f73cdb6e517"),
	string_to_block("72e9dd7416bcf45b755dbaa88e4a4043")
    };

    uint8_t* a = string_to_block("1122334455667700ffeeddccbbaa9988");
    uint8_t* b = string_to_block("7f679d90bebc24305a468d42b9d4edcd");

    clock_t begin = clock();
    for (long i = 0; i < 65536; i++)
	encoding(a, keys);
    clock_t end = clock();
    double seconds = double(end - begin) / CLOCKS_PER_SEC;
    std::cout << "encoding: " << 1 / seconds << " MB/sec" << std::endl;

    begin = clock();
    for (long i = 0; i < 65536; i++)
	decoding(b, keys);
    end = clock();
    seconds = double(end - begin) / CLOCKS_PER_SEC;
    std::cout << "decoding: " << 1 / seconds << " MB/sec" << std::endl;
    /*
    std::ofstream f;
    f.open(path);
    for (int j = 0; j < 8; ++j)
	for (int i = 0; i < 256; ++i)
	    for (int m = 0; m < 256; ++m)
		for (int p = 0; p < 16; ++p) {
		    f << (LS_MATRIX[2 * j][i][p] ^ LS_MATRIX[2*j + 1][m][p]) << " ";
		}
    */
    return 0;
}
