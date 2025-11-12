#include "lox/token.h"

#include <iostream>

namespace llox {

std::ostream& operator<<(std::ostream& out, const Token& token) {
  out << token.str();
  return out;
}

}  // namespace llox
