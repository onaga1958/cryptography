#include <iostream>
#include <string>
#include <array>
#include <fstream>
#include <utility>
#include "grasshopper.h"
#include "utils.h"

bool _compare(const uint8_t* block_0, const uint8_t* block_1) {
    bool result = true;
    for (uint8_t i = 0; i < SECTIONS_NUMBER; i++)
	if (block_0[i] != block_1[i]) {
	    result = false;
	    break;
	}
    return result;
}

void _test(
	void (*function_to_test)(uint8_t* block),
	std::string function_name,
	const StringArray& input,
	const StringArray& answers) {

    for (uint8_t i = 0; i < TESTS_NUM; i++) {
	uint8_t* result_block = string_to_block(input[i]);
	uint8_t* answer_block = string_to_block(answers[i]);
	(*function_to_test)(result_block);

	bool compare_result = _compare(result_block, answer_block);

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

void test_get_iteration_keys() {
    MainKey key(
	string_to_block("8899aabbccddeeff0011223344556677"),
	string_to_block("fedcba98765432100123456789abcdef")
    );
    Keys right_keys = {
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

    Keys answer = get_iteration_keys(key);
    bool result = true;
    for (uint8_t i = 0; i < ITERATIONS_NUM; i++)
	if (!_compare(answer[i], right_keys[i])) {
	    result = false;
	    break;
	}

    std::cout << "get_iteration_keys test: " << result << "\n";
    if (!result) {
	for (int i = 0; i < ITERATIONS_NUM; i++) {
	    std::cout << "right " << i << " key: ";
	    print_block(right_keys[i]);
	    std::cout << "got   " << i << " key: ";
	    print_block(answer[i]);
	}
    }
    std::cout << std::endl;
}

void test_encoding() {
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

    uint8_t* answer = string_to_block("1122334455667700ffeeddccbbaa9988");
    uint8_t* right_answer = string_to_block("7f679d90bebc24305a468d42b9d4edcd");
    set_tables(keys);
    encoding(answer, keys);

    bool right = _compare(answer, right_answer);
    std::cout << "encoding test: " << right << "\n";
    if (!right) {
	std::cout << "right: ";
	print_block(right_answer);
	std::cout << "  got: ";
	print_block(answer);
    }
    std::cout << std::endl;
}

void test_S_i() {
    StringArray answers = {
	"ffeeddccbbaa99881122334455667700",
	"b66cd8887d38e8d77765aeea0c9a7efc",
	"559d8dd7bd06cbfe7e7b262523280d39",
	"0c3322fed531e4630d80ef5c5a81c50b"
    };

    StringArray input = {
	"b66cd8887d38e8d77765aeea0c9a7efc",
	"559d8dd7bd06cbfe7e7b262523280d39",
	"0c3322fed531e4630d80ef5c5a81c50b",
	"23ae65633f842d29c5df529c13f5acda"
    };

    _test(S_i_function, "S_i", input, answers);
}

void test_L_i() {
    StringArray answers = {
	"64a59400000000000000000000000000",
	"d456584dd0e3e84cc3166e4b7fa2890d",
	"79d26221b87b584cd42fbc4ffea5de9a",
	"01000000000000000000000000000000"
	// "0e93691a0cfc60408b7b68f66b513c13"
    };

    StringArray input = {
	"d456584dd0e3e84cc3166e4b7fa2890d",
	"79d26221b87b584cd42fbc4ffea5de9a",
	"0e93691a0cfc60408b7b68f66b513c13",
	"cf6ea276726c487ab85d27bd10dd8494"
	// "00000000000000000000000000000000"
	// "e6a8094fee0aa204fd97bcb0b44b8580"
    };

    _test(L_i_function, "L_i", input, answers);
}

void test_decoding() {
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

    set_tables(keys);
    uint8_t* right_answer = string_to_block("1122334455667700ffeeddccbbaa9988");
    uint8_t* answer = string_to_block("7f679d90bebc24305a468d42b9d4edcd");
    decoding(answer, keys);

    bool right = _compare(answer, right_answer);
    std::cout << "decoding test: " << right << "\n";
    if (!right) {
	std::cout << "right: ";
	print_block(right_answer);
	std::cout << "  got: ";
	print_block(answer);
    }
    std::cout << std::endl;
}

void test() {
    test_S();
    test_L();
    test_get_iteration_keys();
    test_encoding();
    test_L_i();
    test_S_i();
    test_decoding();
}

int main() {
    test();
    return 0;
}
