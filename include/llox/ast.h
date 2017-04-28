#ifndef LLOX_AST_H
#define LLOX_AST_H

#include "token.h"
#include "util.h"

#include <string>
#include <vector>

namespace llox {

/// Expressions.

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

  virtual ~Expr() {}

  virtual std::unique_ptr<Expr> clone() = 0;

  virtual void accept(ExprVisitor &visitor) = 0;
};

class AssignExpr : public Expr {
 public:
  std::unique_ptr<Token> name;
  std::unique_ptr<Expr> value;

  AssignExpr(std::unique_ptr<Token> name, std::unique_ptr<Expr> value)
      : Expr(Expr::AssignExprKind),
        name(std::move(name)),
        value(std::move(value)) {}

  std::unique_ptr<Expr> clone() override {
    return llox::make_unique<AssignExpr>(name->clone(), value->clone());
  }

  void accept(ExprVisitor &visitor) override { visitor.visit(this); }
};

class BinaryExpr : public Expr {
 public:
  std::unique_ptr<Expr> left;
  std::unique_ptr<Token> op;
  std::unique_ptr<Expr> right;

  BinaryExpr(std::unique_ptr<Expr> left, std::unique_ptr<Token> op,
             std::unique_ptr<Expr> right)
      : Expr(Expr::BinaryExprKind),
        left(std::move(left)),
        op(std::move(op)),
        right(std::move(right)) {}

  std::unique_ptr<Expr> clone() override {
    return llox::make_unique<BinaryExpr>(left->clone(), op->clone(),
                                         right->clone());
  }

  void accept(ExprVisitor &visitor) override { visitor.visit(this); }
};

class CallExpr : public Expr {
 public:
  std::unique_ptr<Expr> callee;
  std::unique_ptr<Token> paren;
  std::vector<std::unique_ptr<Expr>> arguments;

  CallExpr(std::unique_ptr<Expr> callee, std::unique_ptr<Token> paren,
           std::vector<std::unique_ptr<Expr>> &actual_arguments)
      : Expr(Expr::CallExprKind),
        callee(std::move(callee)),
        paren(std::move(paren)) {
    for (auto &elem : actual_arguments) {
      arguments.push_back(std::move(elem));
    }
  }

  std::unique_ptr<Expr> clone() override {
    std::vector<std::unique_ptr<Expr>> new_arguments;
    for (auto &arg : arguments) new_arguments.push_back(arg->clone());
    return llox::make_unique<CallExpr>(callee->clone(), paren->clone(),
                                       new_arguments);
  }

  void accept(ExprVisitor &visitor) override { visitor.visit(this); }
};

class GetExpr : public Expr {
 public:
  std::unique_ptr<Expr> object;
  std::unique_ptr<Token> name;

  GetExpr(std::unique_ptr<Expr> object, std::unique_ptr<Token> name)
      : Expr(Expr::GetExprKind),
        object(std::move(object)),
        name(std::move(name)) {}

  std::unique_ptr<Expr> clone() override {
    return llox::make_unique<GetExpr>(object->clone(), name->clone());
  }

  void accept(ExprVisitor &visitor) override { visitor.visit(this); }
};

class GroupingExpr : public Expr {
 public:
  std::unique_ptr<Expr> expression;

  GroupingExpr(std::unique_ptr<Expr> expression)
      : Expr(Expr::GroupingExprKind), expression(std::move(expression)) {}

  std::unique_ptr<Expr> clone() override {
    return llox::make_unique<GroupingExpr>(expression->clone());
  }

  void accept(ExprVisitor &visitor) override { visitor.visit(this); }
};

class BoolLiteralExpr : public Expr {
 public:
  bool value;

  BoolLiteralExpr(bool value) : Expr(Expr::BoolLiteralExprKind), value(value) {}

  std::unique_ptr<Expr> clone() override {
    return llox::make_unique<BoolLiteralExpr>(value);
  }

  void accept(ExprVisitor &visitor) override { visitor.visit(this); }
};

class NilLiteralExpr : public Expr {
 public:
  NilLiteralExpr() : Expr(Expr::NilLiteralExprKind) {}

