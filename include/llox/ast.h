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
  enum ExprKind {
    AssignExprKind,
    BinaryExprKind,
    CallExprKind,
    GetExprKind,
    GroupingExprKind,
    BoolLiteralExprKind,
    NilLiteralExprKind,
    NumberLiteralExprKind,
    StringLiteralExprKind,
    LogicalExprKind,
    SetExprKind,
    SuperExprKind,
    ThisExprKind,
    UnaryExprKind,
    VariableExprKind,
  };

  ExprKind kind;

  Expr(ExprKind kind) : kind(kind) {}

  virtual Expr *clone() = 0;

  virtual void accept(ExprVisitor &visitor) = 0;
};

class AssignExpr : public Expr {
 public:
  std::unique_ptr<Token> name;
  std::unique_ptr<Expr> value;

  AssignExpr(Token *name, Expr *value)
      : Expr(Expr::AssignExprKind), name(name), value(value) {}

  Expr *clone() override {
    return new AssignExpr(name->clone(), value->clone());
  }

  void accept(ExprVisitor &visitor) override { visitor.visit(this); }
};

class BinaryExpr : public Expr {
 public:
  std::unique_ptr<Expr> left;
  std::unique_ptr<Token> op;
  std::unique_ptr<Expr> right;

  BinaryExpr(Expr *left, Token *op, Expr *right)
      : Expr(Expr::BinaryExprKind), left(left), op(op), right(right) {}

  Expr *clone() override {
    return new BinaryExpr(left->clone(), op->clone(), right->clone());
  }

  void accept(ExprVisitor &visitor) override { visitor.visit(this); }
};

class CallExpr : public Expr {
 public:
  std::unique_ptr<Expr> callee;
  std::unique_ptr<Token> paren;
  std::vector<std::unique_ptr<Expr>> arguments;

  CallExpr(Expr *callee, Token *paren, std::vector<Expr *> &actual_arguments)
      : Expr(Expr::CallExprKind), callee(callee), paren(paren) {
    for (auto &elem : actual_arguments) {
      arguments.push_back(std::unique_ptr<Expr>(elem));
    }
  }

  Expr *clone() override {
    std::vector<Expr *> new_arguments;
    new_arguments.resize(arguments.size());
    for (auto &arg : arguments) new_arguments.push_back(arg->clone());
    return new CallExpr(callee->clone(), paren->clone(), new_arguments);
  }

  void accept(ExprVisitor &visitor) override { visitor.visit(this); }
};

class GetExpr : public Expr {
 public:
  std::unique_ptr<Expr> object;
  std::unique_ptr<Token> name;

  GetExpr(Expr *object, Token *name)
      : Expr(Expr::GetExprKind), object(object), name(name) {}

  Expr *clone() override { return new GetExpr(object->clone(), name->clone()); }

  void accept(ExprVisitor &visitor) override { visitor.visit(this); }
};

class GroupingExpr : public Expr {
 public:
  std::unique_ptr<Expr> expression;

  GroupingExpr(Expr *expression)
      : Expr(Expr::GroupingExprKind), expression(expression) {}

  Expr *clone() override { return new GroupingExpr(expression->clone()); }

  void accept(ExprVisitor &visitor) override { visitor.visit(this); }
};

class BoolLiteralExpr : public Expr {
 public:
  bool value;

  BoolLiteralExpr(bool value) : Expr(Expr::BoolLiteralExprKind), value(value) {}

  Expr *clone() override { return new BoolLiteralExpr(value); }

  void accept(ExprVisitor &visitor) override { visitor.visit(this); }
};

class NilLiteralExpr : public Expr {
 public:
  NilLiteralExpr() : Expr(Expr::NilLiteralExprKind) {}

  Expr *clone() override { return new NilLiteralExpr(); }

  void accept(ExprVisitor &visitor) override { visitor.visit(this); }
};

class NumberLiteralExpr : public Expr {
 public:
  double value;

  NumberLiteralExpr(double value)
      : Expr(Expr::NumberLiteralExprKind), value(value) {}

  Expr *clone() override { return new NumberLiteralExpr(value); }

  void accept(ExprVisitor &visitor) override { visitor.visit(this); }
};

class StringLiteralExpr : public Expr {
 public:
  std::string value;

  StringLiteralExpr(std::string &value)
      : Expr(Expr::StringLiteralExprKind), value(value) {}

  Expr *clone() override { return new StringLiteralExpr(value); }

  void accept(ExprVisitor &visitor) override { visitor.visit(this); }
};

class LogicalExpr : public Expr {
 public:
  std::unique_ptr<Expr> left;
  std::unique_ptr<Token> op;
  std::unique_ptr<Expr> right;

  LogicalExpr(Expr *left, Token *op, Expr *right)
      : Expr(Expr::LogicalExprKind), left(left), op(op), right(right) {}

  Expr *clone() override {
    return new LogicalExpr(left->clone(), op->clone(), right->clone());
  }

  void accept(ExprVisitor &visitor) override { visitor.visit(this); }
};

class SetExpr : public Expr {
 public:
  std::unique_ptr<Expr> object;
  std::unique_ptr<Token> name;
  std::unique_ptr<Expr> value;

  SetExpr(Expr *object, Token *name, Expr *value)
      : Expr(Expr::SetExprKind), object(object), name(name), value(value) {}

  Expr *clone() override {
    return new SetExpr(object->clone(), name->clone(), value->clone());
  }

  void accept(ExprVisitor &visitor) override { visitor.visit(this); }
};

class SuperExpr : public Expr {
 public:
  std::unique_ptr<Token> keyword;
  std::unique_ptr<Token> method;

  SuperExpr(Token *keyword, Token *method)
      : Expr(Expr::SuperExprKind), keyword(keyword), method(method) {}

  Expr *clone() override {
    return new SuperExpr(keyword->clone(), method->clone());
  }

  void accept(ExprVisitor &visitor) override { visitor.visit(this); }
};

class ThisExpr : public Expr {
 public:
  std::unique_ptr<Token> keyword;

  ThisExpr(Token *keyword) : Expr(Expr::ThisExprKind), keyword(keyword) {}

  Expr *clone() override { return new ThisExpr(keyword->clone()); }

  void accept(ExprVisitor &visitor) override { visitor.visit(this); }
};

class UnaryExpr : public Expr {
 public:
  std::unique_ptr<Token> op;
  std::unique_ptr<Expr> right;

  UnaryExpr(Token *op, Expr *right)
      : Expr(Expr::UnaryExprKind), op(op), right(right) {}

  Expr *clone() override { return new UnaryExpr(op->clone(), right->clone()); }

  void accept(ExprVisitor &visitor) override { visitor.visit(this); }
};

class VariableExpr : public Expr {
 public:
  std::unique_ptr<Token> name;

  VariableExpr(Token *name) : Expr(Expr::VariableExprKind), name(name) {}

  Expr *clone() override { return new VariableExpr(name->clone()); }

  void accept(ExprVisitor &visitor) override { visitor.visit(this); }
};

}  // namespace llox

#endif
