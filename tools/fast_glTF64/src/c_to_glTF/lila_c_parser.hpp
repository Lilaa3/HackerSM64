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
#include <string>

namespace LCP {
typedef std::filesystem::path Path;
typedef std::string string;
typedef char8_t char_t;


const char_t EMPTY_CHAR = U'\0';

const auto BIT = [](int i) { return 1 << i; };

enum StateFlags {
    READING_TOKENS = BIT(0),
    READING_VALUES = BIT(1),
    START = READING_TOKENS,
};

enum DebugFlags {
    PRINT_PARSING_TEXT = BIT(0),
    PRINT_FILE = BIT(1),
    PRINT_COMMENTS = BIT(2),
    PRINT_TOKENS = BIT(3),
    RECOMMENDED = PRINT_PARSING_TEXT,
    ALL = UINT_MAX,
};

class ParsedValue {
  public:
    int to_do;
};

class Initialization : public ParsedValue {
  public:
    std::vector<string> types;
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

const Path TEST_PATH("parser_test_file.c");
int test_parser();

class Parser {
  public:
    DebugFlags debug_flags = DebugFlags::RECOMMENDED;

    int read_c_file_at_path(const Path &path);
    int read_c_data(const string &text);
    std::vector<Initialization> initializers;

  private:
    // Must be reset for every file
    size_t index;
    char_t prev_chr, chr, next_chr;
    string text;
    StateFlags state = StateFlags::START;

    size_t token_start;
    bool in_string;
    std::vector<FirstPassToken> tokens;

    std::vector<ParsedValue> stack;

    void read_tokens();
    void handle_comments();
    void handle_tokens();
    

    void read_values();
};
}; // namespace LCP