#pragma once
#include "SceneManager.h"
#include <functional>
#include <vector>
#include <string>

// デフォルトのトランジションの時間
constexpr float DEFALT_DURATION = 0.7f;

// std::fuction<戻り値(引数)> -> 関数そのものを保存し、実行することが可能

class Transitor
{
public:
	// フェード初期化
	// フェード時間などを諸々決められる
	void XFadeInit(bool _isFadeIn, float _duration = DEFALT_DURATION, SceneManager::SCENE_TYPE _nextScene = SceneManager::SCENE_TYPE(-1));
	// 通常画面遷移
	void XFadeInit(SceneManager::SCENE_TYPE _nextScene);

	// フェードの更新
	void XFade();

	// トランジション中にやりたい処理の追加 pair<関数名, 処理内容>
	// ※これはvoid型で引数がないもののみ（void型ラムダ式である）
	// ※[&](){～}という形で
	void AddTransitorProcessingUpdate(std::pair<std::string, std::function<void()>> _function);
	// 関数名, 処理内容指定
	void AddTransitorProcessingUpdate(std::string _functionName, const std::function<void()> _func);
	// 処理内容のみ指定
	// ※多重チェックをしていないため要注意
	void AddTransitorProcessingUpdate(const std::function<void()> _func);

	// トランジション後にやりたい処理の追加 pair<関数名, 処理内容>
	// ※これはvoid型で引数がないもののみ（void型ラムダ式である）
	// ※[&](){～}という形で
	void AddTransitorAfterUpdate(std::pair<std::string, std::function<void()>> _func);
	// 関数名, 処理内容指定
	void AddTransitorAfterUpdate(std::string _functionName, std::function<void()> _func);
	// 処理内容のみ指定
	// ※多重チェックをしていないため要注意
	void AddTransitorAfterUpdate(std::function<void()> _func);

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

	std::vector<std::pair<std::string, std::function<void()>>> mUpdateFunction{};	// トランジション中に処理したいこと
	std::vector<std::pair<std::string, std::function<void()>>> mAfterFunction{};	// トランジション後に処理したいこと

	int mnProcessingFunctionCount = 0;
	int mnAfterFunctionCount = 0;
};