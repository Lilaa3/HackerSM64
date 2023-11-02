#include <fmt/core.h>
#include <fmt/std.h>
#include <fmt/color.h>

#include "settings.hpp"

using namespace SM64_fast_glTF64;
using json = nlohmann::json;

ConverterSettings read_c_settings_file(std::filesystem::path path) {
    fmt::print(INFO_COLOR, "Reading c to glTF settings json.\n");
    std::ifstream settings_file;
    try {
        fmt::print(INFO2_COLOR, "Reading as text({}).\n", path);
        settings_file.open(path);

        if (!settings_file) { // If file was not open, print an error
            fmt::print(ERROR_COLOR, "File does not exist.\n");
            throw;
        }
    } catch (const std::exception &e) {
        fmt::print(ERROR_COLOR, "Unhandled exception while reading the file:\n{}\n", e.what());
        throw e;
    }

    json json_data;
    try {
        fmt::print(INFO2_COLOR, "Parsing the json text into data.\n");
        json_data = json::parse(settings_file,
                                /* callback */ nullptr,
                                /* allow exceptions */ false,
                                /* ignore_comments */ true);
    } catch (const std::exception &e) {
        fmt::print(ERROR_COLOR, "Unhandled exception while parsing from json:\n{}\n", e.what());
        throw e;
    }

    ConverterSettings settings;
    try {
        fmt::print(INFO2_COLOR, "Interperting the parsed data.\n");
        settings.read_from_json(json_data);
    } catch (const std::exception &e) {
        fmt::print(ERROR_COLOR,
                   "Unhandled exception while reading the parsed json data into classes:\n{}\n",
                   e.what());
        throw e;
    }

    fmt::print(SUCESS_COLOR, "Read and parsed sucefully!\n");

    if (settings.base.general.debug_prints)
        fmt::print("Settings {}\n", (std::string) settings);

    return settings;
}