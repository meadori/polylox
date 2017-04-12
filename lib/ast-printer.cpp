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

void AstPrinter::visit(CallExpr *expr) {}

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

void AstPrinter::visit(ThisExpr *expr) {}

void AstPrinter::visit(UnaryExpr *expr) {
  parenthesize(expr->op->lexeme, expr->right.get());
}

void AstPrinter::visit(VariableExpr *expr) {}

template <typename... ExprT>
std::string AstPrinter::parenthesize(const std::string &name, ExprT... exprs) {
  std::vector<Expr *> exprvec = {exprs...};

  representation.append("(").append(name);

  for (Expr *expr : exprvec) {
    representation.append(" ");
    expr->accept(*this);
  }

  representation.append(")");

  return representation;
}
