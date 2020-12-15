#pragma once
#include <vector>
#include <cstdint>
#include <string>
#include "math.h"


class Volume {
private:
    std::vector<uint8_t> volume_data_;
    size_t side_ {0};

public:
    uint8_t operator ()(unsigned x, unsigned y, unsigned z);

    void loadFromFile(const std::string& path, size_t side);
    double sampleDensity(Vector pos);
};

