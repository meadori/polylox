#ifndef LLOX_ASTPRINTER_H
#define LLOX_ASTPRINTER_H

#include "ast.h"

namespace llox {

class AstPrinter : ExprVisitor {
  std::string representation;

 public:
  AstPrinter() {}

  std::string print(Expr *expr);

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

 private:
  template <typename... ExprT>
  std::string parenthesize(const std::string &name, ExprT... exprs);

  template <typename ExprT>
  std::string parenthesize(const std::string &name,
                           const std::vector<ExprT> &exprs);
};

}  // namespace llox

#endif
