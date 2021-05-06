#pragma once


#include <vector>
#include "glm/vec3.hpp"
#include "Transform.h"
#include "GLcommon.h"

class Actor {
public:
    virtual ~Actor() {};

public:
    Transform tf {};
};


