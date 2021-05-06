#pragma once
#include <unordered_map>
#include <string>
#include <any>


class Parameters {
public:
    template <typename T>
    T& at(const std::string& key) {
        if (!params_.contains(key)) {
            params_[key] = T();
        }
        return std::any_cast<T&>(params_[key]);
    }

    static Parameters& get();

private:
    Parameters() = default;

private:
    std::unordered_map<std::string, std::any> params_;
};


