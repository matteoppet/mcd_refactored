#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

size_t writeCallback(char* ptr, size_t size, size_t nmemb, std::string* data);
std::string parse_telemetry_from_api(const std::string& url);
void create_txt_file_from_api(const std::string& url, const std::string txt_path);