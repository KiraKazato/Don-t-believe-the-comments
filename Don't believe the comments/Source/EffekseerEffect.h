#pragma once
#include <EffekseerForDXLib.h>
#include "DxLib.h"

class EffekseerEffect
{
public:
	//コンストラクタ
	EffekseerEffect(const char* filename, VECTOR initPos, float EffectSize);
	//デストラクタ
	~EffekseerEffect();

	void Load();//エフェクトの読み込み
	void Update();//アップデート
	void Draw();//描画
	void Play();//エフェクトの再生
	void Stop();//エフェクトの停止


	void SetPosition(VECTOR pos) { PlayPos = pos; }//座標の設定
	VECTOR GetPosition() { return PlayPos; }//座標の取得

	void SetRotation(VECTOR rot) { mvRotation = rot; }//回転の設定
	VECTOR GetRotation() { return mvRotation; }//回転の取得

	// ループをするかしないかの処理
	void SetLoop(bool mfLoop) { LoopFlag = mfLoop; }

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
};