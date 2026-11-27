#pragma once
#include "Scene.h"
#include "SoundManager.h"

class SettingScene : public Scene
{
public:
	SettingScene();
	~SettingScene();
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual void Finalize() override;

private:
	// スライダー1本分のデータをまとめる構造体
	struct VolumeSlider
	{
		float barTopY = 0.0f;      // バーの上端Y座標
		float barBottomY = 0.0f;   // バーの下端Y座標
		float dialX = 0.0f;        // つまみのX座標
		float dialY = 0.0f;        // つまみのY座標
		float volumeGaugeY = 0.0f; // ゲージ中身のY座標
		bool grabTheDial = false;  // つまみを掴んでいるか
		int nowVolume = 0;         // 現在の音量（0〜100）
		float volumeRatio = 0.0f;  // 音量の割合（0.0〜1.0）
	};

	// スライダー用の共通処理（BGMとSEで使い回す）
	void InitSlider(VolumeSlider& slider, float gridY, int initialVolume);
	void UpdateSlider(VolumeSlider& slider, bool canGrab);
	void DrawSlider(const VolumeSlider& slider, const char* label);

private:
	int settingGraph;        // 設定画面の背景ハンドル
	int volumeBar;           // 音量調節バーの外枠ハンドル
	int volumeBarBackGround; // バーの中身（ゲージ）ハンドル
	int volumeDial;          // 音量調節つまみハンドル
	int mnBackButtonHandle;  // 戻るボタンハンドル

	// BGMとSEで共通して使うサイズ・横位置の情報
	int dialWidth;
	int dialHeight;
	int volumeGaugeWidth;
	int volumeGaugeHeight;
	float barLeftX;
	float barRightX;
	int overallLength;
	float volumeGaugeX{};

	// マウス座標
	int MouseX{};
	int MouseY{};

	// BGM用・SE用のスライダー本体
	VolumeSlider bgmSlider;
	VolumeSlider seSlider;
};