#pragma once
#include"DxLib.h"
#include"Scene.h"
#include "Master.h"
#include"EffekseerEffect.h"

class LevelSelectScene :public Scene
{
	enum LevelType
	{
		LEVEL_DAY1,
		LEVEL_COMINGSOON,
	};

public:
	LevelSelectScene();

	~LevelSelectScene();

	virtual void Initialize()override;

	virtual void Update()override;

	virtual void Draw()override;

	virtual void Finalize()override;

private:
	LevelType currentLevel;

	EffekseerEffect* startEffect;

	int selectBackGroundGraph;//難易度設定画面の背景グラフ

	int day1ButtonGraph;//Day1用のボタンUIグラフ

	//左右の矢印グラフ
	int leftArrowGraph;
	int rightArrowGraph;

	//Coming soonグラフ
	int comingSoonGraph;

	//マウスの座標を受け取る
	int mouseX{};
	int mouseY{};

	float stageButtonPositionX1 = Master::gridWidth * 48;
	float stageButtonPositionX2 = Master::gridWidth * 73;
	float stageButtonPositionY1 = Master::gridHeight * 56;
	float stageButtonPositionY2 = Master::gridHeight * 95;

	bool isWaitingSceneChange = false; // シーン遷移の待機中かどうか
	int waitFrameCounter = 0;          // 経過したフレーム数を数える変数
	const int WAIT_TIME = 197;          // 待ちたいフレーム数

	VECTOR centerPosition = VGet(Master::Width / 2.0f, Master::Height / 2.0f, 0.0f);//Effectを再生する場所（StartEffect）
};
