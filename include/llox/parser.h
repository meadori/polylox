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

  std::unique_ptr<Expr> parse() { return std::unique_ptr<Expr>(expression()); }

 private:
  template <typename... TokenT>
  bool match(TokenT... tokens);

  Expr *expression() { return assignment(); }

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

  Token *releaseLastToken() { return tokens->at(current - 1).release(); }

  Expr *assignment();

  Expr *land();

  Expr *lor();

  Expr *equality();

  Expr *comparison();

  Expr *term();

  Expr *factor();

  Expr *unary();

  Expr *primary();

  bool consume(TokenType type, const std::string &message);
};

}  // namespace llox

#endif
