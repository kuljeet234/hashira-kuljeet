#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// Convert a string in any base (2–16) into decimal (as long long).
// Works fine for smaller inputs (testcase1). 
// For very large numbers (testcase2), you’d need big integer support.
long long safeConvertToDecimal(const std::string &value, int base) {
    long long result = 0;
    for (char c : value) {
        int digit;
        if (std::isdigit(c)) digit = c - '0';
        else digit = 10 + (std::tolower(c) - 'a');
        result = result * base + digit;
    }
    return result;
}

// Multiply polynomial by (x - root)
std::vector<long long> multiplyPoly(const std::vector<long long> &poly, long long root) {
    std::vector<long long> result(poly.size() + 1, 0);

    for (size_t i = 0; i < poly.size(); i++) {
        result[i + 1] += poly[i];         // ✅ Multiply by x: shift up
        result[i] -= root * poly[i];      // ✅ Multiply by -root
    }
    return result;
}

int main(int argc, char* argv[]) {
    // Expect JSON filename from command line
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
    std::vector<long long> roots;
    for (int i = 1; i <= n && (int)roots.size() < k; i++) {
        if (data.contains(std::to_string(i))) {
            int base = std::stoi(data[std::to_string(i)]["base"].get<std::string>());
            std::string value = data[std::to_string(i)]["value"];
            long long root = safeConvertToDecimal(value, base);
            roots.push_back(root);
        }
    }

    // Build polynomial coefficients
    std::vector<long long> poly = {1}; // start with 1
    for (long long root : roots) {
        poly = multiplyPoly(poly, root);
    }

    // Print coefficients
    std::cout << "Polynomial coefficients: ";
    for (long long coeff : poly) {
        std::cout << coeff << " ";
    }
    std::cout << std::endl;

    return 0;
}
