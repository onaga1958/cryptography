#include <iostream>
#include <string>
#include <array>
#include <fstream>
#include <utility>
#include "grasshopper.h"

unsigned char _to_uchar(char hex_symb) {
    if ('0' <= hex_symb && hex_symb <= '9')
	return hex_symb - '0';
    else
	return hex_symb - 'a' + 10;
}

unsigned char to_uchar(char * hex_symb_pair) {
    return (_to_uchar(hex_symb_pair[0]) << 4) + _to_uchar(hex_symb_pair[1]);
}

Block string_to_block(char * str) {
    Block result;
    for (unsigned char i = 0; i < SECTIONS_NUMBER; i++) {
	result[i] = to_uchar(str + i * 2);
    }
    return result;
}

char _uchar_to_hex(unsigned char ch) {
    if (ch < 10)
	return '0' + ch;
    else
	return 'a' - 10 + ch;
}

std::string uchar_to_hex_string(unsigned char uchar) {
    std::string result = {_uchar_to_hex(uchar >> 4), _uchar_to_hex(uchar & 15)};
    return result;
}

void print_block(const Block& block) {
    for (unsigned char i = 0; i < SECTIONS_NUMBER; i++)
	std::cout << uchar_to_hex_string(block[i]);
    std::cout << std::endl;
}

template <typename... Rest>
void _test(
	Block&& (*function_to_test)(const Block & block, const Rest&... rest),
	std::string function_name,
	const StringArray& input,
	const StringArray& answers,
	const Rest&... rest) {

    for (unsigned char i = 0; i < TESTS_NUM; i++) {
	Block input_block = string_to_block(input[i]);
	Block answer_block = string_to_block(answers[i]);
	Block result_block = (*function_to_test)(input_block, rest...);
	bool compare_result = result_block == answer_block;

	std::cout << "for function " << function_name << " test number " << int(i) << ": ";
	std::cout << compare_result << std::endl;
	if (!compare_result) {
	    std::cout << "got result: ";
	    print_block(result_block);
	    std::cout << "expected: ";
	    print_block(answer_block);
	    std::cout << std::endl;
	}
    }
    std::cout << "End function " << function_name << " testing\n" << std::endl;
}

void test_S() {
    StringArray input = {
	"ffeeddccbbaa99881122334455667700",
	"b66cd8887d38e8d77765aeea0c9a7efc",
	"559d8dd7bd06cbfe7e7b262523280d39",
	"0c3322fed531e4630d80ef5c5a81c50b"
    };

    StringArray answers = {
	"b66cd8887d38e8d77765aeea0c9a7efc",
	"559d8dd7bd06cbfe7e7b262523280d39",
	"0c3322fed531e4630d80ef5c5a81c50b",
	"23ae65633f842d29c5df529c13f5acda"
    };

    _test(S_function, "S", input, answers);
}

void test_L() {
    StringArray input = {
	"64a59400000000000000000000000000",
	"d456584dd0e3e84cc3166e4b7fa2890d",
	"79d26221b87b584cd42fbc4ffea5de9a",
	"01000000000000000000000000000000"
	// "0e93691a0cfc60408b7b68f66b513c13"
    };

    StringArray answers = {
	"d456584dd0e3e84cc3166e4b7fa2890d",
	"79d26221b87b584cd42fbc4ffea5de9a",
	"0e93691a0cfc60408b7b68f66b513c13",
	"cf6ea276726c487ab85d27bd10dd8494"
	// "00000000000000000000000000000000"
	// "e6a8094fee0aa204fd97bcb0b44b8580"
    };

    _test(L_function, "L", input, answers);
}

void test() {
    test_S();
    test_L();
}

int main() {
    test();
    return 0;
}
