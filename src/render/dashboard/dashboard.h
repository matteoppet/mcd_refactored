#pragma once

#include "imgui.h"
#include "../../satellite/satellite.h"

#include <vector>
#include <cfloat>
#include <string>


class Dashboard {
    private:
        std::string selected_satellite_norad = "";
        int view_mode = 0; // 0=3D, 1=2D
        char search_filter[128] = "";

    public:
        void draw_view_mode();
        void draw_search_bar();
        void draw_list_satellites(std::vector<SatelliteObject>& satellitesList);
        void draw_add_delete_buttons_satellites();

        std::string get_selected_satellite_norad() {
            return selected_satellite_norad;
        }
};