#pragma once

#include <iostream>

#include "raylib.h" 
#include "raymath.h"

class CustomCamera : public Camera3D {
    public:
        Vector3 cameraOffset = { 0.0f, 4.0f, 10.0f };

        CustomCamera() {
            position = { 0.0f, 4.0f, 10.0f };  // Camera position
            target = { 0.0f, 0.0f, 0.0f };     // Camera looking at point
            up = { 0.0f, 1.0f, 0.0f };         // Camera up vector (rotation towards target)
            fovy = 45.0f;                      // Camera field-of-view Y
            projection = CAMERA_PERSPECTIVE;  // Camera mode type
        };
        
};