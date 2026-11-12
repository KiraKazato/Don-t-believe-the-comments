#pragma once
#include"DxLib.h"
#include"Scene.h"
#include "Master.h"
#include"EffekseerEffect.h"

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
	EffekseerEffect* gameStartEffect;

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

	bool isWaitingSceneChange = false; // シーン遷移の待機中かどうか
	int waitFrameCounter = 0;          // 経過したフレーム数を数える変数
	const int WAIT_TIME = 159;          // 待ちたいフレーム数

	bool isShaking = false;               // 画像が震えている最中かどうか
	int shakeFrameCounter = 0;            // 震えているフレーム数を数える変数
	const int SHAKE_TIME = 15;            // 震わせる時間
	const float MAX_SHAKE_POWER = 6.0f;  // 最初の最大の揺れ幅

};