#include "Transitor.h"
#include "Master.h"
#include "DxLib.h"

void Transitor::XFadeInit(bool _isIn, float _duration, SceneManager::SCENE_TYPE _nextScene)
{
	// 起動したか
	if (mbIsBoot)
	{
		mbIsBoot = false;
		return;
	}

	mfTransitorMaxTime = _duration;

	mfTransitorTime = 0.0f;	// 時間計測の初期化
	mbIsFadeIn = _isIn;
	mf_t = 0.0f;			// 進行度

	// -1でないならシーンを変える
	if(_nextScene != -1)
	{
		mNextScene = _nextScene;
	}
	else
	{
		mNextScene = Master::mpSceneManager->GetNowScene();
	}
}

void Transitor::XFadeInit(SceneManager::SCENE_TYPE _nextScene)
{
	// シーン遷移 -> フェードアウト
	XFadeInit(false, DEFALT_DURATION, _nextScene);
}

void Transitor::XFade()
{
	mf_t = mfTransitorTime / mfTransitorMaxTime;
	mf_t = std::min(std::max(0.0f, mf_t), 1.0f);
	if (mbIsFadeIn)
	{
		mf_t = 1.0f - mf_t;
	}

	TransitorUpdate();

	Master::mpSceneManager->SetNextSceneFade(mNextScene, true);

	// 関数リストを消す
	if (IsTransitionFinish())
	{
		mUpdateFunction.clear();
	}
}


void Transitor::AddTransitorUpdate(std::function<void()> _func)
{
	// あっちこっちでの処理を行いたいのを考えるためリストにする
	mUpdateFunction.emplace_back(_func);
}

void Transitor::TransitorUpdate()
{
	for (auto& updateFunction : mUpdateFunction)
	{
		// 実行する
		updateFunction();
	}
}

void Transitor::FadeDraw()
{
	float alpha = 255.0f * mf_t;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(alpha));
	DrawBox(0, 0, Master::Width, Master::Height, GetColor(0, 0, 0), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}