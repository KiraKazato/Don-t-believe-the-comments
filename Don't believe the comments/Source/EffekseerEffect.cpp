#include "EffekseerEffect.h"
#include <EffekseerForDXLib.h>
#include"Master.h"

EffekseerEffect::EffekseerEffect(const char* filename, VECTOR initPos, float EffectSize): PlayPos(initPos), mvRotation(VGet(0.0f, 0.0f, 0.0f)), effectResourcehandle(-1), filePath(filename), effectSize(EffectSize), PlayCount(0), playingEffectHandle(-1), LoopFlag(true)
{
	Load();
}

EffekseerEffect::~EffekseerEffect()
{
	// エフェクトリソースの解放
	DeleteEffekseerEffect(effectResourcehandle);
}

void EffekseerEffect::Load()
{
	// エフェクトのリソースを読み込む
	effectResourcehandle = LoadEffekseerEffect(filePath, effectSize);
}

void EffekseerEffect::Update()
{
	//再生されているなら
	if (playingEffectHandle != -1)
	{
		//奥行き(Z座標)は 0.0f 
		SetPosPlayingEffekseer2DEffect(playingEffectHandle, PlayPos.x, PlayPos.y, 0.0f);

		//画面の奥を軸(Z軸)にして回転させるため Z だけ設定
		SetRotationPlayingEffekseer2DEffect(playingEffectHandle, 0.0f, 0.0f, mvRotation.z);

		//エフェクトが終わっていたらハンドルを -1 に戻す
		if (IsEffekseer2DEffectPlaying(playingEffectHandle) == -1)
		{
			playingEffectHandle = -1;
		}
	}
}

void EffekseerEffect::Draw()
{
	//2Dエフェクトの描画はDrawEffekseer2D()をほかで呼ぶため、
	//ここは空のまま
}

// 基本的にエフェクトの使用はこの関数を呼ぶ。
void EffekseerEffect::Play()
{
	//再生関数
	playingEffectHandle = PlayEffekseer2DEffect(effectResourcehandle);

	//座標の設定
	if (playingEffectHandle != -1)
	{
		SetPosPlayingEffekseer2DEffect(playingEffectHandle, PlayPos.x, PlayPos.y, 0.0f);
	}
}

// エフェクトを止めたいときに使う
void EffekseerEffect::Stop()
{
	//再生チェック関数
	int NowPlayEffect = IsEffekseer2DEffectPlaying(playingEffectHandle);

	// 再生中であれば止める
	if (NowPlayEffect != -1)
	{
		StopEffekseer2DEffect(playingEffectHandle);

		//止めたあとは -1に
		playingEffectHandle = -1; 
	}
}