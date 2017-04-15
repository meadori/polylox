#ifndef LLOX_TOKEN_H
#define LLOX_TOKEN_H

#include "util.h"

#include <string>

namespace llox {

enum TokenType {
  // Single-character tokens.
  LEFT_PAREN,
  RIGHT_PAREN,
  LEFT_BRACE,
  RIGHT_BRACE,
  COMMA,
  DOT,
  MINUS,
  PLUS,
  SEMICOLON,
  SLASH,
  STAR,

  // One or two character tokens.
  BANG,
  BANG_EQUAL,
  EQUAL,
  EQUAL_EQUAL,
  GREATER,
  GREATER_EQUAL,
  LESS,
  LESS_EQUAL,

  // Literals.
  IDENTIFIER,
  STRING,
  NUMBER,

  // Keywords.
  AND,
  CLASS,
  ELSE,
  FALSE,
  FUN,
  FOR,
  IF,
  NIL,
  OR,
  PRINT,
  RETURN,
  SUPER,
  THIS,
  TRUE,
  VAR,
  WHILE,

  END
};

class Token {
 public:
  TokenType type;
  std::string lexeme;
  unsigned int line;

  Token(TokenType type, const std::string &lexeme, unsigned int line)
      : type(type), lexeme(lexeme), line(line) {}

  virtual std::unique_ptr<Token> clone() const {
    return llox::make_unique<Token>(type, lexeme, line);
  }

  virtual std::string str() const {
    return std::to_string(type) + " " + lexeme;
  }
};

class StringToken : public Token {
 public:
  std::string literal;

  StringToken(const std::string &lexeme, unsigned int line,
              const std::string &literal)
      : Token(STRING, lexeme, line), literal(literal) {}

  std::unique_ptr<Token> clone() const override {
    return llox::make_unique<StringToken>(lexeme, line, literal);
  }

  std::string str() const override {
    return std::to_string(type) + " " + lexeme + " " + literal;
  }
};

class NumberToken : public Token {
 public:
  double literal;

  NumberToken(const std::string &lexeme, unsigned int line, double literal)
      : Token(NUMBER, lexeme, line), literal(literal) {}

  std::unique_ptr<Token> clone() const override {
    return llox::make_unique<NumberToken>(lexeme, line, literal);
  }

  std::string str() const override {
    return std::to_string(type) + " " + lexeme + " " + std::to_string(literal);
  }
};

std::ostream &operator<<(std::ostream &out, const Token &token);

}  // namespace llox

#endif
