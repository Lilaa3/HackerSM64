#include <fmt/color.h>

const auto YELLOW_COLOR = fg(fmt::rgb(255, 255, 0));
const auto BLUE_COLOR = fg(fmt::rgb(120, 120, 255));
const auto RED_COLOR = fg(fmt::rgb(255, 0, 0));
const auto GREEN_COLOR = fg(fmt::rgb(0, 255, 0));

const auto SUCESS_COLOR = GREEN_COLOR;
const auto INFO_COLOR = YELLOW_COLOR;
const auto INFO2_COLOR = BLUE_COLOR; // For some color variaty and readability
const auto ERROR_COLOR = RED_COLOR;