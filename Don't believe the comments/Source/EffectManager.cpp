#include "EffectManager.h"
#include "Master.h"
#include <vector>

// コンストラクタ
EffectManager::EffectManager()
	: effectResourceHandle(-1)
	, playingEffectHandle(-1)
	, holdEffectResourceHandle(-1)
	, playingHoldEffectHandle(-1)
{
}

// デストラクタ
EffectManager::~EffectManager()
{
	if (effectResourceHandle != -1)
	{
		DeleteEffekseerEffect(effectResourceHandle);
	}
	// 長押し用エフェクトの解放
	if (holdEffectResourceHandle != -1)
	{
		DeleteEffekseerEffect(holdEffectResourceHandle);
	}
}

// 初期化
void EffectManager::Initalize()
{
	if (Effekseer_Init(effectParticleLimit) == -1)
	{
		DxLib_End();
	}

	Effekseer_Set2DSetting(Master::Width, Master::Height);
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

	// 単発クリック用のエフェクト読み込み
	effectResourceHandle = LoadEffekseerEffect("Resource/Effect/ClickEffect.efk", 20.0f);

	// 長押し用の違うエフェクトを読み込む
	holdEffectResourceHandle = LoadEffekseerEffect("Resource/Effect/LongPressEffect.efk", 60.0f);
}

void EffectManager::Update()
{
	// 左クリックした瞬間だけ単発クリックエフェクトを出す
	if (Master::mpMouseManager->PressLeftClick())
	{
		int mouseX = 0;
		int mouseY = 0;
		Master::mpMouseManager->GetMousePosition(&mouseX, &mouseY);

		playingEffectHandle = PlayEffekseer2DEffect(effectResourceHandle);
		if (playingEffectHandle != -1)
		{
			SetPosPlayingEffekseer2DEffect(playingEffectHandle, static_cast<float>(mouseX), static_cast<float>(mouseY), 0.0f);
			// リストに登録する（これで連打しても全部最後まで再生される）
			RegisterNormalEffect(playingEffectHandle);
		}
	}

	// 2D用のアップデート
	UpdateEffekseer2D();

	// 再生が終わったエフェクトをリストからお掃除する
	for (auto it = mNormalEffectHandles.begin(); it != mNormalEffectHandles.end(); )
	{
		// 再生終了（-1）になっていたらリストから消す
		if (IsEffekseer2DEffectPlaying(*it) == -1)
		{
			it = mNormalEffectHandles.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void EffectManager::Draw()
{
	// リストにある通常エフェクトと長押しエフェクトだけを描画する
	DrawEffekseer2D_Begin();

	// 登録されている通常エフェクトをすべて描画
	for (int handle : mNormalEffectHandles)
	{
		if (IsEffekseer2DEffectPlaying(handle) == 0)
		{
			DrawEffekseer2D_Draw(handle);
		}
	}

	// 長押しエフェクトが再生中なら描画
	if (playingHoldEffectHandle != -1 && IsEffekseer2DEffectPlaying(playingHoldEffectHandle) == 0)
	{
		DrawEffekseer2D_Draw(playingHoldEffectHandle);
	}

	DrawEffekseer2D_End();
}

// 長押しエフェクト：終わったらすぐ最初から再生＋位置を追従させる
void EffectManager::KeepPlayingEffect(float x, float y)
{
	// まだ再生されていない、または最後まで再生し終わった瞬間なら
	if (playingHoldEffectHandle == -1 || IsEffekseer2DEffectPlaying(playingHoldEffectHandle) == -1)
	{
		// すぐにまた最初から再生を開始する
		playingHoldEffectHandle = PlayEffekseer2DEffect(holdEffectResourceHandle);
	}

	// 再生中のエフェクトを現在の座標（x, y）に追従させる
	if (playingHoldEffectHandle != -1)
	{
		SetPosPlayingEffekseer2DEffect(playingHoldEffectHandle, x, y, 0.0f);
	}
}

// 長押しをやめたときにエフェクトをピタッと止める関数
void EffectManager::StopHoldEffect()
{
	if (playingHoldEffectHandle != -1)
	{
		StopEffekseer2DEffect(playingHoldEffectHandle);
		playingHoldEffectHandle = -1;
	}
}

// 描画リストにハンドルを登録する関数
void EffectManager::RegisterNormalEffect(int handle)
{
	if (handle != -1)
	{
		mNormalEffectHandles.push_back(handle);
	}
}