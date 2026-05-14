#pragma once
#include <memory>

#include "Scene.h"

namespace Tmpl8 {

    class Surface;
    class Game
    {
    public:
        void SetTarget(Surface* surface) { screen = surface; }
        void Init();
        void Shutdown();
        void Tick(float deltaTime);
        void MouseUp(int button) {}
        void MouseDown(int button);
        void MouseMove(int x, int y) {}
        void MouseMoveAbsolute(int x, int y);
        void KeyUp(int key) {}
        void KeyDown(int key) {}

    private:
        Surface* screen = nullptr;
        std::unique_ptr<Scene> currentScene;
    };

}; // namespace Tmpl8