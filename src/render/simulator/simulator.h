#pragma once

#include <iostream>
#include <vector>

#include "../src/satellite/satellite.h"

#include "raylib.h"
#include "raymath.h"

struct Earth {
    Texture2D texture;
    Model model;

    Vector3 position;

    static constexpr float EARTH_RADIUS = 1.5f;
    static constexpr float EARTH_RADIUS_KM_REAL = 6371.0f;

    void init() {
        position = { 0.0f, 0.0f, 0.0f };

        texture = LoadTexture("../assets/earth_texture.png");
        model = LoadModelFromMesh(GenMeshSphere(EARTH_RADIUS, 62, 62)); 
        model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
    };   
};

class Simulator {
    private:
        Earth earth;
        const float SCALE = Earth::EARTH_RADIUS / Earth::EARTH_RADIUS_KM_REAL;

        Model satellite_model;

    public:
        Simulator() {
        };

        void init() {
            earth.init();
            satellite_model = LoadModel("../assets/simple_satellite.glb");
        }

        void draw_earth();
        void draw_satellites(std::vector<SatelliteObject>& satellitesList);
        void de_initialization();
};
