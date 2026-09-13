#include "parse_data.h"

#include <curl/curl.h>

// callback function to write the response data to a string
size_t writeCallback(char* ptr, size_t size, size_t nmemb, std::string* data) {
    data->append(ptr, size * nmemb);
    return size * nmemb;
}

// get telemetry data from celestrak api and return it as a string
std::string parse_telemetry_from_api(const std::string& url) {
    CURL* curl = curl_easy_init();
    std::string response;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    return response;
}


void create_txt_file_from_api(const std::string& url, const std::string txt_path) {
    std::string telemetryData = parse_telemetry_from_api(url);
    std::ofstream txtFile(txt_path, std::ios::binary);
    if (!txtFile.is_open()) {
        std::cerr << "Failed to open TXT file for writing: " << txt_path << std::endl;
        return;
    }

    std::istringstream ss(telemetryData);
    std::string line;
    while (std::getline(ss, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        txtFile << line << "\n";
    }
    txtFile.close();
}


void append_new_satellite_to_file(std::string& name, std::string& tle1, std::string& tle2) {
    std::ofstream outfile("../assets/data.txt", std::ios_base::app); 
    
    if (!name.empty() && name.back() == '\r') name.pop_back();
    if (!tle1.empty() && tle1.back() == '\r') tle1.pop_back();
    if (!tle2.empty() && tle2.back() == '\r') tle2.pop_back();

    if (outfile.is_open()) {
        std::cout << name << std::endl;
        outfile << "\n" << name << "\n" << tle1 << "\n" << tle2;
        outfile.close();
    } else {
        std::cerr << "Error: Could not open assets/data.txt for appending." << std::endl;
    }
}