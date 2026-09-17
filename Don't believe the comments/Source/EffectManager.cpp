#include "EffectManager.h"
#include "Master.h"

// コンストラクタ
EffectManager::EffectManager():effectResourceHandle(-1), playingEffectHandle(-1)
{
}

// デストラクタ
EffectManager::~EffectManager()
{
}

// 初期化
void EffectManager::Initalize()
{

	// 引数には画面に表示する最大パーティクル数を設定する
	if (Effekseer_Init(effectParticleLimit) == -1)
	{
		DxLib_End();
	}

	//2Dの設定
	Effekseer_Set2DSetting(Master::Width, Master::Height);

	//フルスクリーンウィンドウの切り替えで表示する最大パーティクル数を設定する
	// Effekseerを使用する場合は必ず設定する・
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	// DXライブラリのデバイスロストした時のコールバックを設定する。
	// ウィンドウとフルスクリーンの切り替えが発生する場合は必ず実行する
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();
}

void EffectManager::Update()
{
	//2D用のアップデート
	UpdateEffekseer2D();
}

void EffectManager::Draw()
{
	//2D用の描画
	DrawEffekseer2D();
}