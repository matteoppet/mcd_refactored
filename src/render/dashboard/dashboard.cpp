#include "dashboard.h"


void Dashboard::draw_view_mode() {
    ImGui::AlignTextToFramePadding();
    ImGui::Text("View Mode");
    ImGui::SameLine();
    ImGui::RadioButton("3D", &view_mode, 0);
    ImGui::SameLine();
    ImGui::RadioButton("2D", &view_mode, 1);
}


void Dashboard::draw_search_bar() {
    ImGui::Text("List Satellites");

    ImGui::AlignTextToFramePadding();
    ImGui::Text("Search: ");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(-1.0f);
    if (ImGui::InputTextWithHint("##SearchBox", "Search satellite by name", search_filter.InputBuf, IM_ARRAYSIZE(search_filter.InputBuf)))
    {
        search_filter.Build();
    }
}


SatelliteObject* Dashboard::draw_list_satellites(std::vector<SatelliteObject>& satellitesList) {
    float spaceNeededForButtons = ImGui::GetFrameHeightWithSpacing() * 2.0f;
    ImVec2 listBoxSize(-FLT_MIN, ImGui::GetContentRegionAvail().y - spaceNeededForButtons);

    SatelliteObject* satellite_selected = nullptr;

    if (ImGui::BeginListBox("##SatList", listBoxSize)) {
        for (auto& satellite : satellitesList) {
            if (search_filter.PassFilter(satellite.name.c_str())) {
                const bool isSelected = (selected_satellite_norad == satellite.norad_id);

            if (ImGui::Selectable(satellite.name.c_str(), isSelected)) {
                selected_satellite_norad = satellite.norad_id; 
                satellite_selected = &satellite;
            }

            if (isSelected) {
                ImGui::SetItemDefaultFocus();
                satellite_selected = &satellite;
            }
            }
        }
        ImGui::EndListBox();
    }

    return satellite_selected;
}


void Dashboard::draw_add_delete_buttons_satellites() {
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.15f, 0.50f, 0.15f, 1.0f));        
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.20f, 0.65f, 0.20f, 1.0f)); 
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.10f, 0.40f, 0.10f, 1.0f));

    if (ImGui::Button("+ Add Satellite", ImVec2(-FLT_MIN, 0.0f))) {
        ImGui::OpenPopup("Add New Satellite");
    }
    draw_popup_modal_add_satellite();

    ImGui::PopStyleColor(3);

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.60f, 0.15f, 0.15f, 1.0f));        
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.80f, 0.20f, 0.20f, 1.0f)); 
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.50f, 0.10f, 0.10f, 1.0f));
    
    if (ImGui::Button("- Delete Satellite", ImVec2(-FLT_MIN, 0.0f))) {
        // display a pop-up window
    }

    ImGui::PopStyleColor(3);
}


