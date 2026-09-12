#include <iostream>

#include "raylib.h"
#include "rlImGui.h"
#include "imgui.h"

#include "constants.h"
#include "satellite/satellite.h"
#include "render/simulator/simulator.h"
#include "render/camera.h"
#include "second_thread/second_thread.h"
#include "render/dashboard/dashboard.h"



int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WINDOW_SIZE_X, WINDOW_SIZE_Y, "Mission Control Dashboard");
    SetTargetFPS(TARGET_FPS);
    TraceLog(LOG_INFO, "CURRENT WORKING DIRECTORY: %s", GetWorkingDirectory());

    rlImGuiSetup(true);

    std::cout << "Loading satellites..." << std::endl;
    load_satellites(SATELLITES_FILE_PATH, API_URL);
    std::cout << "Loaded " << get_all_satellites().size() << " satellites" << std::endl; 

    CustomCamera camera;
    Simulator simulator;
    WorkerManager telemetry;
    Dashboard dashboard;

    telemetry.start();

    simulator.init();

    while (!WindowShouldClose()) {
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
            UpdateCamera(&camera, CAMERA_THIRD_PERSON);
        }
        if (IsKeyPressed(KEY_SPACE)) {
            if (telemetry.IsRunning()) telemetry.stop();
            else telemetry.start(); 
        }

        BeginDrawing();
            ClearBackground(BLACK);

            BeginMode3D(camera);
                simulator.draw_earth();
                simulator.draw_satellites(get_all_satellites());
                DrawGrid(10, 1.0f);
            EndMode3D();

            rlImGuiBegin();
                ImGui::Begin("Mission Control");

                    // PANEL ONE
                    dashboard.draw_view_mode();
                    ImGui::Separator();
                    dashboard.draw_search_bar();
                    dashboard.draw_list_satellites(get_all_satellites());
                    dashboard.draw_add_delete_buttons_satellites();

                ImGui::End();
            rlImGuiEnd();

        EndDrawing();


    }

    simulator.de_initialization();

    telemetry.stop();
    CloseWindow();
    return 0;
}

// TODO: Create ImGui setup

// TODO: after ImGui, update graphic of Raylib