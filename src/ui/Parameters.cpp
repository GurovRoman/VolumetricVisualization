#include "Parameters.h"

Parameters& Parameters::get() {
    static Parameters instance;
    return instance;
}
