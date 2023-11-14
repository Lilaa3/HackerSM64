#include <cassert>
#include <fstream>

#include <fmt/core.h>
#include <fmt/std.h>

#include "lila_c_parser.hpp"

using namespace LCP;

int LCP::test_parser() {
    Parser parser;
    parser.debug_flags = DebugFlags::ALL;
    parser.read_c_file_at_path(TEST_PATH);
    return EXIT_SUCCESS;
}

string erase_from_string(const string &str, const string &to_erase) {
    string result = str;
    size_t pos = result.find(to_erase);
    while (pos != string::npos) { // while the substring is found
        result.erase(pos, to_erase.length());
        pos = result.find(to_erase);
    }
    return result;
}

string erase_spaces(const string &str) {
    string result = str;
    result.erase(remove(result.begin(), result.end(), ' '), result.end());
    result.erase(remove(result.begin(), result.end(), '\t'), result.end());
    result.erase(remove(result.begin(), result.end(), '\n'), result.end());
    return result;
}

string region(string &str, size_t start, size_t end) {
    assert(start <= str.size() && end <= str.size() && start <= end);
    return str.substr(start, end - start);
}

string trim_start(string &str, size_t start) {
    return region(str, start, str.size());
}

string read_utf8_file(const Path &path) {
    std::ifstream file(path, std::ios::in | std::ios::binary);
    if (file.is_open()) {
        // Read the file contents into a string
        string file_contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

        file.close();

        return file_contents;
    } else {
        throw std::runtime_error("Error opening the file.");
    }
}
int Parser::read_c_file_at_path(const Path &path) {
    string text = read_utf8_file(path);
    this->read_c_data(text);
    return EXIT_SUCCESS;
}

int Parser::read_c_data(const string &text) {
    this->text = text;

    if (this->debug_flags & DebugFlags::PRINT_PARSING_TEXT)
        fmt::print("Parsing provided text, {} characters\n", this->text.size());

    if (this->debug_flags & DebugFlags::PRINT_FILE)
        fmt::print("{}\n", this->text);

    this->read_tokens();
    if (this->debug_flags & DebugFlags::PRINT_TOKENS) {
        fmt::print("Tokens have been read sucessfuly\n");

        for (const auto &token : this->tokens) {
            fmt::print("{}\n", (string) token);
        }
    }

    this->read_data();

    return EXIT_SUCCESS;
}

void Parser::read_tokens() {
    this->token_start = 0;
    this->in_string = false;
    this->tokens.clear();
    for (this->index = 0; this->index < text.size(); this->index++) {
        if (this->index > 0)
            this->prev_chr = text[this->index - 1];
        else
            this->prev_chr = EMPTY_CHAR;
        if (this->index < text.size() - 1)
            this->next_chr = text[this->index + 1];
        else
            this->next_chr = EMPTY_CHAR;
        this->chr = text[this->index];

        this->handle_comments();
        this->handle_tokens();
    }
}

const string MULTI_COMMENT_END = "*/";
void Parser::handle_comments() {
    if (this->chr == '/' && (this->next_chr == '/' || this->next_chr == '*')) { // Entered a comment
        const size_t start = this->index;

        long int end; // Yes this should be a constant, gcc is stupid however and it can´t understand it
                      // if i initialize the exact same variable in an if and else
        if (this->next_chr == '/')
            end = this->text.find("\n", start);
        else
            end = this->text.find(MULTI_COMMENT_END, start) + MULTI_COMMENT_END.size();

        if (end == -1)
            end = this->text.size() - 1;

        this->index = end;

        if (start >= this->token_start)
            this->token_start = end;

        if (this->debug_flags & DebugFlags::PRINT_COMMENTS)
            fmt::print("Comment, {}, {}: \"{}\"\n", start, end, region(this->text, start, end));
    }
}

void Parser::handle_tokens() {
    const std::string delimiters = " \n;{}()[],&^#:\"'|/%*->.+-><";
    bool chr_is_delimiter = delimiters.find(this->chr) != std::string::npos;
    bool prev_chr_is_delimiter = delimiters.find(this->prev_chr) != std::string::npos;

    if (chr_is_delimiter || prev_chr_is_delimiter || this->next_chr == EMPTY_CHAR) {
        if (this->prev_chr == '-' || this->chr == '>')
            return;

        if (this->chr == '"' || this->chr == '\'') {
            this->in_string = !this->in_string;
            if (!this->in_string) {
                FirstPassToken token = FirstPassToken(
                    this->token_start, this->index,
                    erase_from_string(region(this->text, this->token_start, this->index), "\\\n"));
                this->tokens.push_back(token);
            }

            FirstPassToken token = FirstPassToken(this->index, this->index + 1, "\"");
            this->tokens.push_back(token);

            this->token_start = this->index + 1;
            return;
        }
        if (this->in_string)
            return;

        if (this->next_chr == EMPTY_CHAR) // Lil hack for tokens right at the end of the text
            this->index++;

        string possible_token = erase_spaces(region(this->text, this->token_start, this->index));
        if (possible_token.size() > 0) {
            FirstPassToken token = FirstPassToken(this->token_start, this->index, possible_token);
            this->tokens.push_back(token);
        }
        this->token_start = this->index;
    }
}

void Parser::read_data() {
    this->stack.clear();

    this->cur_initializer = std::make_shared<Initialization>();
    this->reading_keywords = true;
    this->reading_function = false;

    for (this->index = 0; this->index < this->tokens.size(); this->index++) {
        if (this->reading_keywords) {
            this->read_keywords();
        }
        if (!this->reading_keywords) {
            this->read_values();
        }
    }
}

string Parser::get_tabs() {
    string tabs = "";
    for (size_t i = 0; i < this->stack.size(); i++) {
        tabs += "\t";
    }
    return tabs;
}

void Parser::read_keywords() {
    string token = this->cur_token().text_value;
    if (token == "=" || token == "{") {
        this->values.push_back(this->cur_initializer);
        if (this->prev_token().text_value == ")" && token == "{") {
            this->reading_function = true;
        }
        this->stack.push_back(this->cur_initializer);
        this->cur_initializer = std::make_shared<Initialization>();
        this->reading_keywords = false;
        fmt::print("{}Entering value\n", this->get_tabs());
    }
    else {
        fmt::print("{}\n", token);
        this->cur_initializer->keywords.push_back(token);
    }
}

void Parser::read_values() {
    string token_string = this->cur_token().text_value;
    if (token_string == "=") // Ignore just for now.
        return;

    if (token_string == "{" || token_string == "(") {
        fmt::print("{}Entering stack\n", this->get_tabs());
        auto array = std::make_shared<ParsedValues>();
        this->stack_back()->push_back(array);
        this->stack.push_back(array);
    } else if (token_string == "}" || token_string == ")"
               || (token_string == ";" && !this->reading_function)) {
        this->stack.pop_back();
        if (this->stack.size() == 1 && this->reading_function){
            fmt::print("{}Exiting stack because of funciton\n", this->get_tabs());
            this->stack.pop_back();
        }
        if (this->stack.size() == 0) {
            this->reading_function = false;
            this->reading_keywords = true;
        }
        fmt::print("{}Exiting stack\n", this->get_tabs());
    }
    else {
        this->accumelated_tokens.push_back(this->cur_token());
        fmt::print("{}{}\n", this->get_tabs(), token_string);
    }
}