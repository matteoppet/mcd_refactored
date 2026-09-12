#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <string_view>
#include <optional>
#include <mutex>

#include "libsgp4/SGP4.h"

#include "../parse_data.h"

struct VectorForRaylib {
    float x;
    float y;
    float z;
};

struct SatelliteObject {
    std::string name;
    std::string tle1;
    std::string tle2;
    std::string norad_id;

    std::optional<libsgp4::SGP4> spg4_object;
    libsgp4::Vector position_eci = {0, 0, 0}; // real data
    libsgp4::Vector velocity_eci = {0, 0, 0}; // read data

    double lat_radians;
    double lon_radians;
    double alt_km;
    double inclination_rad;
    double eccentricity;
    double period;

    void update(libsgp4::DateTime timestamp) {
        libsgp4::Eci eci = spg4_object->FindPosition(timestamp);

        position_eci = eci.Position();
        velocity_eci = eci.Velocity();

        libsgp4::CoordGeodetic geo = eci.ToGeodetic();
        lat_radians = geo.latitude;
        lon_radians = geo.longitude;
        alt_km = geo.altitude;
    }

    VectorForRaylib get_position_in_sim(const float SCALE) {
        return {
            (float)position_eci.x * SCALE,
            (float)position_eci.y * SCALE,
            (float)position_eci.z * SCALE
        };
    }

    double get_velocity_kms() const {
        return std::sqrt(std::pow(velocity_eci.x, 2) + 
                         std::pow(velocity_eci.y, 2) + 
                         std::pow(velocity_eci.z, 2));
    }
    double get_velocity_kmh() const {
        return get_velocity_kms() * 3600.0;
    }
};

void create_sgp4_object(SatelliteObject& satellite);
void load_satellites(std::string_view file_path, std::string_view api_url);
std::vector<SatelliteObject>& get_all_satellites();
std::mutex& get_satellite_mutex();