#pragma once

/*  GUIDELINES
    MUST be able to handle functions, even if not even close to perfectly, just so other data can be safely parsed
    MUST only rely on easy to install dependencies/submodules (libclang im looking at you!!!!)
    MUST be able to parse the provided test file after any changes
    Should be easy to read the parsed data.
    Magic values should be defined here. (EMPTY_CHAR for example)
    Specific choices of standard types should generally be type defined here for simplicity (char_t = char32_t, string = std::string, etc)
    Bit flags should be prefered over lots of variables according to context, for this use the BIT lambada.
    All debuging should be optinal.
    Code should be easily portable to not just other architectures but also different languages (as in, no c++ only tricks).
    Constant values should always be initialized with const.
    Formatting should be kept simple, just the default vs code formatting.
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
typedef char32_t char_t;

const char_t EMPTY_CHAR = U'\0';

const auto BIT = [](int i) { return 1 << i; };

enum State {
    READING_KEYWORDS = BIT(0),
    READING_VALUES = BIT(1),
};

class ParsedValue {
  public:
    int to_do;
};

class Initialization : public ParsedValue {
  public:
    std::vector<string> types;
};

const Path TEST_PATH("parser_test_file.c");
int test_parser();

class Parser {
  public:
    int read_c_file_at_path(const Path &path);
    int read_c_data(const string &text);
    std::vector<Initialization> initializers;

  private:
    // Must be reset for every file
    size_t index;
    char_t prev_char, character, next_char;
    string text;
    int state;

    std::vector<ParsedValue> stack;

    void process_char();
    void handle_comments();
    void read_keywords();
    void read_values();
};
}; // namespace LCP