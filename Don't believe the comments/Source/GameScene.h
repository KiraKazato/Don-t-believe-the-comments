#pragma once
#include "Scene.h"
#include <vector>
#include "SoundManager.h"

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

};