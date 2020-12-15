#include "Scene.h"
#include "math.h"
#include "Camera.h"
#include "Volume.h"
#include <cmath>

size_t frame_counter = 0;

void putPixel(float* buf, int w, int h, int x, int y, float r, float g, float b) {
    const size_t pos = (y * w + x) * 4;
    buf[pos] = r;
    buf[pos + 1] = g;
    buf[pos + 2] = b;
    buf[pos + 3] = 1.;
}

void drawBuffer(float* buf, int w, int h) {
    glClear(GL_COLOR_BUFFER_BIT);
    glWindowPos2i(0, 0);
    glDrawPixels(w, h, GL_RGBA, GL_FLOAT, buf);
}

double f(Vector x) {
    static Volume vol;
    static bool loaded = false;
    if (!loaded) {
        vol.loadFromFile("../volumes/csafe_heptane_302x302x302_uint8.raw", 302);
        loaded = true;
    }

    return vol.sampleDensity(x - Vector(0.5, -0.5, -0.5));

    return exp(-(pow(x.x - 5, 2) + pow(x.y, 2) + pow(x.z, 2))) * 10;
    return (x - Vector(5, 0, 0)).length() < 1. ? 1 : 0;
}

Vector color(Vector x) {
    static const Vector c1 = {1., 0.39, 0.15};
    static const Vector c2 = {1., 1., 1.};

    double low = -0.35, high = -0.3;
    double ratio = (x.z - low) / (high - low);
    ratio = std::clamp(ratio, 0., 1.);

    return ratio * c2 + (1 - ratio) * c1;

    auto temp = (x - Vector(1,0,0));
    return {1, equal(temp.x, temp.y, 0.3) && equal(temp.y, temp.z, 0.3) ? 0. : 1., 1};
}

double marchTransparency(Vector begin, Vector end) {
    double acc_transparency = 1.;
    const size_t step_count = 10;

    const Vector step_delta = (end - begin) / step_count;
    const double step_length = step_delta.length();
    for (size_t i = 0; i < step_count; ++i) {
        const Vector pos = begin + step_delta * i;
        const auto opacity = f(pos) * step_length;
        acc_transparency *= std::max(0., 1 - opacity);
        if (equal(acc_transparency, 0)) {
            break;
        }
    }
    return acc_transparency;
}

Vector marchRay(Vector begin, Vector end) {
    Vector val;
    double acc_transparency = 1.;

    const size_t step_count = 1000;

    const Vector step_delta = (end - begin) / step_count;
    const double step_length = step_delta.length();

    Vector sunDirection {-1., -1., 0.5};
    sunDirection /= sunDirection.length();

    for (size_t i = 0; i < step_count; ++i) {
        const Vector pos = begin + step_delta * i;
        const auto density = f(pos);

        if (equal(density, 0., 1e-6)) continue;

        const auto opacity = density * step_length;
        const auto color_ = Vector(0.2, 0.2, 0.2) + color(pos) * marchTransparency(pos, pos + sunDirection);
        val += color_ * opacity * acc_transparency;
        acc_transparency *= std::max(0., 1 - opacity);
        if (equal(acc_transparency, 0)) {
            break;
        }
    }
    return val;
}

void Scene::draw(const glfwm::WindowID id) {
    auto start_time = glfwGetTime();

    int w, h;
    glfwm::Window::getWindow(id)->getSize(w, h);
    static std::vector<float> buffer(w * h * 4);
    static size_t i = 0;
    static size_t j = 0;

    if (buffer.size() != w * h * 4) {
        buffer.clear();
        buffer.resize(w * h * 4);
        i = 0;
        j = 0;
    }

    Camera cam;
    cam.fov_ = 60;

    for (; i < w || j < h; ++j) {
        if (j >= h) {
            j = 0;
            ++i;
        }
        Vector val = marchRay({0, 0, 0}, 3 * cam.GetCameraPixelPosition(i, j, w, h));
        putPixel(buffer.data(), w, h, i, j, val.x, val.y, val.z);
        if (glfwGetTime() - start_time > 1.) {
            break;
        }
    }

    drawBuffer(buffer.data(), w, h);

    ++frame_counter;
}
