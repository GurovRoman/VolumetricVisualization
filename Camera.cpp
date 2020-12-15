#include "Camera.h"
#include <cmath>

Vector Camera::GetCameraPixelPosition(int x, int y, int pixel_width, int pixel_height) {
    auto height = std::tan(M_PI * fov_ / 360) * 2;
    auto width = height * (double(pixel_width) / pixel_height);

    return position_ + Vector(1, -width / 2, -height / 2)
           + Vector(0, width * x / pixel_width, height * y / pixel_height);
}
