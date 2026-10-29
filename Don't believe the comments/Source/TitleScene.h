#pragma once
#include "Scene.h"
#include "SoundManager.h"
#include"EffekseerEffect.h"
#include"Master.h"
#include"DxLib.h"

class TitleScene :public Scene
{
public:
	TitleScene();
	~TitleScene();
	virtual void Initialize()override;
	virtual void Update()override;
	virtual void Draw()override;
	virtual void Finalize()override;

private:

	EffekseerEffect* mnSettingTransition;
	EffekseerEffect* mnLevelSelectTransition;


	//画像ハンドル
	int mnTitleBackGroundHandle;
	int mnStartBottonHandle;
	int mnSettingBottonHandle;

	//マウスの座標を受け取る
	int mnMouseX{};
	int mnMouseY{};

	bool isWaitingSceneChange = false; // シーン遷移の待機中かどうか
	int mnWaitFrameCounter = 0;          // 経過したフレーム数を数える変数
	const int WAIT_TIME = 9;          // 待ちたいフレーム数

	bool isWaitingLevelSceneChange = false; // シーン遷移の待機中かどうか
	int mbWaitFrameCounter = 0;          // 経過したフレーム数を数える変数
	const int LEVEL_TRANSITION_WAIT_TIME = 5;          // 待ちたいフレーム数

	VECTOR centerPosition = VGet(Master::Width / 2.0f, Master::Height / 2.0f, 0.0f);//Effectを再生する場所（StartEffect）

};

