#include <fstream>
#include <iostream>

#include "settings.hpp"

const std::filesystem::path settings_path = "settings.json";

int main() {
    auto settings = read_c_settings_file(settings_path);
    return EXIT_SUCCESS;
}