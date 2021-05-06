#include "EnableGuard.h"

EnableGuard::EnableGuard(GLenum option, bool enable) {
    option_ = option;

    GLboolean temp;
    glGetBooleanv(option, &temp);
    old_state_ = temp;

    setState(enable);
}

EnableGuard::~EnableGuard() {
    setState(old_state_);
}

void EnableGuard::setState(bool enable) const {
    enable ? glEnable(option_) : glDisable(option_);
}
