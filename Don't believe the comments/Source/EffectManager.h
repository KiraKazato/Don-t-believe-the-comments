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

private:
	const int effectParticleLimit = 20000;// 画面に表示できる最大パーティクル
	
	int effectResourceHandle;// エフェクトのリソース用
	int playingEffectHandle;// 再生中のエフェクトハンドル
};