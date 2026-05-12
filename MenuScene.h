#pragma once
#include "Scene.h"
#include "GameState.h"

class MenuScene : public Scene
{
	void Init() override;
	void Tick(float deltaTime) override;
	void Shutdown() override;
	void MouseDown(int button) override;
	void MouseMove(int x, int y) override {/* implement if you want to detect mouse movement */ }
	void MouseMoveAbsolute(int x, int y) override;
};

