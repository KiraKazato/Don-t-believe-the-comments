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
	int QuestionScreen = -1;
	int InputField = -1;
	int BackGround = -1;
private:
	GameCycle mGameCycle;
};