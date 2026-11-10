#pragma once
#include <EffekseerForDXLib.h>
#include "DxLib.h"

class EffekseerEffect
{
public:
	//コンストラクタ
	EffekseerEffect(const char* filename, VECTOR initPos, float EffectSize);

	//初期位置を省略して作れるコンストラクタ
	EffekseerEffect(const char* filename, float EffectSize)
		: EffekseerEffect(filename, VGet(0.0f, 0.0f, 0.0f), EffectSize) {
	}

	//デストラクタ
	~EffekseerEffect();

	void Load();//エフェクトの読み込み
	void Update();//アップデート
	void Draw();//描画
	void Play();//エフェクトの再生
	void Stop();//エフェクトの停止


	void SetPosition(VECTOR pos) { PlayPos = pos; }//座標の設定

	//VECTORを使わず (x, y) の数字だけで場所を変えられる関数
	void SetPosition(float x, float y) { PlayPos = VGet(x, y, 0.0f); }

	//再生すると同時に好きな場所 (x, y) を指定できる関数
	void Play(float x, float y)
	{
		SetPosition(x, y);
		Play();
	}

	VECTOR GetPosition() { return PlayPos; }//座標の取得

	void SetRotation(VECTOR rot) { mvRotation = rot; }//回転の設定
	VECTOR GetRotation() { return mvRotation; }//回転の取得

	// ループをするかしないかの処理
	void SetLoop(bool mfLoop) { LoopFlag = mfLoop; }

	//再生速度を設定する関数
	void SetSpeed(float speed) { effectSpeed = speed; }

private:
	VECTOR PlayPos;		// 再生したい座標
	VECTOR mvRotation;  // 回転させたい角度

	const char* filePath;//ファイルのパス名

	int effectResourcehandle;//エフェクトのリソースハンドル
	int playingEffectHandle;// 再生中のエフェクトハンドル

	float effectSize;//エフェクトの大きさ

	const int EffectPlayInterval = 300;//インターバル

	int PlayCount;//エフェクトのカウント

	bool LoopFlag;//ループするかのフラグ

	float effectSpeed = 1.0f;
};