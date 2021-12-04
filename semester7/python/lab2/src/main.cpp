#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <cstdint>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

const uint32_t MOD = 1000000000;
const int DECIMAL_DIGITS = 9;

std::string digit_vector_to_string(std::vector<uint32_t> digits, bool debug) {
	std::ostringstream out;
	out << digits[digits.size() - 1];

	for (size_t i = digits.size() - 1; i > 0; i--) {
		if (debug)
			out << '_';
		out << std::setw(DECIMAL_DIGITS);
		out << std::setfill('0');
		out << digits[i - 1];
	}
	return out.str();
}

std::string multiply_factors_to_string(std::map<uint32_t, uint32_t> factors) {
	std::vector<uint32_t> digits{ 1 };

	for (auto [factor, power] : factors) {
		if (factor == 0)
			throw std::invalid_argument("Factor 0 is not allowed");

		for (uint32_t n = 0; n < power; n++) {
#ifdef DEBUG
			std::cerr << "Multiply " << digit_vector_to_string(digits, true) << " by " << factor << std::endl;
#endif
			size_t current_max_digit = digits.size();

			uint64_t carry = 0;
			for (size_t i = 0; i < current_max_digit; i++) {
#ifdef DEBUG
				std::cerr << "Carry: " << carry << std::endl;
#endif
				uint64_t result = (uint64_t)digits[i] * factor + carry;
#ifdef DEBUG
				std::cerr << "Result: " << result << std::endl;
#endif

				digits[i] = result % MOD;
				carry = result / MOD;
			}

			if (carry > MOD) {
#ifdef DEBUG
				std::cerr << "Final carry is larger than MOD: " << carry << std::endl;
#endif
				digits.push_back(carry % MOD);
				digits.push_back(carry / MOD);
			} else if (carry > 0) {
#ifdef DEBUG
				std::cerr << "Final carry: " << carry << std::endl;
#endif
				digits.push_back(carry);
			}
		}
	}
#ifdef DEBUG
	std::cerr << "Done. Size: " << digits.size() << std::endl;
#endif
	return digit_vector_to_string(digits, false);
}

namespace py = pybind11;

PYBIND11_MODULE(lab2, m) {
    m.doc() = R"pbdoc(
        Metaprogramming lab 2
        -----------------------

        .. currentmodule:: lab2

        .. autosummary::
           :toctree: _generate

        multiply_factors_to_string
    )pbdoc";

    m.def("multiply_factors_to_string", &multiply_factors_to_string, R"pbdoc(
        Takes a map of factors and powers, and returns long integer in the form of a string.

	Longer description of this function.
    )pbdoc");

#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}