  std::unique_ptr<Expr> clone() override {
    return llox::make_unique<NilLiteralExpr>();
  }

  void accept(ExprVisitor &visitor) override { visitor.visit(this); }
};

class NumberLiteralExpr : public Expr {
 public:
  double value;

  NumberLiteralExpr(double value)
      : Expr(Expr::NumberLiteralExprKind), value(value) {}

  std::unique_ptr<Expr> clone() override {
    return llox::make_unique<NumberLiteralExpr>(value);
  }

  void accept(ExprVisitor &visitor) override { visitor.visit(this); }
};

class StringLiteralExpr : public Expr {
 public:
  std::string value;

  StringLiteralExpr(std::string &value)
      : Expr(Expr::StringLiteralExprKind), value(value) {}

  std::unique_ptr<Expr> clone() override {
    return llox::make_unique<StringLiteralExpr>(value);
  }

  void accept(ExprVisitor &visitor) override { visitor.visit(this); }
};

class LogicalExpr : public Expr {
 public:
  std::unique_ptr<Expr> left;
  std::unique_ptr<Token> op;
  std::unique_ptr<Expr> right;

  LogicalExpr(std::unique_ptr<Expr> left, std::unique_ptr<Token> op,
              std::unique_ptr<Expr> right)
      : Expr(Expr::LogicalExprKind),
        left(std::move(left)),
        op(std::move(op)),
        right(std::move(right)) {}

  std::unique_ptr<Expr> clone() override {
    return llox::make_unique<LogicalExpr>(left->clone(), op->clone(),
                                          right->clone());
  }

  void accept(ExprVisitor &visitor) override { visitor.visit(this); }
};

class SetExpr : public Expr {
 public:
  std::unique_ptr<Expr> object;
  std::unique_ptr<Token> name;
  std::unique_ptr<Expr> value;

  SetExpr(std::unique_ptr<Expr> object, std::unique_ptr<Token> name,
          std::unique_ptr<Expr> value)
      : Expr(Expr::SetExprKind),
        object(std::move(object)),
        name(std::move(name)),
        value(std::move(value)) {}

  std::unique_ptr<Expr> clone() override {
    return llox::make_unique<SetExpr>(object->clone(), name->clone(),
                                      value->clone());
  }

  void accept(ExprVisitor &visitor) override { visitor.visit(this); }
};

class SuperExpr : public Expr {
 public:
  std::unique_ptr<Token> keyword;
  std::unique_ptr<Token> method;

  SuperExpr(std::unique_ptr<Token> keyword, std::unique_ptr<Token> method)
      : Expr(Expr::SuperExprKind),
        keyword(std::move(keyword)),
        method(std::move(method)) {}

  std::unique_ptr<Expr> clone() override {
    return llox::make_unique<SuperExpr>(keyword->clone(), method->clone());
  }

  void accept(ExprVisitor &visitor) override { visitor.visit(this); }
};

class ThisExpr : public Expr {
 public:
  std::unique_ptr<Token> keyword;

  ThisExpr(std::unique_ptr<Token> keyword)
      : Expr(Expr::ThisExprKind), keyword(std::move(keyword)) {}

  std::unique_ptr<Expr> clone() override {
    return llox::make_unique<ThisExpr>(keyword->clone());
  }

  void accept(ExprVisitor &visitor) override { visitor.visit(this); }
};

class UnaryExpr : public Expr {
 public:
  std::unique_ptr<Token> op;
  std::unique_ptr<Expr> right;

  UnaryExpr(std::unique_ptr<Token> op, std::unique_ptr<Expr> right)
      : Expr(Expr::UnaryExprKind), op(std::move(op)), right(std::move(right)) {}

  std::unique_ptr<Expr> clone() override {
    return llox::make_unique<UnaryExpr>(op->clone(), right->clone());
  }

  void accept(ExprVisitor &visitor) override { visitor.visit(this); }
};

class VariableExpr : public Expr {
 public:
  std::unique_ptr<Token> name;

  VariableExpr(std::unique_ptr<Token> name)
      : Expr(Expr::VariableExprKind), name(std::move(name)) {}

