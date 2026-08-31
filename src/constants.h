#pragma once

#include <iostream>
#include <string_view>

constexpr int WINDOW_SIZE_X = 1240;
constexpr int WINDOW_SIZE_Y = 720;
constexpr int TARGET_FPS = 60;

constexpr std::string_view SATELLITES_FILE_PATH = "../../assets/data.txt";
constexpr std::string_view API_URL = "https://celestrak.org/NORAD/elements/gp.php?GROUP=stations&FORMAT=tle";