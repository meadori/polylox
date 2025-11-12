#ifndef LLOX_INTERPRETER_H
#define LLOX_INTERPRETER_H

#include "ast.h"
#include "environment.h"
#include "object.h"

namespace llox {

class Interpreter : public ExprVisitor, public StmtVisitor {
  ObjectPtr value;
  std::unique_ptr<Environment> environment;

 public:
  Interpreter() : value(nullptr), environment(new Environment()) {}

  void interpret(StmtList& statements);

 private:
  void execute(Stmt* stmt);

  ObjectPtr evaluate(Expr* expr);

  /// Expressions.
  void visit(AssignExpr* expr) override;
  void visit(BinaryExpr* expr) override;
  void visit(CallExpr* expr) override;
  void visit(GetExpr* expr) override;
  void visit(GroupingExpr* expr) override;
  void visit(BoolLiteralExpr* expr) override;
  void visit(NilLiteralExpr* expr) override;
  void visit(NumberLiteralExpr* expr) override;
  void visit(StringLiteralExpr* expr) override;
  void visit(LogicalExpr* expr) override;
  void visit(SetExpr* expr) override;
  void visit(SuperExpr* expr) override;
  void visit(ThisExpr* expr) override;
  void visit(UnaryExpr* expr) override;
  void visit(VariableExpr* expr) override;

  /// Statements.
  void visit(BlockStmt* stmt) override;
  void visit(ClassStmt* stmt) override;
  void visit(ExpressionStmt* stmt) override;
  void visit(FunctionStmt* stmt) override;
  void visit(IfStmt* stmt) override;
  void visit(PrintStmt* stmt) override;
  void visit(ReturnStmt* stmt) override;
  void visit(VarStmt* stmt) override;
  void visit(WhileStmt* stmt) override;
};

}  // namespace llox

#endif
