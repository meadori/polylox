#include "llox/parser.h"
#include "llox/util.h"

#include <iostream>
#include <vector>

using namespace llox;

std::unique_ptr<Expr> Parser::equality() {
  std::unique_ptr<Expr> expr = comparison();
  if (!expr) return nullptr;

  while (match(BANG_EQUAL, EQUAL_EQUAL)) {
    std::unique_ptr<Token> op = releaseLastToken();
    std::unique_ptr<Expr> right = comparison();
    if (!right) return nullptr;
    expr = llox::make_expr<BinaryExpr>(expr, op, right);
  }

  return expr;
}

template <typename... TokenT>
bool Parser::match(TokenT... types) {
  std::vector<TokenType> typevec = {types...};

  for (TokenType type : typevec) {
    if (check(type)) {
      advance();
      return true;
    }
  }

  return false;
}

// assignment -> or ( "=" assignment )?
std::unique_ptr<Expr> Parser::assignment() {
  std::unique_ptr<Expr> expr = lor();

  if (match(EQUAL)) {
    std::unique_ptr<Token> equals = releaseLastToken();
    std::unique_ptr<Expr> value = assignment();
    if (!value) return nullptr;

    switch (expr->kind) {
      case Expr::VariableExprKind: {
        std::unique_ptr<Token> name =
            static_cast<VariableExpr *>(expr.get())->name->clone();
        expr = llox::make_expr<AssignExpr>(name, value);
        break;
      }
      case Expr::GetExprKind: {
        GetExpr *variable = static_cast<GetExpr *>(expr.get());
        std::unique_ptr<Token> name = variable->name->clone();
        std::unique_ptr<Expr> object = variable->object->clone();
        expr = llox::make_expr<SetExpr>(object, name, value);
        break;
      }
      default:
        // TODO: Create a proper error handling abstraction.
        std::cerr << "error: Invalid assignment target.\n";
        return nullptr;
        break;
    }
  }

  return expr;
}

// or -> and ( "or" and )*
std::unique_ptr<Expr> Parser::lor() {
  std::unique_ptr<Expr> expr = land();

  while (match(OR)) {
    std::unique_ptr<Token> op = releaseLastToken();
    std::unique_ptr<Expr> right = land();
    if (!right) return nullptr;
    expr = llox::make_expr<LogicalExpr>(expr, op, right);
  }

  return expr;
}

// and -> equality ( "and" equality )*
std::unique_ptr<Expr> Parser::land() {
  std::unique_ptr<Expr> expr = equality();

  while (match(AND)) {
    std::unique_ptr<Token> op = releaseLastToken();
    std::unique_ptr<Expr> right = equality();
    if (!right) return nullptr;
    expr = llox::make_expr<LogicalExpr>(expr, op, right);
  }

  return expr;
}

// comparison -> term ( ( ">" | ">=" | "<" | "<=" ) term )*
std::unique_ptr<Expr> Parser::comparison() {
  std::unique_ptr<Expr> expr = term();
  if (!expr) return nullptr;

  while (match(GREATER, GREATER_EQUAL, LESS, LESS_EQUAL)) {
    std::unique_ptr<Token> op = releaseLastToken();
    std::unique_ptr<Expr> right = term();
    if (!right) return nullptr;
    expr = llox::make_expr<BinaryExpr>(expr, op, right);
  }

  return expr;
}

// term -> factor ( ( "-" | "+" ) factor )*
std::unique_ptr<Expr> Parser::term() {
  std::unique_ptr<Expr> expr = factor();
  if (!expr) return nullptr;

  while (match(MINUS, PLUS)) {
    std::unique_ptr<Token> op = releaseLastToken();
    std::unique_ptr<Expr> right = factor();
    if (!right) return nullptr;
    expr = llox::make_expr<BinaryExpr>(expr, op, right);
  }

  return expr;
}

