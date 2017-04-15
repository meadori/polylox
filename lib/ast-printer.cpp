#include "llox/ast-printer.h"

using namespace llox;

std::string AstPrinter::print(Expr *expr) {
  expr->accept(*this);
  return representation;
}

void AstPrinter::visit(AssignExpr *expr) {
  parenthesize("= " + expr->name->lexeme, expr->value.get());
}

void AstPrinter::visit(BinaryExpr *expr) {
  parenthesize(expr->op->lexeme, expr->left.get(), expr->right.get());
}

void AstPrinter::visit(CallExpr *expr) {
  std::vector<Expr *> exprs;
  exprs.push_back(expr->callee.get());
  for (auto &expr : expr->arguments) exprs.push_back(expr.get());
  parenthesize("call", exprs);
}

void AstPrinter::visit(GetExpr *expr) {}

void AstPrinter::visit(GroupingExpr *expr) {
  parenthesize("group", expr->expression.get());
}

void AstPrinter::visit(BoolLiteralExpr *expr) {
  representation.append(std::to_string(expr->value));
}

void AstPrinter::visit(NilLiteralExpr *expr) { representation.append("nil"); }

void AstPrinter::visit(NumberLiteralExpr *expr) {
  representation.append(std::to_string(expr->value));
}

void AstPrinter::visit(StringLiteralExpr *expr) {
  representation.append(expr->value);
}

void AstPrinter::visit(LogicalExpr *expr) {
  parenthesize(expr->op->lexeme, expr->left.get(), expr->right.get());
}

void AstPrinter::visit(SetExpr *expr) {}

void AstPrinter::visit(SuperExpr *expr) {}

void AstPrinter::visit(ThisExpr *expr) { representation.append("this"); }

void AstPrinter::visit(UnaryExpr *expr) {
  parenthesize(expr->op->lexeme, expr->right.get());
}

void AstPrinter::visit(VariableExpr *expr) {
  representation.append(expr->name->lexeme);
}

template <typename... ExprT>
std::string AstPrinter::parenthesize(const std::string &name, ExprT... exprs) {
  std::vector<Expr *> exprvec = {exprs...};
  return parenthesize(name, exprvec);
}

template <typename ExprT>
std::string AstPrinter::parenthesize(const std::string &name,
                                     const std::vector<ExprT> &exprs) {
  representation.append("(").append(name);

  for (auto &expr : exprs) {
    representation.append(" ");
    expr->accept(*this);
  }

  representation.append(")");

  return representation;
}
