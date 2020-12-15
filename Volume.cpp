#include "Volume.h"
#include <fstream>
#include <cmath>
#include <exception>


void Volume::loadFromFile(const std::string& path, size_t side) {
    volume_data_.clear();
    volume_data_.resize(side * side * side);
    side_ = side;
    std::ifstream file(path, std::ios_base::in | std::ios_base::binary);

    if (file.fail()) {
        throw std::runtime_error("Volume load from file failed!");
    }

    for (auto& el : volume_data_) {
        el = file.get();
    }
}

double Volume::sampleDensity(Vector pos) {
    if (pos.x < 0 || pos.x > 1 || pos.y < 0 || pos.y > 1 || pos.z < 0 || pos.z > 1) {
        return 0.;
    }
    pos.x = round((pos.x * (side_ - 1)));
    pos.y = round((pos.y * (side_ - 1)));
    pos.z = round((pos.z * (side_ - 1)));
    return operator()(pos.x, pos.y, pos.z) / 64.;
}

uint8_t Volume::operator()(unsigned int x, unsigned int y, unsigned int z) {
    return volume_data_[x * side_ * side_ + y * side_ + z];
}
