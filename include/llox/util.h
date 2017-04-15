#ifndef LLOX_UTIL_H
#define LLOX_UTIL_H

#include <string>

namespace llox {

template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

}  // namespace llox

#endif
