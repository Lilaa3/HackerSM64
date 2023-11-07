#include <cassert>
#include <fstream>

#include <fmt/core.h>
#include <fmt/std.h>

#include "lila_c_parser.hpp"

using namespace LCP;

int LCP::test_parser() {
    Parser parser;
    parser.read_c_file_at_path(TEST_PATH);
    return EXIT_SUCCESS;
}

string region(string str, size_t start, size_t end) {
    assert(start <= str.size() && end <= str.size() && start <= end);
    return str.substr(start, end - start);
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
    //fmt::print("{}\n", text);

    this->text = text;

    this->prev_char = ' ';
    for (this->index = 0; this->index < text.size(); this->index++) {
        if (this->index > 0)
            this->prev_char = text[this->index - 1];
        else
            this->prev_char = EMPTY_CHAR;
        if (this->index < text.size() - 1)
            this->next_char = text[this->index + 1];
        else
            this->next_char = EMPTY_CHAR;
        this->character = text[this->index];

        this->process_char();
    }
    return EXIT_SUCCESS;
}

void Parser::process_char() {
    this->handle_comments(); // Skips comments completly.

    // Handle macros here

    if (this->state & State::READING_KEYWORDS)
        this->read_keywords();
    if (this->state & State::READING_VALUES)
        this->read_values();
}

void Parser::handle_comments() {
    if (this->character == '/' && (this->next_char == '/' || this->next_char == '*')) { // Entered a comment
        size_t start = this->index;

        long int end = -1;
        if (this->next_char == '/')
            end = this->text.find("\n", start);
        else 
            end = this->text.find("*/", start) + 2;

        assert(end != -1);
        this->index = end;
        fmt::print("Comment starts at {}, end at {}:\n\"{}\"\n", start, end, region(this->text, start, end));
    }
}

void Parser::read_keywords() {
}

void Parser::read_values() {
}