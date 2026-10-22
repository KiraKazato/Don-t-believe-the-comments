#pragma once
#include "Scene.h"
#include <vector>
#include "SoundManager.h"
#include "GameCycle.h"

class GameScene :public Scene
{
public:
	GameScene();
	~GameScene();
	virtual void Initialize()override;
	virtual void Update()override;
	virtual void Draw()override;
	virtual void Finalize()override;

private:
	int questionScreenHandle = -1;
	int inputFieldHandle = -1;
	int backGroundHandle = -1;
	int commentFieldHandle = -1;
private:
	GameCycle mGameCycle;
};