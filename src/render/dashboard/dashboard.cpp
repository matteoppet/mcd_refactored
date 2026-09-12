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
    ImGui::Button("+ Add Satellite", ImVec2(-FLT_MIN, 0.0f));
    ImGui::PopStyleColor(3);

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.60f, 0.15f, 0.15f, 1.0f));        
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.80f, 0.20f, 0.20f, 1.0f)); 
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.50f, 0.10f, 0.10f, 1.0f));
    ImGui::Button("- Delete Satellite", ImVec2(-FLT_MIN, 0.0f));
    ImGui::PopStyleColor(3);
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