  std::unique_ptr<Expr> clone() override {
    return llox::make_unique<VariableExpr>(name->clone());
  }

  void accept(ExprVisitor &visitor) override { visitor.visit(this); }
};

template <typename T, typename... Args>
std::unique_ptr<Expr> make_expr(Args &&... args) {
  return std::unique_ptr<Expr>(new T(std::move(std::forward<Args>(args))...));
}

/// Statements.

class BlockStmt;
class ClassStmt;
class ExpressionStmt;
class FunctionStmt;
class IfStmt;
class PrintStmt;
class ReturnStmt;
class VarStmt;
class WhileStmt;

class StmtVisitor {
 public:
  virtual void visit(BlockStmt *stmt) = 0;
  virtual void visit(ClassStmt *stmt) = 0;
  virtual void visit(ExpressionStmt *stmt) = 0;
  virtual void visit(FunctionStmt *stmt) = 0;
  virtual void visit(IfStmt *stmt) = 0;
  virtual void visit(PrintStmt *stmt) = 0;
  virtual void visit(ReturnStmt *stmt) = 0;
  virtual void visit(VarStmt *stmt) = 0;
  virtual void visit(WhileStmt *stmt) = 0;
};

class Stmt {
 public:
  enum StmtKind {
    BlockStmtKind,
    ClassStmtKind,
    ExpressionStmtKind,
    FunctionStmtKind,
    IfStmtKind,
    PrintStmtKind,
    ReturnStmtKind,
    VarStmtKind,
    WhileStmtKind,
  };

  StmtKind kind;

  Stmt(StmtKind kind) : kind(kind) {}

  virtual ~Stmt() {}

  virtual std::unique_ptr<Stmt> clone() = 0;

  virtual void accept(StmtVisitor &visitor) = 0;
};

class BlockStmt : public Stmt {
 public:
  std::vector<std::unique_ptr<Stmt>> statements;

  BlockStmt(std::vector<std::unique_ptr<Stmt>> &block_statements)
      : Stmt(BlockStmtKind) {
    for (auto &stmt : block_statements) statements.push_back(std::move(stmt));
  }

  std::unique_ptr<Stmt> clone() override {
    std::vector<std::unique_ptr<Stmt>> new_statements;
    for (auto &stmt : statements) new_statements.push_back(stmt->clone());
    return llox::make_unique<BlockStmt>(new_statements);
  }

  void accept(StmtVisitor &visitor) override { visitor.visit(this); }
};

class ExpressionStmt : public Stmt {
 public:
  std::unique_ptr<Expr> expression;

  ExpressionStmt(std::unique_ptr<Expr> expression)
      : Stmt(ExpressionStmtKind), expression(std::move(expression)) {}

  std::unique_ptr<Stmt> clone() override {
    return llox::make_unique<ExpressionStmt>(expression->clone());
  }

  void accept(StmtVisitor &visitor) override { visitor.visit(this); }
};

class FunctionStmt : public Stmt {
 public:
  std::unique_ptr<Token> name;
  std::vector<std::unique_ptr<Token>> parameters;
  std::vector<std::unique_ptr<Stmt>> body;

  FunctionStmt(std::unique_ptr<Token> name,
               std::vector<std::unique_ptr<Token>> &function_parameters,
               std::vector<std::unique_ptr<Stmt>> &function_body)
      : Stmt(FunctionStmtKind) {
    for (auto &parameter : function_parameters)
      parameters.push_back(std::move(parameter));
    for (auto &stmt : function_body) body.push_back(std::move(stmt));
  }

  std::unique_ptr<Stmt> clone() override {
    std::vector<std::unique_ptr<Token>> new_parameters;
    for (auto &parameter : parameters)
      new_parameters.push_back(parameter->clone());
    std::vector<std::unique_ptr<Stmt>> new_body;
    for (auto &stmt : body) new_body.push_back(stmt->clone());
    return llox::make_unique<FunctionStmt>(name->clone(), new_parameters,
                                           new_body);
  }

  void accept(StmtVisitor &visitor) override { visitor.visit(this); }
};

