#include "satellite.h"

namespace {
    std::vector<SatelliteObject> g_all_satellites;
}

std::vector<SatelliteObject>& get_all_satellites() {
    return g_all_satellites;
}

void load_satellites(std::string_view file_path, std::string_view api_url) {
    std::ifstream file{std::string(file_path)};
    if (!file.is_open()) {
        std::cout << "Satellites file not found, creating it from API" << std::endl;
        create_txt_file_from_api(std::string(api_url), std::string(file_path));

        file.open(std::string(file_path)); 
        if (!file.is_open()) {
            std::cerr << "Fatal error: Still cannot open file after downloading." << std::endl;
            return;
        }
    } else {
        std::cout << "Satellites file found, opening it..." << std::endl;
    }

    std::string name, tle1, tle2;

    while (std::getline(file, name), std::getline(file, tle1), std::getline(file, tle2)) {
        while (!name.empty() && (name.back() == '\r' || name.back() == ',')) name.pop_back();
        while (!tle1.empty() && (tle1.back() == '\r' || tle1.back() == ',')) tle1.pop_back();
        while (!tle2.empty() && (tle2.back() == '\r' || tle2.back() == ',')) tle2.pop_back();

        SatelliteObject sat;
        sat.name = name;
        sat.tle1 = tle1;
        sat.tle2 = tle2;
        sat.norad_id = tle1.substr(2, 5);

        create_sgp4_object(sat);

        std::cout << "Loaded satellite: " << name << std::endl;
        g_all_satellites.push_back(sat);
    }
    
    file.close();
}

void create_sgp4_object(SatelliteObject& satellite) {
    try {
        libsgp4::Tle tle = libsgp4::Tle(satellite.tle1, satellite.tle2);

        satellite.inclination_rad = tle.Inclination(true);
        satellite.eccentricity = tle.Eccentricity();
        satellite.period = 1440.0 / tle.MeanMotion();

        satellite.spg4_object.emplace(tle);
    } catch (libsgp4::SatelliteException& e) {
        std::cerr << "Error initializing SGP4 for satellite " << satellite.name << ": " << e.what() << std::endl;
    }
}

std::mutex& get_satellite_mutex() {
    static std::mutex mtx; 
    return mtx;
}