#include <fstream>
#include <iostream>

#include "settings.hpp"
#include "c_to_glTF/lila_c_parser.hpp"

const std::filesystem::path settings_path = "settings.json";

int main() {
    auto settings = read_settings_file(settings_path);
    LCP::test_parser();
    return EXIT_SUCCESS;
}