#pragma once
#include <GLcommon.h>


class EnableGuard {
public:
    explicit EnableGuard(GLenum option, bool enable = true);

    EnableGuard(const EnableGuard&) = delete;
    EnableGuard(EnableGuard&&) = default;
    EnableGuard& operator= (const EnableGuard&) = delete;
    EnableGuard& operator= (EnableGuard&&) = default;

    ~EnableGuard();

private:
    void setState(bool enable) const;

private:
    bool old_state_;
    GLenum option_;
};


