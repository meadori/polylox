#include "llox/interpreter.h"

#include <iostream>

using namespace llox;

void Interpreter::interpret(StmtList &statements) {
  for (auto &stmt : statements) execute(stmt.get());

  if (value) {
    std::cout << value->toString() << std::endl;
  }
}

void Interpreter::execute(Stmt *stmt) { stmt->accept(*this); }

void Interpreter::evaluate(Expr *expr) { expr->accept(*this); }

void Interpreter::visit(AssignExpr *expr) {}

void Interpreter::visit(BinaryExpr *expr) {
  evaluate(expr->left.get());
  std::unique_ptr<ExprResult> left = std::move(value);

  evaluate(expr->right.get());
  std::unique_ptr<ExprResult> right = std::move(value);

  switch (expr->op->type) {
    case GREATER: {
      double leftValue = static_cast<NumberResult *>(left.get())->value;
      double rightValue = static_cast<NumberResult *>(right.get())->value;
      value.reset(new NumberResult(leftValue > rightValue));
      break;
    }
    case GREATER_EQUAL: {
      double leftValue = static_cast<NumberResult *>(left.get())->value;
      double rightValue = static_cast<NumberResult *>(right.get())->value;
      value.reset(new NumberResult(leftValue >= rightValue));
      break;
    }
    case LESS: {
      double leftValue = static_cast<NumberResult *>(left.get())->value;
      double rightValue = static_cast<NumberResult *>(right.get())->value;
      value.reset(new NumberResult(leftValue < rightValue));
      break;
    }
    case LESS_EQUAL: {
      double leftValue = static_cast<NumberResult *>(left.get())->value;
      double rightValue = static_cast<NumberResult *>(right.get())->value;
      value.reset(new NumberResult(leftValue <= rightValue));
      break;
    }
    case BANG_EQUAL: {
      value.reset(new BoolResult(!left->equals(right.get())));
      break;
    }
    case EQUAL_EQUAL: {
      value.reset(new BoolResult(left->equals(right.get())));
      break;
    }
    case MINUS: {
      double leftValue = static_cast<NumberResult *>(left.get())->value;
      double rightValue = static_cast<NumberResult *>(right.get())->value;
      value.reset(new NumberResult(leftValue - rightValue));
      break;
    }
    case PLUS: {
      if (left->kind == NumberKind && right->kind == NumberKind) {
        double leftValue = static_cast<NumberResult *>(left.get())->value;
        double rightValue = static_cast<NumberResult *>(right.get())->value;
        value.reset(new NumberResult(leftValue + rightValue));
      }

      if (left->kind == StringKind && right->kind == StringKind) {
        const std::string &leftValue =
            static_cast<StringResult *>(left.get())->value;
        const std::string &rightValue =
            static_cast<StringResult *>(right.get())->value;
        value.reset(new StringResult(leftValue + rightValue));
      }

      break;
    }
    case SLASH: {
      double leftValue = static_cast<NumberResult *>(left.get())->value;
      double rightValue = static_cast<NumberResult *>(right.get())->value;
      value.reset(new NumberResult(leftValue / rightValue));
      break;
    }
    case STAR: {
      double leftValue = static_cast<NumberResult *>(left.get())->value;
      double rightValue = static_cast<NumberResult *>(right.get())->value;
      value.reset(new NumberResult(leftValue * rightValue));
      break;
    }
    default:
      break;
  }
}

void Interpreter::visit(CallExpr *expr) {}

void Interpreter::visit(GetExpr *expr) {}

void Interpreter::visit(GroupingExpr *expr) {
  evaluate(expr->expression.get());
}

void Interpreter::visit(BoolLiteralExpr *expr) {
  value.reset(new BoolResult(expr->value));
}

void Interpreter::visit(NilLiteralExpr *expr) { value.reset(new NilResult()); }

void Interpreter::visit(NumberLiteralExpr *expr) {
  value.reset(new NumberResult(expr->value));
}

void Interpreter::visit(StringLiteralExpr *expr) {
  value.reset(new StringResult(expr->value));
}

void Interpreter::visit(LogicalExpr *expr) {
  evaluate(expr->left.get());

  if (expr->op->type == OR && !value->isTrue()) {
    evaluate(expr->right.get());
  }

  if (expr->op->type == AND && value->isTrue()) {
    evaluate(expr->right.get());
  }
}

void Interpreter::visit(SetExpr *expr) {}

void Interpreter::visit(SuperExpr *expr) {}

void Interpreter::visit(ThisExpr *expr) {}

void Interpreter::visit(UnaryExpr *expr) {
  evaluate(expr->right.get());

  switch (expr->op->type) {
    case BANG: {
      BoolResult *result = static_cast<BoolResult *>(value.get());
      value.reset(new BoolResult(!result->isTrue()));
      break;
    }
    case MINUS: {
      NumberResult *result = static_cast<NumberResult *>(value.get());
      value.reset(new NumberResult(-result->value));
      break;
    }
    default:
      break;
  }
}

void Interpreter::visit(VariableExpr *expr) {}

void Interpreter::visit(BlockStmt *stmt) {
  for (auto &stmt : stmt->statements) stmt->accept(*this);
  value.release();
}

void Interpreter::visit(ClassStmt *stmt) {}

void Interpreter::visit(ExpressionStmt *stmt) {}

void Interpreter::visit(FunctionStmt *stmt) {}

void Interpreter::visit(IfStmt *stmt) {
  evaluate(stmt->condition.get());
  if (value->isTrue())
    execute(stmt->thenBranch.get());
  else if (stmt->elseBranch)
    execute(stmt->elseBranch.get());
  value.release();
}

void Interpreter::visit(PrintStmt *stmt) {
  evaluate(stmt->expression.get());
  if (value) {
    std::cout << value->toString() << std::endl;
  }
  value.release();
}

void Interpreter::visit(ReturnStmt *stmt) {}

void Interpreter::visit(VarStmt *stmt) {}

void Interpreter::visit(WhileStmt *stmt) {
  evaluate(stmt->condition.get());
  while (value->isTrue()) {
    execute(stmt->body.get());
    evaluate(stmt->condition.get());
  }
  value.release();
}
