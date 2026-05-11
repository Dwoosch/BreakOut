#pragma once
#include "surface.h"

class Scene {
public:
    int mouseX, mouseY;

    virtual void Init() = 0;
    virtual void Tick(float deltaTime) = 0;
    virtual void Shutdown() = 0;
    virtual void MouseDown(int button) {}
    virtual void MouseMove(int x, int y) {}
	virtual void MouseMoveAbsolute(int x, int y) {}
	virtual void SetTargetScene(Scene* scene) { targetScene = scene; }
	Scene* targetScene = nullptr; // set this to switch scenes
    Scene* nextScene = nullptr;  // set this to switch scenes
    Tmpl8::Surface* screen;
};