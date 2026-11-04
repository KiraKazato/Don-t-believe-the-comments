#pragma once
#include "SceneManager.h"

class Transitor
{
public:
	void FadeOut(float _duration, SceneManager::SCENE_TYPE _nextScene);
	void FadeIn(float _duration);

	bool IsTransitionFinish() { return mfTransitorTime >= mfTransitorMaxTime; }

	void FadaInit() { mfTransitorTime = 0.0f; }

private:
	float mfTransitorTime = 0.0f;
	float mfTransitorMaxTime = 1.0f;
};