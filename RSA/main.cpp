#include <iostream>
#include <gmpxx.h>
#include <string>
#include <fstream>

mpz_class power_by_module(
    const mpz_class& module,
    const mpz_class& base,
    const mpz_class& power
) {
    mpz_class result = 1;
    mpz_class curr_exp = base;
    std::string power_binary_str = power.get_str(2);
    for (auto it = power_binary_str.rbegin(); it != power_binary_str.rend(); ++it) {
        if (*it == '1')
            result = (result * curr_exp) % module;
        curr_exp = (curr_exp * curr_exp) % module;
    }
    return result;
};

std::string transform_to_bytes(const mpz_class& number) {
    std::string hex_str = number.get_str(16);
    size_t bytes_num = hex_str.size() / 2;
    std::string result;
    result.resize(bytes_num);

    for (size_t ind = 0; ind < bytes_num; ++ind) {
        std::string byte_str = hex_str.substr(ind * 2, 2);
        result[ind] = stoi(byte_str, 0, 16);
    }
    return result;
}

void first_task() {
    mpz_class y("6198559753362461818");
    mpz_class d("3690901371947698397");
    mpz_class n("18454506868330233991");
    mpz_class answer = power_by_module(n, y, d);

    std::ofstream file;
    file.open("decrypt.txt");
    file << transform_to_bytes(answer);
    file.close();
}

void second_task() {
    mpz_class n("18454506868330233991");
    mpz_class d("15818148736918707415");
    mpz_class e(7);

    mpz_class messages[] = {
        mpz_class(9237655),
        mpz_class(9934797394),
        mpz_class(939366611)
    };
    mpz_class signature[] = {
        mpz_class(1990325892773023203),
        mpz_class("13229347992719023203"),
        mpz_class("13229343983844023203")
    };

    for (size_t ind = 0; ind < 3; ++ind) {
        bool is_valid = power_by_module(n, signature[ind], e) == messages[ind];
        if (is_valid)
            std::cout << ind << " signature is valid" << std::endl;
        else
            std::cout << ind << " signature is invalid" << std::endl;
    }
}

int main() {
    first_task();
    second_task();
    return 0;
}
