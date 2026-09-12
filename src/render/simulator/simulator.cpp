#include "simulator.h"

void Simulator::draw_earth() {
    DrawModel(earth.model, earth.position, 1.0f, WHITE);
}

void Simulator::draw_satellites(std::vector<SatelliteObject>& satellitesList) {
    for (auto& satellite : satellitesList) {
        VectorForRaylib sim_pos = satellite.get_position_in_sim(SCALE);
    
        Vector3 raylib_pos = { sim_pos.x, sim_pos.y, sim_pos.z };
        // DrawSphere(raylib_pos, 0.05f, RED); 
        DrawModel(satellite_model, raylib_pos, 0.02f, RED);
    }
}

void Simulator::de_initialization() {
    UnloadTexture(earth.texture);
    UnloadModel(earth.model);

    UnloadModel(satellite_model);
}