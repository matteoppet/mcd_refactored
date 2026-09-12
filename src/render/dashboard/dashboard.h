#pragma once

#include "imgui.h"
#include "../../satellite/satellite.h"

#include <vector>
#include <cfloat>
#include <string>
#include <algorithm>
#include <iostream>


class Dashboard {
    private:
        std::string selected_satellite_norad = "";
        int view_mode = 0; // 0=3D, 1=2D
        ImGuiTextFilter search_filter;

    public:
        void draw_view_mode();
        void draw_search_bar();
        SatelliteObject* draw_list_satellites(std::vector<SatelliteObject>& satellitesList);
        void draw_add_delete_buttons_satellites();
        
        void draw_satellite_name(SatelliteObject* sat_selected);
        void draw_satellite_info(SatelliteObject* sat_selected);
        void draw_track_satellite_button();

        std::string get_selected_satellite_norad() {
            return selected_satellite_norad;
        }
};

// TODO: selecetd satellite panel