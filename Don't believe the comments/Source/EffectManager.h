#pragma once

#include <EffekseerForDXLib.h>
#include "DxLib.h"

class EffectManager
{
public:
	//コンストラクタ
	EffectManager();
	//デストラクタ
	~EffectManager();

	void Initalize();//初期化
	void Update();//アップデート
	void Draw();//描画

	//長押し用エフェクトを追従させ、終わったらすぐ最初から再生する関数
	void KeepPlayingEffect(float x, float y);

	//長押しをやめたときにエフェクトを止める関数
	void StopHoldEffect();

private:
	const int effectParticleLimit = 20000;// 画面に表示できる最大パーティクル

	int effectResourceHandle;// 単発クリックエフェクトのリソース用
	int playingEffectHandle;// 単発クリックの再生中ハンドル

	//追加：長押し（つまみ操作など）エフェクト用の変数
	int holdEffectResourceHandle = -1; // 長押しエフェクトのリソース用
	int playingHoldEffectHandle = -1;  // 長押しエフェクトの再生中ハンドル
};