void Dashboard::draw_popup_modal_add_satellite() {
    if (ImGui::BeginPopupModal("Add New Satellite", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        static char name[128] = "";
        static char tle1[128] = "";
        static char tle2[128] = "";
        ImVec4 blue_text_tles = ImVec4(0.00f, 0.82f, 1.00f, 1.0f);
        
        ImGui::Text("Insert TLE data using the NORAD Two-Line Element Set format:");
        ImGui::Text("Example TLE(1): ");
        ImGui::SameLine();
        ImGui::TextColored(blue_text_tles, "1 54216U 22143A   26241.25605179  .00019583  00000+0  24462-3 0  9996");
        ImGui::Text("Example TLE(2): ");
        ImGui::SameLine();
        ImGui::TextColored(blue_text_tles, "2 54216  41.4675 238.4409 0001837 253.6235 106.4402 15.59376380305105");
        ImGui::Separator(); 
        
        // --- first input field ----
        ImGui::Text("Insert Name:   ");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(-FLT_MIN);
        ImGui::InputText("##NAME", name, IM_ARRAYSIZE(name));
        // --- second input field ----
        ImGui::Text("Insert TLE(1): ");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(-FLT_MIN);
        ImGui::InputText("##TLE1", tle1, IM_ARRAYSIZE(tle1));
        // --- third input field ----
        ImGui::Text("Insert TLE(2): ");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(-FLT_MIN);
        ImGui::InputText("##TLE2", tle2, IM_ARRAYSIZE(tle2));
        ImGui::Separator();

        if (ImGui::Button("Confirm", ImVec2(120, 0))) {
            std::string strName(name);
            std::string strTle1(tle1);
            std::string strTle2(tle2);

            try {
                libsgp4::Tle testTle(strName, strTle1, strTle2);
                append_new_satellite_to_file(strName, strTle1, strTle2);

                SatelliteObject new_sat;
                new_sat.name = strName;
                new_sat.tle1 = strTle1;
                new_sat.tle2 = strTle2;
                create_sgp4_object(new_sat);

                get_all_satellites().push_back(new_sat);
                
                name[0] = '\0';
                tle1[0] = '\0';
                tle2[0] = '\0';
                ImGui::CloseCurrentPopup();

            } catch (const std::exception& e) {
                ImGui::OpenPopup("Invalid TLE Error");
            }
        }
        
        ImGui::SameLine();
        
        if (ImGui::Button("Cancel", ImVec2(120, 0))) {
            name[0] = '\0';
            tle1[0] = '\0';
            tle2[0] = '\0';
            ImGui::CloseCurrentPopup();
        }

        draw_invalid_tle_popup();

        ImGui::EndPopup();
    }
}


void Dashboard::draw_invalid_tle_popup() {
    if (ImGui::BeginPopupModal("Invalid TLE Error", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Error: Invalid TLE Format!");
        ImGui::Text("The data provided failed the NORAD checksum or length validation.");
        ImGui::Separator();

        if (ImGui::Button("OK", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}


void Dashboard::draw_satellite_info(SatelliteObject* sat_selected) {
    // altitude
    ImGui::Text("Alt: ");
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.15f, 0.50f, 0.15f, 1.0f));
    ImGui::Text(std::to_string(sat_selected->alt_km).c_str());
    ImGui::PopStyleColor(1);
    // velocity
    ImGui::Text("Vel: ");
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.15f, 0.50f, 0.15f, 1.0f));
    ImGui::Text(std::to_string(sat_selected->get_velocity_kms()).c_str());
    ImGui::PopStyleColor(1);
    ImGui::SameLine();
    ImGui::Text("km/s");
    // eccentricity
    ImGui::Text("Ecc: ");
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.15f, 0.50f, 0.15f, 1.0f));
    ImGui::Text(std::to_string(sat_selected->eccentricity).c_str());
    ImGui::PopStyleColor(1);
    // inclination 
    ImGui::Text("Inc: ");
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.15f, 0.50f, 0.15f, 1.0f));
    ImGui::Text(std::to_string(sat_selected->inclination_rad).c_str());
    ImGui::PopStyleColor(1);
    ImGui::SameLine();
    ImGui::Text("rad");
}


void Dashboard::draw_track_satellite_button() {
    ImGui::Button("Track Satellite", ImVec2(-FLT_MIN, 0.0f));
}


void Dashboard::draw_satellite_name(SatelliteObject* sat_selected) {
    ImGui::Text("Selected: ");
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.15f, 0.50f, 0.15f, 1.0f));
    ImGui::Text(sat_selected->name.c_str());
    ImGui::PopStyleColor(1);
    ImGui::Text("Norad ID: ");
    ImGui::SameLine();
    ImGui::Text(sat_selected->norad_id.c_str());
}


void Dashboard::draw_camera_settings(CustomCamera* camera) {
    ImGui::AlignTextToFramePadding();
    ImGui::Text("FOV");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(-FLT_MIN);
    ImGui::SliderFloat("##CameraFov", &camera->fovy, 1.0f, 120.0f);

    ImGui::AlignTextToFramePadding();
    ImGui::Text("Camera Mode");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(-FLT_MIN);
    int projIndex = (camera->projection == CAMERA_PERSPECTIVE) ? 0 : 1;
    if (ImGui::Combo("##CameraProjection", &projIndex, "Perspective\0Orthographic\0\0")) {
        if (projIndex == 0) {
            camera->projection = CAMERA_PERSPECTIVE;
        } else {
            camera->projection = CAMERA_ORTHOGRAPHIC;
        }
    }
}


void Dashboard::draw_reset_button(CustomCamera* camera) {
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.60f, 0.15f, 0.15f, 1.0f));        
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.80f, 0.20f, 0.20f, 1.0f)); 
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.50f, 0.10f, 0.10f, 1.0f));
    if (ImGui::Button("Reset Position", ImVec2(-FLT_MIN, 0.0f))) {
        // reset position
    }
    ImGui::PopStyleColor(3);
}