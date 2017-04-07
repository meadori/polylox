#include <iostream>
#include <memory>

#include "llox/scanner.h"

using namespace llox;

void Scanner::scanToken() {
  char c = advance();

  switch (c) {
    case '(':
      addToken(TokenType::LEFT_PAREN);
      break;
    case ')':
      addToken(TokenType::RIGHT_PAREN);
      break;
    case '{':
      addToken(TokenType::LEFT_BRACE);
      break;
    case '}':
      addToken(TokenType::RIGHT_BRACE);
      break;
    case ',':
      addToken(TokenType::COMMA);
      break;
    case '.':
      addToken(TokenType::DOT);
      break;
    case '-':
      addToken(TokenType::MINUS);
      break;
    case '+':
      addToken(TokenType::PLUS);
      break;
    case ';':
      addToken(TokenType::SEMICOLON);
      break;
    case '*':
      addToken(TokenType::STAR);
      break;
    case '!':
      addToken(match('=') ? BANG_EQUAL : BANG);
      break;
    case '=':
      addToken(match('=') ? EQUAL_EQUAL : EQUAL);
      break;
    case '<':
      addToken(match('=') ? LESS_EQUAL : LESS);
      break;
    case '>':
      addToken(match('=') ? GREATER_EQUAL : GREATER);
      break;
    case '/':
      if (match('/')) {
        // A comment goes until the end of the line.
        while (peek() != '\n' && !isAtEnd()) advance();
      } else {
        addToken(SLASH);
      }
      break;
    case ' ':
    case '\r':
    case '\t':
      // Ignore whitespace.
      break;
    case '\n':
      line++;
      break;
    case '"':
      string();
      break;
    default:
      if (isDigit(c)) {
        number();
      } else if (isAlpha(c)) {
        identifier();
      } else {
        std::cerr << "error\n";
      }
      break;
  }
}

std::unique_ptr<std::vector<Scanner::TokenPtr>> Scanner::scanTokens() {
  while (!isAtEnd()) {
    start = current;
    scanToken();
  }

  tokens->emplace_back(new Token(TokenType::END, "", line));
  return std::move(tokens);
}

void Scanner::string() {
  while (peek() != '"' && !isAtEnd()) {
    if (peek() == '\n') line++;
    advance();
  }

  // Unterminated string.
  if (isAtEnd()) {
    std::cerr << "error\n";
    return;
  }

  // The closing ".
  advance();

  // Trim the surrounding quotes.
  std::string value = source.substr(start + 1, current - start - 2);
  addStringToken(value);
}

void Scanner::number() {
  while (isDigit(peek())) advance();

  // Look for a fractional part.
  if (peek() == '.' && isDigit(peekNext())) {
    // Consume the "."
    advance();

    while (isDigit(peek())) advance();
  }

  std::string value = source.substr(start, current - start);
  addNumberToken(std::stod(value));
}

void Scanner::identifier() {
  while (isAlphaNumeric(peek())) advance();

  // See if the identifier is a reserved word.
  std::string text = source.substr(start, current - start);

  TokenType type = IDENTIFIER;
  auto typeIt = keywords.find(text);
  if (typeIt != keywords.end()) type = typeIt->second;

  addToken(type);
}
