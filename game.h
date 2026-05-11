#pragma once
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
        Scene* currentScene;

    private:
        Surface* screen;
		Scene* nextScene;
    };

}; // namespace Tmpl8