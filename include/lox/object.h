#ifndef LLOX_OBJECT_H
#define LLOX_OBJECT_H

namespace llox {

enum ObjectKind {
  BoolKind,
  NumberKind,
  NilKind,
  StringKind,
};

class Object {
 public:
  Object(ObjectKind kind) : kind(kind) {}

  virtual bool isTrue() const { return true; }

  virtual bool equals(Object* other) const = 0;

  virtual std::unique_ptr<Object> clone() const = 0;

  virtual std::string toString() const = 0;

  ObjectKind kind;
};

class Number : public Object {
 public:
  double value;

  Number(double value) : Object(NumberKind), value(value) {}

  bool equals(Object* other) const override {
    if (other->kind != NumberKind) return false;
    return value == static_cast<Number*>(other)->value;
  }

  std::unique_ptr<Object> clone() const override {
    return llox::make_unique<Number>(value);
  }

  std::string toString() const override { return std::to_string(value); }
};

class String : public Object {
 public:
  std::string value;

  String(const std::string& value) : Object(StringKind), value(value) {}

  bool equals(Object* other) const override {
    if (other->kind != StringKind) return false;
    return value == static_cast<String*>(other)->value;
  }

  std::unique_ptr<Object> clone() const override {
    return llox::make_unique<String>(value);
  }

  std::string toString() const override { return value; }
};

class Bool : public Object {
 public:
  bool value;

  Bool(bool value) : Object(BoolKind), value(value) {}

  bool isTrue() const override { return value; }

  bool equals(Object* other) const override {
    if (other->kind != BoolKind) return false;
    return value == static_cast<Bool*>(other)->value;
  }

  std::unique_ptr<Object> clone() const override {
    return llox::make_unique<Bool>(value);
  }

  std::string toString() const override { return std::to_string(value); }
};

class Nil : public Object {
 public:
  Nil() : Object(NilKind) {}

  bool isTrue() const override { return false; }

  bool equals(Object* other) const override { return other->kind == NilKind; }

  std::unique_ptr<Object> clone() const override {
    return llox::make_unique<Nil>();
  }

  std::string toString() const override { return "nil"; }
};

typedef std::unique_ptr<Object> ObjectPtr;

}  // namespace llox

#endif