class ClassStmt : public Stmt {
 public:
  std::unique_ptr<Token> name;
  std::unique_ptr<Expr> superclass;
  std::vector<std::unique_ptr<Stmt>> methods;

  ClassStmt(std::unique_ptr<Token> name, std::unique_ptr<Expr> superclass,
            std::vector<std::unique_ptr<Stmt>> &class_methods)
      : Stmt(BlockStmtKind),
        name(std::move(name)),
        superclass(std::move(superclass)) {
    for (auto &method : class_methods) methods.push_back(std::move(method));
  }

  std::unique_ptr<Stmt> clone() override {
    std::vector<std::unique_ptr<Stmt>> new_methods;
    for (auto &method : methods) new_methods.push_back(method->clone());
    return llox::make_unique<ClassStmt>(name->clone(), superclass->clone(),
                                        new_methods);
  }

  void accept(StmtVisitor &visitor) override { visitor.visit(this); }
};

class IfStmt : public Stmt {
 public:
  std::unique_ptr<Expr> condition;
  std::unique_ptr<Stmt> thenBranch;
  std::unique_ptr<Stmt> elseBranch;

  IfStmt(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> thenBranch,
         std::unique_ptr<Stmt> elseBranch)
      : Stmt(IfStmtKind),
        condition(std::move(condition)),
        thenBranch(std::move(thenBranch)),
        elseBranch(std::move(elseBranch)) {}

  std::unique_ptr<Stmt> clone() override {
    return llox::make_unique<IfStmt>(condition->clone(), thenBranch->clone(),
                                     elseBranch->clone());
  }

  void accept(StmtVisitor &visitor) override { visitor.visit(this); }
};

class PrintStmt : public Stmt {
 public:
  std::unique_ptr<Expr> expression;

  PrintStmt(std::unique_ptr<Expr> expression)
      : Stmt(PrintStmtKind), expression(std::move(expression)) {}

  std::unique_ptr<Stmt> clone() override {
    return llox::make_unique<PrintStmt>(expression->clone());
  }

  void accept(StmtVisitor &visitor) override { visitor.visit(this); }
};

class ReturnStmt : public Stmt {
 public:
  std::unique_ptr<Token> keyword;
  std::unique_ptr<Expr> value;

  ReturnStmt(std::unique_ptr<Token> keyword, std::unique_ptr<Expr> value)
      : Stmt(ReturnStmtKind),
        keyword(std::move(keyword)),
        value(std::move(value)) {}

  std::unique_ptr<Stmt> clone() override {
    return llox::make_unique<ReturnStmt>(keyword->clone(), value->clone());
  }

  void accept(StmtVisitor &visitor) override { visitor.visit(this); }
};

class VarStmt : public Stmt {
 public:
  std::unique_ptr<Token> name;
  std::unique_ptr<Expr> initializer;

  VarStmt(std::unique_ptr<Token> name, std::unique_ptr<Expr> initializer)
      : Stmt(VarStmtKind),
        name(std::move(name)),
        initializer(std::move(initializer)) {}

  std::unique_ptr<Stmt> clone() override {
    return llox::make_unique<VarStmt>(name->clone(), initializer->clone());
  }

  void accept(StmtVisitor &visitor) override { visitor.visit(this); }
};

class WhileStmt : public Stmt {
 public:
  std::unique_ptr<Expr> condition;
  std::unique_ptr<Stmt> body;

  WhileStmt(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> body)
      : Stmt(WhileStmtKind),
        condition(std::move(condition)),
        body(std::move(body)) {}

  std::unique_ptr<Stmt> clone() override {
    return llox::make_unique<WhileStmt>(condition->clone(), body->clone());
  }

  void accept(StmtVisitor &visitor) override { visitor.visit(this); }
};

typedef std::vector<std::unique_ptr<Stmt>> StmtList;

template <typename T, typename... Args>
std::unique_ptr<Stmt> make_stmt(Args &&... args) {
  return std::unique_ptr<Stmt>(new T(std::move(std::forward<Args>(args))...));
}

}  // namespace llox

#endif
