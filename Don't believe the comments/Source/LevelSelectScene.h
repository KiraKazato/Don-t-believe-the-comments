#pragma once
#include"DxLib.h"
#include"Scene.h"
#include "Master.h"

class LevelSelectScene :public Scene
{

	enum ButtonType
	{
		BUTTON_LEVEL,
		BUTTON_ARROW_LEFT,
		BUTTON_ARROW_RIGHT,
		MAX_BUTTON,
	};

public:
	LevelSelectScene();

	~LevelSelectScene();

	virtual void Initialize()override;

	virtual void Update()override;

	virtual void Draw()override;

	virtual void Finalize()override;

	void ChangeStage();

private:
	StageNumber currentStage;

	//左右の矢印グラフ
	int mnLeftArrowGraph;
	int mnRightArrowGraph;


	int mnSelectBackGroundGraphHandle;//難易度設定画面の背景グラフ
	int mnDay1ButtonGraphHandle;//Day1用のボタンUIグラフ
	int mnComingSoonGraphHandle;//Coming soonグラフ
	int mnBackToTitleScene;//タイトルに戻るボタンUI

	//マウスの座標を受け取る
	int mouseX{};
	int mouseY{};

	//ボタンのハンドル
	std::vector<int>mStageButtonHandle;

	//レベルボタンの基準
	float stageButtonPositionX1 = Master::gridWidth * 48;
	float stageButtonPositionX2 = Master::gridWidth * 73;
	float stageButtonPositionY1 = Master::gridHeight * 42;
	float stageButtonPositionY2 = Master::gridHeight * 82;
};
