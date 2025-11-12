#include <fstream>
#include <iostream>
#include <streambuf>
#include <string>
#include <vector>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "lox/ast-printer.h"
#include "lox/ast.h"
#include "lox/interpreter.h"
#include "lox/parser.h"
#include "lox/scanner.h"
#include "lox/token.h"

ABSL_FLAG(bool, print_ast, false,
          "Print the Abstract Syntax Tree (AST) of the input file.");

static void run(const std::string& source, llox::Interpreter& interpreter) {
  llox::Scanner scanner(source);
  std::unique_ptr<llox::Scanner::TokenList> tokens = scanner.scanTokens();
  llox::Parser parser(std::move(tokens));
  std::unique_ptr<llox::StmtList> statements = parser.parse();

  if (statements) {
    bool should_print_ast = absl::GetFlag(FLAGS_print_ast);

    if (should_print_ast) {
      llox::AstPrinter printer;
      std::cout << printer.print(*statements) << "\n";
    } else {
      interpreter.interpret(*statements);
    }
  }
}

static void runFile(const char* path) {
  llox::Interpreter interpreter;
  std::ifstream t(path);
  std::string str((std::istreambuf_iterator<char>(t)),
                  std::istreambuf_iterator<char>());
  run(str, interpreter);
}

static void runPrompt() {
  llox::Interpreter interpreter;
  for (;;) {
    std::cout << "> ";

    std::string source;
    std::getline(std::cin, source);
    run(source, interpreter);
  }
}

int main(int argc, char** argv) {
  std::vector<char*> non_flag_args = absl::ParseCommandLine(argc, argv);

  if (non_flag_args.size() > 2) {
    std::cerr << "usage: " << non_flag_args[0]
              << " --print-ast=<true|false> <input_file>\n";
    return 1;
  } else if (non_flag_args.size() == 2) {
    runFile(non_flag_args[1]);
  } else {
    runPrompt();
  }
  return 0;
}
