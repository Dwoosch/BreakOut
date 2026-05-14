#pragma once
#include <memory>
#include "Audio.h"
#include "surface.h"

class Scene {
public:
    virtual ~Scene() = default;

    int mouseX, mouseY;

    virtual void Init() = 0;
    virtual std::unique_ptr<Scene> Tick(float deltaTime) = 0;
    virtual void Draw(Tmpl8::Surface* screen) = 0;
    virtual void Shutdown() = 0;
    virtual void MouseDown(int button) {}
    virtual void MouseMove(int x, int y) {}
	virtual void MouseMoveAbsolute(int x, int y) {}
};