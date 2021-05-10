#include <iostream>
#include <sstream>
#include <vector>

#include <cmath>

std::string prime_factor(unsigned x) {
  std::stringstream out;
  unsigned int num_to_fact = x;
  unsigned int j = 0; // exponential term
  for (unsigned int i = 2; i <= x / 2; i++) {
      while (num_to_fact % i == 0) {
          num_to_fact = num_to_fact / i;
          j ++;
      }
      if (j > 0) {
          out << i;
          if (j > 1) {
              out << '^' << j;
          }
          if (num_to_fact > 1){
              out << " x ";
          }
      }
      j = 0;
  }
  if (out.str().empty()) {
      out << x;
  }

  return out.str();
}


int main() {

  auto input = 9999965;
  auto output = prime_factor(input);

  std::cout << "Prime factorization of " << input << " is " << output
            << std::endl;
  // expected output: 3^2

  input = 240;
  output = prime_factor(input);

  std::cout << "Prime factorization of " << input << " is " << output
            << std::endl;
  // expected output: 2^4 x 3 x 5

  input = 60;
  output = prime_factor(input);

  std::cout << "Prime factorization of " << input << " is " << output
            << std::endl;
  // expected output: 2^2 x 3 x 5

  input = 8320;
  output = prime_factor(input);

  std::cout << "Prime factorization of " << input << " is " << output
            << std::endl;
  // expected output: 2^7 x 5 x 13

  input = 2;
  output = prime_factor(input);

  std::cout << "Prime factorization of " << input << " is " << output
            << std::endl;
  // expected output: 2
  return 0;
}
