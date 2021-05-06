#pragma once

#include <scene/Actor.h>

class Scene;

class GUI {
public:
    GUI();
    ~GUI();

    void setScene(const Scene* scene) {
        scene_ = scene;
    }

    void updateGUI();
    void drawGUI();

private:
    void drawFrametime();
    void drawMain();
    void drawActorListWidget();
    void drawProperties(const std::string& name, Actor* actor, bool& open);

    const Scene* scene_ {nullptr};
};


