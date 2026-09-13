#pragma once

#include "imgui.h"
#include "../../satellite/satellite.h"
#include "../camera.h"
#include "../../parse_data.h"
#include "libsgp4/Tle.h"

#include <vector>
#include <cfloat>
#include <string>
#include <algorithm>
#include <iostream>
#include <exception>


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
        void draw_popup_modal_add_satellite();
        void draw_invalid_tle_popup();

        void draw_camera_settings(CustomCamera* camera);
        void draw_reset_button(CustomCamera* camera);

        std::string get_selected_satellite_norad() {
            return selected_satellite_norad;
        }
};

// TODO: delete satellite button