#pragma once
#include "Scene.h"

class GameOverScene : public Scene
{
public:
    void Init() override;
    std::unique_ptr<Scene> Tick(float deltaTime) override;
    void Draw(Tmpl8::Surface* screen) override;
    void Shutdown() override;
    void MouseDown(int button) override;
    void MouseMove(int x, int y) override {/* implement if you want to detect mouse movement */ }
    void MouseMoveAbsolute(int x, int y) override;

private:
    bool mouseClicked = false;
	int finalScore = 0;
};

