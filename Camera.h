#pragma once
#include "math.h"


class Camera {
public:
    Vector position_ {};
    //Angle rotation_ {};
    double fov_ {90.};
    double aspect_ratio_ {16. / 9};

public:
    Vector GetCameraPixelPosition(int x, int y, int width, int height);
};

