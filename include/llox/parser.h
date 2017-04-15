#ifndef LLOX_PARSER_H
#define LLOX_PARSER_H

#include "ast.h"
#include "scanner.h"

#include <vector>

namespace llox {

class Parser {
  std::unique_ptr<Scanner::TokenList> tokens;
  unsigned int current = 0;

 public:
  Parser(std::unique_ptr<Scanner::TokenList> tokens)
      : tokens(std::move(tokens)) {}

  std::unique_ptr<Expr> parse() { return expression(); }

 private:
  template <typename... TokenT>
  bool match(TokenT... tokens);

  std::unique_ptr<Expr> expression() { return assignment(); }

  bool check(TokenType type) {
    if (isAtEnd()) return false;
    return peek()->type == type;
  }

  Token *advance() {
    if (!isAtEnd()) current++;
    return previous();
  }

  bool isAtEnd() const { return peek()->type == END; }

  Token *peek() const { return tokens->at(current).get(); }

  Token *previous() const { return tokens->at(current - 1).get(); }

  std::unique_ptr<Token> releaseLastToken() {
    return std::move(tokens->at(current - 1));
  }

  std::unique_ptr<Expr> assignment();

  std::unique_ptr<Expr> land();

  std::unique_ptr<Expr> lor();

  std::unique_ptr<Expr> equality();

  std::unique_ptr<Expr> comparison();

  std::unique_ptr<Expr> term();

  std::unique_ptr<Expr> factor();

  std::unique_ptr<Expr> unary();

  std::unique_ptr<Expr> finishCallExpr(std::unique_ptr<Expr> callee);

  std::unique_ptr<Expr> call();

  std::unique_ptr<Expr> primary();

  bool consume(TokenType type, const std::string &message);
};

}  // namespace llox

#endif
