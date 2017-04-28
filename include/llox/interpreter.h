#ifndef LLOX_INTERPRETER_H
#define LLOX_INTERPRETER_H

#include "ast.h"

namespace llox {

enum ResultKind {
  BoolKind,
  NumberKind,
  NilKind,
  StringKind,
};

class ExprResult {
 public:
  ExprResult(ResultKind kind) : kind(kind) {}

  virtual bool isTrue() const { return true; }

  virtual bool equals(ExprResult *other) const = 0;

  virtual std::string toString() const = 0;

  ResultKind kind;
};

class NumberResult : public ExprResult {
 public:
  double value;

  NumberResult(double value) : ExprResult(NumberKind), value(value) {}

  bool equals(ExprResult *other) const override {
    if (other->kind != NumberKind) return false;
    return value == static_cast<NumberResult *>(other)->value;
  }

  std::string toString() const override { return std::to_string(value); }
};

class StringResult : public ExprResult {
 public:
  std::string value;

  StringResult(const std::string &value)
      : ExprResult(StringKind), value(value) {}

  bool equals(ExprResult *other) const override {
    if (other->kind != StringKind) return false;
    return value == static_cast<StringResult *>(other)->value;
  }

  std::string toString() const override { return value; }
};

class BoolResult : public ExprResult {
 public:
  bool value;

  BoolResult(bool value) : ExprResult(BoolKind), value(value) {}

  bool isTrue() const override { return value; }

  bool equals(ExprResult *other) const override {
    if (other->kind != BoolKind) return false;
    return value == static_cast<BoolResult *>(other)->value;
  }

  std::string toString() const override { return std::to_string(value); }
};

class NilResult : public ExprResult {
 public:
  NilResult() : ExprResult(NilKind) {}

  bool isTrue() const override { return false; }

  bool equals(ExprResult *other) const override {
    return other->kind == NilKind;
  }

  std::string toString() const override { return "nil"; }
};

class Interpreter : public ExprVisitor, public StmtVisitor {
  std::unique_ptr<ExprResult> value;

 public:
  Interpreter() : value(nullptr) {}

  void interpret(StmtList &statements);

 private:
  void execute(Stmt *stmt);

  void evaluate(Expr *expr);

  /// Expressions.
  void visit(AssignExpr *expr) override;
  void visit(BinaryExpr *expr) override;
  void visit(CallExpr *expr) override;
  void visit(GetExpr *expr) override;
  void visit(GroupingExpr *expr) override;
  void visit(BoolLiteralExpr *expr) override;
  void visit(NilLiteralExpr *expr) override;
  void visit(NumberLiteralExpr *expr) override;
  void visit(StringLiteralExpr *expr) override;
  void visit(LogicalExpr *expr) override;
  void visit(SetExpr *expr) override;
  void visit(SuperExpr *expr) override;
  void visit(ThisExpr *expr) override;
  void visit(UnaryExpr *expr) override;
  void visit(VariableExpr *expr) override;

  /// Statements.
  void visit(BlockStmt *stmt) override;
  void visit(ClassStmt *stmt) override;
  void visit(ExpressionStmt *stmt) override;
  void visit(FunctionStmt *stmt) override;
  void visit(IfStmt *stmt) override;
  void visit(PrintStmt *stmt) override;
  void visit(ReturnStmt *stmt) override;
  void visit(VarStmt *stmt) override;
  void visit(WhileStmt *stmt) override;
};

}  // namespace llox

#endif
