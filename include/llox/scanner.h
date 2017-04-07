#ifndef LLOX_SCANNER_H
#define LLOX_SCANNER_H

#include <map>
#include <string>
#include <vector>

#include "token.h"

namespace llox {

class Token;

class Scanner {
 public:
  typedef std::unique_ptr<Token> TokenPtr;

 private:
  std::string source;

  std::unique_ptr<std::vector<TokenPtr>> tokens;

  std::map<std::string, TokenType> keywords;

  unsigned int start = 0;

  unsigned int current = 0;

  unsigned int line = 1;

 public:
  Scanner(const std::string &source) : source(source) {
    tokens.reset(new std::vector<TokenPtr>());

    keywords["and"] = TokenType::AND;
    keywords["class"] = TokenType::CLASS;
    keywords["else"] = ELSE;
    keywords["false"] = FALSE;
    keywords["for"] = FOR;
    keywords["fun"] = FUN;
    keywords["if"] = IF;
    keywords["nil"] = NIL;
    keywords["or"] = OR;
    keywords["print"] = PRINT;
    keywords["return"] = RETURN;
    keywords["super"] = SUPER;
    keywords["this"] = THIS;
    keywords["true"] = TRUE;
    keywords["var"] = VAR;
    keywords["while"] = WHILE;
  }

  std::unique_ptr<std::vector<TokenPtr>> scanTokens();

 private:
  void scanToken();

  char advance() {
    current += 1;
    return source[current - 1];
  }

  void addToken(TokenType type) {
    std::string text = source.substr(start, current - start);
    tokens->emplace_back(new Token(type, text, line));
  }

  void addStringToken(const std::string &literal) {
    std::string text = source.substr(start, current - start);
    tokens->emplace_back(new StringToken(text, line, literal));
  }

  void addNumberToken(double literal) {
    std::string text = source.substr(start, current - start);
    tokens->emplace_back(new NumberToken(text, line, literal));
  }

  bool isAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
  }

  bool isAlphaNumeric(char c) { return isAlpha(c) || isDigit(c); }

  bool isAtEnd() const { return current >= source.size(); }

  bool isDigit(char c) const { return c >= '0' && c <= '9'; }

  bool match(char expected) {
    if (isAtEnd()) return false;
    if (source[current] != expected) return false;

    current += 1;

    return true;
  }

  char peek() {
    if (current >= source.size()) return '\0';
    return source[current];
  }

  char peekNext() {
    if (current + 1 >= source.size()) return '\0';
    return source[current + 1];
  }

  void string();

  void number();

  void identifier();
};

}  // namespace llox

#endif
