#include "llox/parser.h"

#include <iostream>
#include <vector>

using namespace llox;

Expr *Parser::equality() {
  Expr *expr = comparison();
  if (!expr) return nullptr;

  while (match(BANG_EQUAL, EQUAL_EQUAL)) {
    Token *op = releaseLastToken();
    Expr *right = comparison();
    if (!right) return nullptr;
    expr = new BinaryExpr(expr, op, right);
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

// comparison -> term ( ( ">" | ">=" | "<" | "<=" ) term )*
Expr *Parser::comparison() {
  Expr *expr = term();
  if (!expr) return nullptr;

  while (match(GREATER, GREATER_EQUAL, LESS, LESS_EQUAL)) {
    Token *op = releaseLastToken();
    Expr *right = term();
    if (!right) return nullptr;
    expr = new BinaryExpr(expr, op, right);
  }

  return expr;
}

// term -> factor ( ( "-" | "+" ) factor )*
Expr *Parser::term() {
  Expr *expr = factor();
  if (!expr) return nullptr;

  while (match(MINUS, PLUS)) {
    Token *op = releaseLastToken();
    Expr *right = factor();
    if (!right) return nullptr;
    expr = new BinaryExpr(expr, op, right);
  }

  return expr;
}

// factor -> unary ( ( "/" | "*" ) unary )*
Expr *Parser::factor() {
  Expr *expr = unary();
  if (!expr) return nullptr;

  while (match(SLASH, STAR)) {
    Token *op = releaseLastToken();
    Expr *right = unary();
    if (!right) return nullptr;
    expr = new BinaryExpr(expr, op, right);
  }

  return expr;
}

// unary -> ( "-" | "!" ) expression | primary
Expr *Parser::unary() {
  if (match(MINUS, BANG)) {
    Token *op = releaseLastToken();
    Expr *expr = expression();
    if (!expr) return nullptr;
    return new UnaryExpr(op, expr);
  }

  return primary();
}

// primary -> NUMBER | STRING | "false" | "true" | "nil"
//          | "(" expression ")"
Expr *Parser::primary() {
  if (match(NUMBER)) {
    NumberToken *token = static_cast<NumberToken *>(releaseLastToken());
    return new NumberLiteralExpr(token->literal);
  }

  if (match(STRING)) {
    StringToken *token = static_cast<StringToken *>(releaseLastToken());
    return new StringLiteralExpr(token->literal);
  }

  if (match(FALSE)) return new BoolLiteralExpr(false);

  if (match(TRUE)) return new BoolLiteralExpr(true);

  if (match(NIL)) return new NilLiteralExpr();

  if (match(LEFT_PAREN)) {
    Expr *expr = expression();
    if (!expr) return nullptr;
    if (!consume(RIGHT_PAREN, "Expect ')' after expression.")) return nullptr;
    return new GroupingExpr(expr);
  }

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
