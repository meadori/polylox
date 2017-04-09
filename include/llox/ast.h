#ifndef LLOX_AST_H
#define LLOX_AST_H

#include "token.h"

#include <string>
#include <vector>

namespace llox {

class AssignExpr;
class BinaryExpr;
class CallExpr;
class GetExpr;
class GroupingExpr;
class BoolLiteralExpr;
class NilLiteralExpr;
class NumberLiteralExpr;
class StringLiteralExpr;
class LogicalExpr;
class SetExpr;
class SuperExpr;
class ThisExpr;
class UnaryExpr;
class VariableExpr;

class ExprVisitor {
 public:
  virtual void visit(AssignExpr *expr) = 0;
  virtual void visit(BinaryExpr *expr) = 0;
  virtual void visit(CallExpr *expr) = 0;
  virtual void visit(GetExpr *expr) = 0;
  virtual void visit(GroupingExpr *expr) = 0;
  virtual void visit(BoolLiteralExpr *expr) = 0;
  virtual void visit(NilLiteralExpr *expr) = 0;
  virtual void visit(NumberLiteralExpr *expr) = 0;
  virtual void visit(StringLiteralExpr *expr) = 0;
  virtual void visit(LogicalExpr *expr) = 0;
  virtual void visit(SetExpr *expr) = 0;
  virtual void visit(SuperExpr *expr) = 0;
  virtual void visit(ThisExpr *expr) = 0;
  virtual void visit(UnaryExpr *expr) = 0;
  virtual void visit(VariableExpr *expr) = 0;
};

class Expr {
 public:
  virtual void accept(ExprVisitor &visitor) = 0;
};

class AssignExpr : public Expr {
 public:
  std::unique_ptr<Token> name;
  std::unique_ptr<Expr> value;

  AssignExpr(Token *name, Expr *value) : name(name), value(value) {}

  virtual void accept(ExprVisitor &visitor) { visitor.visit(this); }
};

class BinaryExpr : public Expr {
 public:
  std::unique_ptr<Expr> left;
  std::unique_ptr<Token> op;
  std::unique_ptr<Expr> right;

  BinaryExpr(Expr *left, Token *op, Expr *right)
      : left(left), op(op), right(right) {}

  virtual void accept(ExprVisitor &visitor) { visitor.visit(this); }
};

class CallExpr : public Expr {
 public:
  std::unique_ptr<Expr> callee;
  std::unique_ptr<Token> paren;
  std::vector<std::unique_ptr<Expr>> arguments;

  CallExpr(Expr *callee, Token *paren, std::vector<Expr *> &arguments)
      : callee(callee), paren(paren) {
    for (auto &elem : arguments) {
      arguments.push_back(std::move(elem));
    }
  }

  virtual void accept(ExprVisitor &visitor) { visitor.visit(this); }
};

class GetExpr : public Expr {
 public:
  std::unique_ptr<Expr> object;
  std::unique_ptr<Token> name;

  GetExpr(Expr *object, Token *name) : object(object), name(name) {}
};

class GroupingExpr : public Expr {
 public:
  std::unique_ptr<Expr> expression;

  GroupingExpr(Expr *expression) : expression(expression) {}

  virtual void accept(ExprVisitor &visitor) { visitor.visit(this); }
};

class BoolLiteralExpr : public Expr {
 public:
  bool value;

  BoolLiteralExpr(bool value) : value(value) {}

  virtual void accept(ExprVisitor &visitor) { visitor.visit(this); }
};

class NilLiteralExpr : public Expr {
 public:
  virtual void accept(ExprVisitor &visitor) { visitor.visit(this); }
};

class NumberLiteralExpr : public Expr {
 public:
  double value;

  NumberLiteralExpr(double value) : value(value) {}

  virtual void accept(ExprVisitor &visitor) { visitor.visit(this); }
};

class StringLiteralExpr : public Expr {
 public:
  std::string value;

  StringLiteralExpr(std::string &value) : value(value) {}

  virtual void accept(ExprVisitor &visitor) { visitor.visit(this); }
};

class LogicalExpr : public Expr {
 public:
  std::unique_ptr<Expr> left;
  std::unique_ptr<Token> op;
  std::unique_ptr<Expr> right;

  LogicalExpr(Expr *left, Token *op, Expr *right)
      : left(left), op(op), right(right) {}

  virtual void accept(ExprVisitor &visitor) { visitor.visit(this); }
};

class SetExpr : public Expr {
 public:
  std::unique_ptr<Expr> object;
  std::unique_ptr<Token> name;
  std::unique_ptr<Expr> value;

  SetExpr(Expr *object, Token *name, Expr *value)
      : object(object), name(name), value(value) {}

  virtual void accept(ExprVisitor &visitor) { visitor.visit(this); }
};

class SuperExpr : public Expr {
 public:
  std::unique_ptr<Token> keyword;
  std::unique_ptr<Token> method;

  SuperExpr(Token *keyword, Token *method) : keyword(keyword), method(method) {}

  virtual void accept(ExprVisitor &visitor) { visitor.visit(this); }
};

class ThisExpr : public Expr {
 public:
  std::unique_ptr<Token> keyword;

  ThisExpr(Token *keyword) : keyword(keyword) {}

  virtual void accept(ExprVisitor &visitor) { visitor.visit(this); }
};

class UnaryExpr : public Expr {
 public:
  std::unique_ptr<Token> op;
  std::unique_ptr<Expr> right;

  UnaryExpr(Token *op, Expr *right) : op(op), right(right) {}

  virtual void accept(ExprVisitor &visitor) { visitor.visit(this); }
};

class VariableExpr : public Expr {
 public:
  std::unique_ptr<Token> name;

  VariableExpr(Token *name) : name(name) {}

  virtual void accept(ExprVisitor &visitor) { visitor.visit(this); }
};

}  // namespace llox

#endif
