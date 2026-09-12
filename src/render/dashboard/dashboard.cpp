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
    ImGui::InputText("##SearchInput", search_filter, IM_ARRAYSIZE(search_filter));
}

void Dashboard::draw_list_satellites(std::vector<SatelliteObject>& satellitesList) {
    float spaceNeededForButtons = ImGui::GetFrameHeightWithSpacing() * 2.0f;
    ImVec2 listBoxSize(-FLT_MIN, ImGui::GetContentRegionAvail().y - spaceNeededForButtons);

    if (ImGui::BeginListBox("##SatList", listBoxSize)) {
        for (auto& satellite : satellitesList) {
            const bool isSelected = (selected_satellite_norad == satellite.norad_id);

            if (ImGui::Selectable(satellite.name.c_str(), isSelected)) {
                selected_satellite_norad = satellite.norad_id; 
            }

            if (isSelected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndListBox();
    }
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