// factor -> unary ( ( "/" | "*" ) unary )*
std::unique_ptr<Expr> Parser::factor() {
  std::unique_ptr<Expr> expr = unary();
  if (!expr) return nullptr;

  while (match(SLASH, STAR)) {
    std::unique_ptr<Token> op = releaseLastToken();
    std::unique_ptr<Expr> right = unary();
    if (!right) return nullptr;
    expr = llox::make_expr<BinaryExpr>(expr, op, right);
  }

  return expr;
}

// unary -> ( "-" | "!" ) expression | primary
std::unique_ptr<Expr> Parser::unary() {
  if (match(MINUS, BANG)) {
    std::unique_ptr<Token> op = releaseLastToken();
    std::unique_ptr<Expr> expr = expression();
    if (!expr) return nullptr;
    return llox::make_expr<UnaryExpr>(op, expr);
  }

  return call();
}

std::unique_ptr<Expr> Parser::finishCallExpr(std::unique_ptr<Expr> callee) {
  std::vector<std::unique_ptr<Expr>> arguments;

  if (!check(RIGHT_PAREN)) {
    do {
      if (arguments.size() >= 8) {
        std::cerr << "error: Cannot have more than 8 arguments.\n";
        return nullptr;
      }
      std::unique_ptr<Expr> expr = expression();
      if (expr)
        arguments.push_back(std::move(expr));
      else
        return nullptr;
    } while (match(COMMA));
  }

  if (!consume(RIGHT_PAREN, "Expect ')' after arguments.")) return nullptr;

  return std::unique_ptr<CallExpr>(new CallExpr(
      std::move(callee), std::move(releaseLastToken()), arguments));
}

std::unique_ptr<Expr> Parser::call() {
  std::unique_ptr<Expr> expr = primary();

  while (true) {
    if (match(LEFT_PAREN)) {
      expr = finishCallExpr(std::move(expr));
    } else if (match(DOT)) {
      if (consume(IDENTIFIER, "Expect property name after '.'."))
        expr = llox::make_expr<GetExpr>(expr, releaseLastToken());
    } else {
      break;
    }
  }

  return expr;
}

// primary -> NUMBER | STRING | "false" | "true" | "nil"
//          | "(" expression ")"
std::unique_ptr<Expr> Parser::primary() {
  if (match(NUMBER)) {
    std::unique_ptr<Token> number = releaseLastToken();
    double literal = static_cast<NumberToken *>(number.get())->literal;
    return llox::make_unique<NumberLiteralExpr>(literal);
  }

  if (match(STRING)) {
    std::unique_ptr<Token> string = releaseLastToken();
    std::string literal = static_cast<StringToken *>(string.get())->literal;
    return llox::make_unique<StringLiteralExpr>(literal);
  }

  if (match(SUPER)) {
    std::unique_ptr<Token> keyword = releaseLastToken();
    if (!consume(DOT, "Expect '.' after 'super'.")) return nullptr;
    if (!consume(IDENTIFIER, "Expect superclass method name.")) return nullptr;
    std::unique_ptr<Token> method = releaseLastToken();
    return llox::make_expr<SuperExpr>(keyword, method);
  }

  if (match(FALSE)) return llox::make_unique<BoolLiteralExpr>(false);

  if (match(TRUE)) return llox::make_unique<BoolLiteralExpr>(true);

  if (match(NIL)) return llox::make_unique<NilLiteralExpr>();

  if (match(THIS)) {
    std::unique_ptr<Token> keyword = releaseLastToken();
    return llox::make_expr<ThisExpr>(keyword);
  }

  if (match(IDENTIFIER)) {
    std::unique_ptr<Token> name = releaseLastToken();
    return llox::make_expr<VariableExpr>(name);
  }

  if (match(LEFT_PAREN)) {
    std::unique_ptr<Expr> expr = expression();
    if (!expr) return nullptr;
    if (!consume(RIGHT_PAREN, "Expect ')' after expression.")) return nullptr;
    return llox::make_expr<GroupingExpr>(expr);
  }

  std::cerr << "Expect expression.\n";

  return nullptr;
}

bool Parser::consume(TokenType type, const std::string &message) {
  if (check(type)) {
    advance();
    return true;
  }

  // TODO: Create a proper error handling abstraction.
  std::cerr << "error: " << message << "\n";
  return false;
}
