#pragma once
#include "SceneManager.h"
#include <functional>
#include <vector>

// デフォルトの時間
constexpr float DEFALT_DURATION = 0.7f;

class Transitor
{
public:
	// フェード初期化
	// フェード時間などを諸々決められる
	void XFadeInit(bool _isIn, float _duration = DEFALT_DURATION, SceneManager::SCENE_TYPE _nextScene = SceneManager::SCENE_TYPE(-1));
	// 通常画面遷移
	void XFadeInit(SceneManager::SCENE_TYPE _nextScene);

	// フェードの更新
	void XFade();

	// トランジション中にやりたい処理の追加
	// std::fuction<戻り値(引数)>について：関数そのものを保存・実行することが可能
	// ※これはvoid型で引数がないもののみ（void型ラムダ式である）
	// ※[&](){～}という形で
	void AddTransitorUpdate(std::function<void()> _func);

	// トランジション後にやりたい処理の追加
	// ※これはvoid型で引数がないもののみ（void型ラムダ式である）
	// ※[&](){～}という形で
	void AddAfterUpdate(std::function<void()> _func);

	// トランジション中にやりたい処理を実行
	void TransitorUpdate();

	// トランジション後にやりたい処理を実行
	void TransitorAfter();

	// フェードが終了したか
	bool IsTransitionFinish() { return mfTransitorTime >= mfTransitorMaxTime; }

	// フェードを進める
	void FadeStep(float _step) { mfTransitorTime += _step; }

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

	std::vector<std::function<void()>> mUpdateFunction{};	// トランジション中に処理したいこと
	std::vector<std::function<void()>> mAfterFunction{};	// トランジション後に処理したいこと
};