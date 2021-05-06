#pragma once
#include <unordered_set>


template <typename RefType>
class GlobalInstanceKeeper {
public:
    explicit GlobalInstanceKeeper(RefType ref) : ref_(ref) {
        active_instances.insert(ref_);
    }
    ~GlobalInstanceKeeper() noexcept {
        active_instances.erase(ref_);
    }
    static const auto& getInstances() {
        return active_instances;
    }
private:
    static inline std::unordered_set<RefType> active_instances;
    RefType ref_;
};
