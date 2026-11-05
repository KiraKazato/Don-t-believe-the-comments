#pragma once
#include "SceneManager.h"

// デフォルトの時間
constexpr float DEFALT_DURATION = 0.7f;

class Transitor
{
public:
	// フェード初期化
	void XFadeInit(bool _isIn, float _duration = DEFALT_DURATION, SceneManager::SCENE_TYPE _nextScene = SceneManager::SCENE_TYPE(-1));
	void XFadeInit(SceneManager::SCENE_TYPE _nextScene);

	// フェードの更新
	void XFade();

	// フェードが終了したか
	bool IsTransitionFinish() { return mfTransitorTime >= mfTransitorMaxTime; }

	// フェードを進める
	void FadeStep(float _step) { mfTransitorTime += _step; }

	void SetT(float _time) { mf_t = _time; }

	// フェードを書く
	void FadeDraw();

	// 次シーンの取得
	SceneManager::SCENE_TYPE GetNextScene() { return mNextScene; }

private:
	float mfTransitorTime = 1.0f;
	float mfTransitorMaxTime = 0.0f;
	float mf_t = 0.0f;

private:
	bool mbIsFadeIn = false;
	bool mbIsBoot = true;

	SceneManager::SCENE_TYPE mNextScene{};
};