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

SatelliteObject* SATELLITE_SELECTED = nullptr;
int view_mode = 0; // 0: 3D | 1: 2D

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

        if (SATELLITE_SELECTED) {
            std::cout << SATELLITE_SELECTED->name << std::endl;
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
                    if (ImGui::BeginTabBar("Mission Control"))
                    {  
                        if (ImGui::BeginTabItem("Mission Control"))
                        {
                            dashboard.draw_view_mode();
                            ImGui::Separator();
                            dashboard.draw_search_bar();
                            SATELLITE_SELECTED = dashboard.draw_list_satellites(get_all_satellites());
                            dashboard.draw_add_delete_buttons_satellites();
                            ImGui::EndTabItem();
                        }

                        if (ImGui::BeginTabItem("Camera Settings"))
                        {
                            dashboard.draw_camera_settings(&camera);
                            ImGui::Separator();
                            dashboard.draw_reset_button(&camera);
                            ImGui::EndTabItem();
                        }
                        ImGui::EndTabBar();
                    } 


                ImGui::End();

                if (SATELLITE_SELECTED) {
                    ImGui::SetNextWindowPos(ImVec2(ImGui::GetWindowWidth()-200, 100), ImGuiCond_FirstUseEver);
                    ImGui::Begin("Satellite Info", NULL, ImGuiWindowFlags_AlwaysAutoResize);
                        dashboard.draw_satellite_name(SATELLITE_SELECTED);
                        ImGui::Separator();
                        dashboard.draw_satellite_info(SATELLITE_SELECTED);
                        ImGui::Separator();
                        dashboard.draw_track_satellite_button();
                    ImGui::End();   
                }

            rlImGuiEnd();

        EndDrawing();
    }

    simulator.de_initialization();

    telemetry.stop();
    CloseWindow();
    return 0;
}