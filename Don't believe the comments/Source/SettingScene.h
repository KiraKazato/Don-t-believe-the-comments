#pragma once
#include "Scene.h"
#include"SoundManager.h"

class SettingScene : public Scene
{
public:
	SettingScene();
	~SettingScene();
	virtual void Initialize()override;
	virtual void Update()override;
	virtual void Draw()override;
	virtual void Finalize()override;

private:

	int settingGraph;//設定画面の背景ハンドル

	int volumeBar;//音量調節バーハンドル

	int volumeBarBackGround;//バーの背景ハンドル

	int volumeDial;//音量調節つまみハンドル

	//音量バーのつまみ座標
	float dialX{};
	float dialY{};


	int dialWidth;//つまみ画像の幅
	int dialHeight;//つまみ画像の高さ

	int volumeGaugeWidth;//音量ゲージの画像の幅
	int volumeGaugeHeight;//音量ゲージのつまみ画像の高さ


	float barLeftX;//バーの左端座標
	float barRightX; //バーの右端座標

	int overallLength; //バー全体の長さ

	//音量ゲージの座標
	float volumeGaugeX{};
	float volumeGaugeY{};

	//つまみがつかまれて引っ張られているか？
	bool grabTheDial;

	//マウスの座標を受け取る
	int MouseX{};
	int MouseY{};

	//現在のボリューム
	int nowVolume;

	//ボリュームの割合
	float volumeRatio;
};
