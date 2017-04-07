#include <iostream>

#include "llox/token.h"

namespace llox {

std::ostream& operator<<(std::ostream& out, const Token& token) {
  out << token.str();
  return out;
}

}  // namespace llox
