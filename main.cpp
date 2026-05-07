#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include <nlohmann/json.hpp>
#include <boost/multiprecision/cpp_int.hpp>

using json = nlohmann::json;
using BigInt = boost::multiprecision::cpp_int;

// Convert a string in any base (2-16) to a BigInt.
// boost::multiprecision::cpp_int is heap-allocated and grows as
// needed, so 21-digit base-6 inputs no longer overflow.
BigInt convertToDecimal(const std::string &value, int base) {
    BigInt result = 0;
    BigInt b = base;
    for (char c : value) {
        int digit;
        if (std::isdigit(c)) digit = c - '0';
        else digit = 10 + (std::tolower(c) - 'a');
        result = result * b + digit;
    }
    return result;
}

// Multiply polynomial by (x - root). Coefficients are BigInt because
// the product of many large roots overflows even __int128.
std::vector<BigInt> multiplyPoly(const std::vector<BigInt> &poly, const BigInt &root) {
    std::vector<BigInt> result(poly.size() + 1, 0);

    for (size_t i = 0; i < poly.size(); i++) {
        result[i + 1] += poly[i];        // multiply by x: shift up
        result[i] -= root * poly[i];     // multiply by -root
    }
    return result;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: ./main <json_file>" << std::endl;
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << argv[1] << std::endl;
        return 1;
    }

    json data;
    file >> data;

    int n = data["keys"]["n"];
    int k = data["keys"]["k"];

    // Collect roots (only first k roots are needed)
    std::vector<BigInt> roots;
    for (int i = 1; i <= n && (int)roots.size() < k; i++) {
        if (data.contains(std::to_string(i))) {
            int base = std::stoi(data[std::to_string(i)]["base"].get<std::string>());
            std::string value = data[std::to_string(i)]["value"];
            roots.push_back(convertToDecimal(value, base));
        }
    }

    // Build polynomial coefficients
    std::vector<BigInt> poly = {1};
    for (const BigInt &root : roots) {
        poly = multiplyPoly(poly, root);
    }

    // Print coefficients
    std::cout << "Polynomial coefficients: ";
    for (const BigInt &coeff : poly) {
        std::cout << coeff << " ";
    }
    std::cout << std::endl;

    return 0;
}
