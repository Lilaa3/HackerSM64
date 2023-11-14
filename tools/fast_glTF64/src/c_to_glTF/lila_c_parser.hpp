#pragma once

/*  GUIDELINES
    MUST be able to handle functions, even if not even close to perfectly, just so other data can be
   safely parsed MUST only rely on easy to install dependencies/submodules (libclang im looking at
   you!!!!) MUST be able to parse the provided test file after any changes Should be easy to read the
   parsed data. Magic values should be defined here. (EMPTY_CHAR for example) Specific choices of
   standard types should generally be type defined here for simplicity (char_t = char32_t, string =
   std::string, etc) Bit flags should be prefered over lots of variables according to context, for this
   use the BIT lambada. All debuging should be optinal. Code should be easily portable to not just other
   architectures but also different languages (as in, no c++ only tricks). Constant values should always
   be initialized with const. Formatting should be kept simple, just the default vs code formatting.
    Naming conventions are as follow:
      UPPER_SNAKE_CASE is used for constants and lambadas.
      snake_case is used for variables and functions (also typedef variables, except for classes)
      PascalCase for classes (and structs)
*/

#include <filesystem>
#include <map>
#include <string>
#include <any>

namespace LCP {
typedef std::filesystem::path Path;
typedef std::string string;
typedef char8_t char_t;

const char_t EMPTY_CHAR = U'\0';

const auto BIT = [](int i) { return 1 << i; };

enum DebugFlags {
    PRINT_PARSING_TEXT = BIT(0),
    PRINT_FILE = BIT(1),
    PRINT_COMMENTS = BIT(2),
    PRINT_TOKENS = BIT(3),
    RECOMMENDED = PRINT_PARSING_TEXT,
    ALL = UINT_MAX,
};

enum IfMacroType {
    IF,
    IFDEF,
    IFNDEF,
};

class IfMacro {
  public:
    IfMacroType type;
    string value;

    operator std::string() const {
        return fmt::format("TO IMPLEMENT");
    }
};

enum ParsedType { ARRAY, MACRO_CALL, INT, FLOAT, STRING };

class ParsedValue {
  public:
    size_t start, end;
    IfMacro if_def;
    ParsedType type;
    std::any value;

    ParsedValue(size_t start = 0, size_t end = 0, IfMacro if_def = IfMacro(), ParsedType type = ParsedType(), std::any value = std::any()) {
      this->start = start;
      this->end = end;
      this->if_def = if_def;
      this->type = type;
      this->value = value;
    }
    virtual operator std::string() const {
        return fmt::format("TO IMPLEMENT");
    }
};

class ParsedValues {
  public:
    std::vector<std::shared_ptr<ParsedValues>> values;

    virtual void push_back(std::shared_ptr<ParsedValues> value) {
      this->values.push_back(value);
    }
  
    virtual operator std::string() const {
        return fmt::format("TO IMPLEMENT");
    }
};

class Initialization : public ParsedValues {
  public:
    bool is_static;
    bool is_const;
    std::vector<string> keywords;
    operator std::string() const {
        return fmt::format("TO IMPLEMENT");
    }
};

class FirstPassToken {
  public:
    size_t start;
    size_t end;
    string text_value;
    FirstPassToken(size_t start = 0, size_t end = 0, string text_value = "") {
        this->start = start;
        this->end = end;
        this->text_value = text_value;
    }
    operator std::string() const {
        return fmt::format("{}, {}: \"{}\"", this->start, this->end, this->text_value);
    }
};

const FirstPassToken EMPTY_TOKEN = FirstPassToken();

const Path TEST_PATH("parser_test_file.c");
int test_parser();

class Parser {
  public:
    DebugFlags debug_flags = DebugFlags::RECOMMENDED;

    int read_c_file_at_path(const Path &path);
    int read_c_data(const string &text);
    std::vector<std::shared_ptr<Initialization>> values;

  private:
    size_t index;

    char_t prev_chr, chr, next_chr;
    string text;
    size_t token_start;
    bool in_string;
    std::vector<FirstPassToken> tokens;
    void read_tokens();
    void handle_comments();
    void handle_tokens();

    const FirstPassToken &prev_token() {
        if (this->index > 0)
            return this->tokens[this->index - 1];
        return EMPTY_TOKEN;
    }
    const FirstPassToken &cur_token() {
        return this->tokens[this->index];
    }
    const FirstPassToken &next_token() {
        if (this->index < this->tokens.size() - 1)
            return this->tokens[this->index + 1];
        return EMPTY_TOKEN;
    }
    std::vector<std::shared_ptr<ParsedValues>> stack;
    std::shared_ptr<Initialization> cur_initializer;
    std::vector<FirstPassToken> accumelated_tokens;
    const std::shared_ptr<ParsedValues> &stack_back() {
      return this->stack.back();
    }
  
    string get_tabs();
    void read_data();
    void read_keywords();
    void read_values();
    bool reading_keywords, reading_function;
};
}; // namespace LCP