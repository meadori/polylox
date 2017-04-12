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

// assignment -> or ( "=" assignment )?
Expr *Parser::assignment() {
  Expr *expr = lor();

  if (match(EQUAL)) {
    std::unique_ptr<Token> equals(releaseLastToken());
    Expr *value = assignment();
    if (!value) return nullptr;

    switch (expr->kind) {
      case Expr::VariableExprKind: {
        VariableExpr *variable = static_cast<VariableExpr *>(expr);
        Token *name = variable->name->clone();
        expr = new AssignExpr(name, value);
        break;
      }
      case Expr::GetExprKind: {
        GetExpr *variable = static_cast<GetExpr *>(expr);
        Token *name = variable->name->clone();
        Expr *object = variable->object->clone();
        expr = new SetExpr(variable->object.get(), name, value);
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
Expr *Parser::lor() {
  Expr *expr = land();

  while (match(OR)) {
    Token *op = releaseLastToken();
    Expr *right = land();
    if (!right) return nullptr;
    expr = new LogicalExpr(expr, op, right);
  }

  return expr;
}

// and -> equality ( "and" equality )*
Expr *Parser::land() {
  Expr *expr = equality();

  while (match(AND)) {
    Token *op = releaseLastToken();
    Expr *right = equality();
    if (!right) return nullptr;
    expr = new LogicalExpr(expr, op, right);
  }

  return expr;
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

  return call();
}

Expr *Parser::finishCallExpr(Expr *callee) {
  std::vector<Expr *> arguments;

  if (!check(RIGHT_PAREN)) {
    do {
      if (arguments.size() >= 8) {
        std::cerr << "error: Cannot have more than 8 arguments.\n";
        return nullptr;
      }
      Expr *expr = expression();
      if (expr)
        arguments.push_back(expr);
      else
        return nullptr;
    } while (match(COMMA));
  }

  if (!consume(RIGHT_PAREN, "Expect ')' after arguments.")) return nullptr;

  return new CallExpr(callee, releaseLastToken(), arguments);
}

Expr *Parser::call() {
  Expr *expr = primary();

  while (true) {
    if (match(LEFT_PAREN)) {
      expr = finishCallExpr(expr);
    } else if (match(DOT)) {
      if (consume(IDENTIFIER, "Expect property name after '.'."))
        expr = new GetExpr(expr, releaseLastToken());
    } else {
      break;
    }
  }

  return expr;
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

  if (match(IDENTIFIER)) return new VariableExpr(releaseLastToken());

  if (match(LEFT_PAREN)) {
    Expr *expr = expression();
    if (!expr) return nullptr;
    if (!consume(RIGHT_PAREN, "Expect ')' after expression.")) return nullptr;
    return new GroupingExpr(expr);
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
