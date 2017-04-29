#ifndef LLOX_ENVIRONMENT_H
#define LLOX_ENVIRONMENT_H

#include "object.h"

#include <map>

namespace llox {

class Environment {
  std::unique_ptr<Environment> enclosing;
  std::map<std::string, ObjectPtr> values;

 public:
  void define(const std::string &name, ObjectPtr value) {
    values[name] = std::move(value);
  }

  Object *get(const std::string &name) const {
    auto It = values.find(name);
    if (It != values.end()) return It->second.get();
    return nullptr;
  }
};

}  // namespace llox

#endif
