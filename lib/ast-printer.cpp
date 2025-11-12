#include "llox/ast-printer.h"

using namespace llox;

std::string AstPrinter::print(StmtList& statements) {
  for (auto& stmt : statements) stmt->accept(*this);
  return representation;
}

void AstPrinter::visit(AssignExpr* expr) {
  parenthesize("= " + expr->name->lexeme, expr->value.get());
}

void AstPrinter::visit(BinaryExpr* expr) {
  parenthesize(expr->op->lexeme, expr->left.get(), expr->right.get());
}

void AstPrinter::visit(CallExpr* expr) {
  std::vector<Expr*> exprs;
  exprs.push_back(expr->callee.get());
  for (auto& expr : expr->arguments) exprs.push_back(expr.get());
  parenthesize("call", exprs);
}

void AstPrinter::visit(GetExpr* expr) {
  representation.append("(. ");
  expr->object->accept(*this);
  representation.append(" " + expr->name->lexeme + ")");
}

void AstPrinter::visit(GroupingExpr* expr) {
  parenthesize("group", expr->expression.get());
}

void AstPrinter::visit(BoolLiteralExpr* expr) {
  representation.append(std::to_string(expr->value));
}

void AstPrinter::visit(NilLiteralExpr* expr) { representation.append("nil"); }

void AstPrinter::visit(NumberLiteralExpr* expr) {
  representation.append(std::to_string(expr->value));
}

void AstPrinter::visit(StringLiteralExpr* expr) {
  representation.append(expr->value);
}

void AstPrinter::visit(LogicalExpr* expr) {
  parenthesize(expr->op->lexeme, expr->left.get(), expr->right.get());
}

void AstPrinter::visit(SetExpr* expr) {
  representation.append("(= ");
  expr->object->accept(*this);
  representation.append(" " + expr->name->lexeme + " ");
  expr->value->accept(*this);
  representation.append(")\n");
}

void AstPrinter::visit(SuperExpr* expr) {
  representation.append("(super " + expr->method->lexeme + ")");
}

void AstPrinter::visit(ThisExpr* expr) { representation.append("this"); }

void AstPrinter::visit(UnaryExpr* expr) {
  parenthesize(expr->op->lexeme, expr->right.get());
}

void AstPrinter::visit(VariableExpr* expr) {
  representation.append(expr->name->lexeme);
}

void AstPrinter::visit(BlockStmt* stmt) {
  representation.append("(block\n");
  for (auto& stmt : stmt->statements) stmt->accept(*this);
  representation.append(")\n");
}

void AstPrinter::visit(ClassStmt* stmt) {}

void AstPrinter::visit(ExpressionStmt* stmt) {
  parenthesize(";", stmt->expression.get());
  representation.append("\n");
}

void AstPrinter::visit(FunctionStmt* stmt) {}

void AstPrinter::visit(IfStmt* stmt) {
  if (!stmt->elseBranch) {
    representation.append("(if ");
    stmt->condition->accept(*this);
    representation.append(" ");
    stmt->thenBranch->accept(*this);
  } else {
    representation.append("(if-else ");
    stmt->condition->accept(*this);
    representation.append(" ");
    stmt->thenBranch->accept(*this);
    representation.append(" ");
    stmt->elseBranch->accept(*this);
  }
  representation.append(")\n");
}

void AstPrinter::visit(PrintStmt* stmt) {
  parenthesize("print", stmt->expression.get());
  representation.append("\n");
}

void AstPrinter::visit(ReturnStmt* stmt) {}

void AstPrinter::visit(VarStmt* stmt) {
  representation.append("(var " + stmt->name->lexeme);
  if (stmt->initializer) {
    representation.append(" = ");
    stmt->initializer->accept(*this);
  }
  representation.append(")\n");
}

void AstPrinter::visit(WhileStmt* stmt) {
  representation.append("(while ");
  stmt->condition->accept(*this);
  representation.append("\n");
  stmt->body->accept(*this);
}

template <typename... ExprT>
std::string AstPrinter::parenthesize(const std::string& name, ExprT... exprs) {
  std::vector<Expr*> exprvec = {exprs...};
  return parenthesize(name, exprvec);
}

template <typename ExprT>
std::string AstPrinter::parenthesize(const std::string& name,
                                     const std::vector<ExprT>& exprs) {
  representation.append("(").append(name);

  for (auto& expr : exprs) {
    representation.append(" ");
    expr->accept(*this);
  }

  representation.append(")");

  return representation;
}
