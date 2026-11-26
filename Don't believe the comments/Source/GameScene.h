#pragma once
#include "Scene.h"
#include <vector>
#include "SoundManager.h"
#include "GameCycle.h"
#include "EffekseerEffect.h" 


class GameScene :public Scene
{
public:
	GameScene();
	~GameScene();
	virtual void Initialize()override;
	virtual void Update()override;
	virtual void Draw()override;
	virtual void Finalize()override;

	void DrawDecoration();

private:
	int questionScreenHandle = -1;
	int inputFieldHandle = -1;
	int backGroundHandle = -1;
	int commentFieldHandle = -1;

	int commentCountFontHandle = -1;
private:
	GameCycle mGameCycle;
	int mnBackGround;

	//：背景用バブルエフェクトと、枠内切り抜き用の仮キャンバス
	EffekseerEffect* mpBubbleEffect = nullptr;
	int mnBubbleScreen = -1;
};