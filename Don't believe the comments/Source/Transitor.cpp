#include "Transitor.h"
#include "Master.h"
#include "DxLib.h"

void Transitor::FadeOut(float _duration, SceneManager::SCENE_TYPE _nextScene)
{
	if (IsTransitionFinish())
	{
		return;
	}

	mfTransitorMaxTime = _duration;
	constexpr float frameTime = 1.0f / 60.0f;

	float t = mfTransitorTime / mfTransitorMaxTime;

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255.0f * t));
	DrawBox(0, 0, Master::Width, Master::Height, GetColor(0, 0, 0), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	if (IsTransitionFinish())
	{
		Master::mpSceneManager->SetNextScene(_nextScene);
		mfTransitorTime = 0.0f;
	}
}

void Transitor::FadeIn(float _duration)
{
	if (IsTransitionFinish())
	{
		return;
	}

	mfTransitorMaxTime = _duration;
	constexpr float frameTime = 1.0f / 60.0f;

	float t = 1.0f - (mfTransitorTime / mfTransitorMaxTime);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255.0f * t));
	DrawBox(0, 0, Master::Width, Master::Height, GetColor(0, 0, 0), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
