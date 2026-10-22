#pragma once
#include"DxLib.h"
#include"Scene.h"
#include "Master.h"

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
};
