#include "lox/interpreter.h"

#include <cmath>
#include <iostream>

using namespace llox;

void Interpreter::interpret(StmtList& statements) {
  for (auto& stmt : statements) execute(stmt.get());

  if (value) {
    std::cout << value->toString() << std::endl;
  }
}

void Interpreter::execute(Stmt* stmt) { stmt->accept(*this); }

ObjectPtr Interpreter::evaluate(Expr* expr) {
  expr->accept(*this);
  return std::move(value);
}

void Interpreter::visit(AssignExpr* expr) {
  value = evaluate(expr->value.get());
  environment->define(expr->name->lexeme, value->clone());
}

void Interpreter::visit(BinaryExpr* expr) {
  ObjectPtr left = ObjectPtr(evaluate(expr->left.get()));
  ObjectPtr right = ObjectPtr(evaluate(expr->right.get()));

  switch (expr->op->type) {
    case GREATER: {
      double leftValue = static_cast<Number*>(left.get())->value;
      double rightValue = static_cast<Number*>(right.get())->value;
      value.reset(new Bool(leftValue > rightValue));
      break;
    }
    case GREATER_EQUAL: {
      double leftValue = static_cast<Number*>(left.get())->value;
      double rightValue = static_cast<Number*>(right.get())->value;
      value.reset(new Bool(leftValue >= rightValue));
      break;
    }
    case LESS: {
      double leftValue = static_cast<Number*>(left.get())->value;
      double rightValue = static_cast<Number*>(right.get())->value;
      value.reset(new Bool(leftValue < rightValue));
      break;
    }
    case LESS_EQUAL: {
      double leftValue = static_cast<Number*>(left.get())->value;
      double rightValue = static_cast<Number*>(right.get())->value;
      value.reset(new Bool(leftValue <= rightValue));
      break;
    }
    case BANG_EQUAL: {
      value.reset(new Bool(!left->equals(right.get())));
      break;
    }
    case EQUAL_EQUAL: {
      value.reset(new Bool(left->equals(right.get())));
      break;
    }
    case MINUS: {
      double leftValue = static_cast<Number*>(left.get())->value;
      double rightValue = static_cast<Number*>(right.get())->value;
      value.reset(new Number(leftValue - rightValue));
      break;
    }
    case PLUS: {
      if (left->kind == NumberKind && right->kind == NumberKind) {
        double leftValue = static_cast<Number*>(left.get())->value;
        double rightValue = static_cast<Number*>(right.get())->value;
        value.reset(new Number(leftValue + rightValue));
      }

      if (left->kind == StringKind && right->kind == StringKind) {
        const std::string& leftValue = static_cast<String*>(left.get())->value;
        const std::string& rightValue =
            static_cast<String*>(right.get())->value;
        value.reset(new String(leftValue + rightValue));
      }

      break;
    }
    case SLASH: {
      double leftValue = static_cast<Number*>(left.get())->value;
      double rightValue = static_cast<Number*>(right.get())->value;
      value.reset(new Number(leftValue / rightValue));
      break;
    }
    case STAR: {
      double leftValue = static_cast<Number*>(left.get())->value;
      double rightValue = static_cast<Number*>(right.get())->value;
      value.reset(new Number(leftValue * rightValue));
      break;
    }
    case PERCENT: {
      double leftValue = static_cast<Number*>(left.get())->value;
      double rightValue = static_cast<Number*>(right.get())->value;
      value.reset(new Number(std::fmod(leftValue, rightValue)));
      break;
    }
    default:
      break;
  }
}

void Interpreter::visit(CallExpr* expr) {}

void Interpreter::visit(GetExpr* expr) {}

void Interpreter::visit(GroupingExpr* expr) {
  value = evaluate(expr->expression.get());
}

void Interpreter::visit(BoolLiteralExpr* expr) {
  value.reset(new Bool(expr->value));
}

void Interpreter::visit(NilLiteralExpr* expr) { value.reset(new Nil()); }

void Interpreter::visit(NumberLiteralExpr* expr) {
  value.reset(new Number(expr->value));
}

void Interpreter::visit(StringLiteralExpr* expr) {
  value.reset(new String(expr->value));
}

void Interpreter::visit(LogicalExpr* expr) {
  ObjectPtr left = ObjectPtr(evaluate(expr->left.get()));

  if (expr->op->type == OR && !left->isTrue()) {
    value = evaluate(expr->right.get());
  }

  if (expr->op->type == AND && left->isTrue()) {
    value = evaluate(expr->right.get());
  }
}

void Interpreter::visit(SetExpr* expr) {}

void Interpreter::visit(SuperExpr* expr) {}

void Interpreter::visit(ThisExpr* expr) {}

void Interpreter::visit(UnaryExpr* expr) {
  ObjectPtr right = ObjectPtr(evaluate(expr->right.get()));

  switch (expr->op->type) {
    case BANG: {
      Bool* result = static_cast<Bool*>(right.get());
      value.reset(new Bool(!result->isTrue()));
      break;
    }
    case MINUS: {
      Number* result = static_cast<Number*>(right.get());
      value.reset(new Number(-result->value));
      break;
    }
    default:
      break;
  }
}

void Interpreter::visit(VariableExpr* expr) {
  value = environment->get(expr->name->lexeme)->clone();
}

void Interpreter::visit(BlockStmt* stmt) {
  for (auto& stmt : stmt->statements) stmt->accept(*this);
  value = nullptr;
}

void Interpreter::visit(ClassStmt* stmt) {}

void Interpreter::visit(ExpressionStmt* stmt) {
  value = evaluate(stmt->expression.get());
}

void Interpreter::visit(FunctionStmt* stmt) {}

void Interpreter::visit(IfStmt* stmt) {
  value = evaluate(stmt->condition.get());
  if (value->isTrue())
    execute(stmt->thenBranch.get());
  else if (stmt->elseBranch)
    execute(stmt->elseBranch.get());
  value = nullptr;
}

void Interpreter::visit(PrintStmt* stmt) {
  value = evaluate(stmt->expression.get());
  if (value) {
    std::cout << value->toString() << std::endl;
  }
  value = nullptr;
}

void Interpreter::visit(ReturnStmt* stmt) {}

void Interpreter::visit(VarStmt* stmt) {
  if (stmt->initializer) value = evaluate(stmt->initializer.get());
  environment->define(stmt->name->lexeme, value->clone());
  value = nullptr;
}

void Interpreter::visit(WhileStmt* stmt) {
  value = evaluate(stmt->condition.get());
  while (value->isTrue()) {
    execute(stmt->body.get());
    value = evaluate(stmt->condition.get());
  }
  value = nullptr;
}
