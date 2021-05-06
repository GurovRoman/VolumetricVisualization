#include "Volume.h"
#include <fstream>
#include <cmath>
#include <exception>
#include <filesystem>


void Volume::loadFromFile(const std::string& path) {
    std::string data_path;
    {
        std::ifstream config("volumes/" + path);
        config >> data_path;
        config >> size_.x >> size_.y >> size_.z;
    }

    volume_data_.clear();
    volume_data_.resize(size_.x * size_.y * size_.z);
    std::ifstream file("volumes/" + data_path, std::ios_base::in | std::ios_base::binary);

    if (file.fail()) {
        throw std::runtime_error("Volume load from file failed!");
    }


    file.seekg(0, std::ios::end);
    std::vector<char> data(file.tellg());
    file.seekg(0, std::ios::beg);
    file.read(data.data(), data.size());
    for (size_t i = 0; i < volume_data_.size(); ++i) {
        volume_data_[i] = float(static_cast<unsigned char>(data[i])) / 255;
    }

    density_tex3d_ = std::make_shared<Texture>(GL_TEXTURE_3D, GL_R16F, GL_RED, GL_FLOAT, size_.x, size_.y, size_.z, volume_data_.data());

    tf.scale = size_;
    tf.scale = tf.scale / glm::max(tf.scale.x, glm::max(tf.scale.y, tf.scale.z));
}


GLfloat Volume::operator()(unsigned int x, unsigned int y, unsigned int z) {
    return volume_data_[x * size_.y * size_.z + y * size_.z + z];
}

const std::vector<std::string>& Volume::getVolumeFileList() {
    static std::vector<std::string> list;
    static bool ready = false;
    if (!ready) {
        list.clear();
        for (const auto& entry : std::filesystem::directory_iterator("volumes")) {
            const auto& path = entry.path().filename().string();
            if (!entry.is_directory() && path.ends_with(".cfg")) {
                list.push_back(path);
            }
        }
    }
    return list;
}
