#include "Transitor.h"
#include "Master.h"
#include "DxLib.h"

void Transitor::XFadeInit(bool _isFadeIn, float _duration, SceneManager::SCENE_TYPE _nextScene)
{
	// 起動したか
	if (mbIsBoot)
	{
		mbIsBoot = false;
		return;
	}

	mfTransitorMaxTime = _duration;

	mfTransitorTime = 0.0f;	// 時間計測の初期化
	mbIsFadeIn = _isFadeIn;
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
		mAfterFunction.clear();
	}
}


void Transitor::AddTransitorProcessingUpdate(std::pair<std::string, std::function<void()>> _function)
{
	// 同じものを多重で入れないようにする
	for (size_t i = 0; i < mUpdateFunction.size(); i++)
	{
		const auto& func = mUpdateFunction[i];
		if (func.first == _function.first)
		{
			return;
		}
	}

	// あっちこっちでの処理を行いたいのを考えるためリストにする
	mUpdateFunction.emplace_back(_function);
	mnProcessingFunctionCount++;
}

void Transitor::AddTransitorProcessingUpdate(std::string _functionName, const std::function<void()> _func)
{
	// 同じものを多重で入れないようにする
	for (size_t i = 0; i < mUpdateFunction.size(); i++)
	{
		const auto& func = mUpdateFunction[i];
		if (func.first == _functionName)
		{
			return;
		}
	}

	// あっちこっちでの処理を行いたいのを考えるためリストにする
	mUpdateFunction.emplace_back(_functionName, _func);
	mnProcessingFunctionCount++;
}

void Transitor::AddTransitorProcessingUpdate(const std::function<void()> _func)
{
	// Foo1 Foo2 ... のような名前が付く
	AddTransitorProcessingUpdate("Foo" + std::to_string(mnProcessingFunctionCount), _func);
	mnProcessingFunctionCount++;
}

void Transitor::AddTransitorAfterUpdate(std::pair<std::string, std::function<void()>> _func)
{
	// 同じものを多重で入れないようにする
	for (size_t i = 0; i < mAfterFunction.size(); i++)
	{
		const auto& func = mAfterFunction[i];
		if (func.first == _func.first)
		{
			return;
		}
	}

	mAfterFunction.emplace_back(_func);
	mnAfterFunctionCount++;
}

void Transitor::AddTransitorAfterUpdate(std::string _functionName, std::function<void()> _func)
{
	// 同じものを多重で入れないようにする
	for (size_t i = 0; i < mAfterFunction.size(); i++)
	{
		const auto& func = mAfterFunction[i];
		if (func.first == _functionName)
		{
			return;
		}
	}

	mAfterFunction.emplace_back(_functionName, _func);
	mnAfterFunctionCount++;
}

void Transitor::AddTransitorAfterUpdate(std::function<void()> _func)
{
	// Foo1 Foo2 ... のような名前が付く
	AddTransitorProcessingUpdate("Foo" + std::to_string(mnAfterFunctionCount), _func);
	mnAfterFunctionCount++;
}

void Transitor::TransitorUpdate()
{
	for (auto& updateFunction : mUpdateFunction)
	{
		// 実行する
		updateFunction.second();
	}
}

void Transitor::TransitorAfter()
{
	for (auto& afterFunction : mAfterFunction)
	{
		afterFunction.second();
	}
}

void Transitor::FadeDraw()
{
	float alpha = 255.0f * mf_t;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(alpha));
	DrawBox(0, 0, Master::Width, Master::Height, GetColor(0, 0, 0), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}