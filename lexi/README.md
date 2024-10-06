<p align="center">
  <img alt="dream logo" src=".github/lexi.svg" style="width: 25%">   
</p>

<p align="center" style="height: 32px">
  <img align="middle" alt="license" src="https://img.shields.io/github/license/SwampPear/blam.svg">
  <img align="middle" alt="last commit" src="https://img.shields.io/github/last-commit/SwampPear/blam.svg">
  <img align="middle" style="height: 21px" alt="built with love <3" src="http://ForTheBadge.com/images/badges/built-with-love.svg">
</p>

# Lexi

Lexi is a compact tokenization library intended for use in compilers and
lexical parsers.

### Usage

```
#include <filesystem>
#include "lexi.hpp"

int main() {
    // read file to string
    std::string path = std::filesystem::current_path()/"main.blam";
    std::string src = Lexi::readFile(path);

    // build tokenizer
    Lexi::Tokenizer tokenizer = Lexi::Tokenizer();
    tokenizer.addRule("STRING", "\"[a-zA-Z0-9\\s\\}]*\"");
    tokenizer.addRule("SINGLE_LINE_COMMENT", "\\/\\/[\\sa-zA-Z0-9]*\n*");
    tokenizer.addRule("MULTI_LINE_COMMENT", "\\/\\*[\\sa-zA-Z0-9]*\\*\\/");
    tokenizer.addRule("L_DELIMETER", "\\(");
    tokenizer.addRule("R_DELIMETER", "\\)");
    tokenizer.addRule("L_CURLY_DELIMETER", "\\{");
    tokenizer.addRule("R_CURLY_DELIMETER", "\\}");
    tokenizer.addRule("L_SQUARE_DELIMETER", "\\[");
    tokenizer.addRule("R_SQUARE_DELIMETER", "\\]");
    tokenizer.addRule("OP_DOT", "\\.");
    tokenizer.addRule("OP_PLUS", "\\+");
    tokenizer.addRule("OP_MINUS", "\\-");
    tokenizer.addRule("OP_EQUALS", "=");
    tokenizer.addRule("OP_SLASH", "/");
    tokenizer.addRule("KEYWORD", "return");
    tokenizer.addRule("SPACE", "\\s+");
    
    // tokenize
    Lexi::TokenNode root = tokenizer.tokenize(&src);

    std::cout << tokenizer.tokenToString(root, true) << std::endl;

    return EXIT_SUCCESS;
}
```
