#pragma once
#include"Scene.h"
#include "EffekseerEffect.h"

#include <bitset>

class ResultScene : public Scene
{
public:
	ResultScene();
	~ResultScene();
	virtual void Initialize()override;
	virtual void Update()override;
	virtual void Draw()override;
	virtual void Finalize()override;

	void DrawViewersCount(float _viwersCountTime, int _count, int _startCount);
	void DrawCommentCount(int _count);
	void DrawCorrectAnswerCount(int _count, int _maxCount);

	int ViewersCount(float _duration, int _count);

	void ViewersAnimation(int _count, int _startCount, char _sign, unsigned int _color);

	void ProcessButton();

private:
	template<typename _StartTy, typename _Ty>
	_Ty Lerp(_StartTy _startVal, _Ty _val, float _t);

	//画像ハンドル
	int mnNextButtonHandle;
	int mnSceneBackGroundHandle;
	int mnShowResultHandle;
	int mnNumberOfComments;
	int mnNumberOfCorrectAnswers;
	int mnNumberOfViewers;
	int mnStreamEnded;
	int mnStreamFailed;
	int mnBackToTitle;

	//フォントハンドル
	int mnViewersCountFont = -1;
	int mnViewersStartCountFont = -1;
	int mnConmmentsCountFont = -1;

	//マウスの座標を受け取る
	int mnMouseX{};
	int mnMouseY{};

	EffekseerEffect* mnResultEffect;

	//視聴者数カウント
	float mfViwersCountTime = 0.0f;

	//視聴者数アニメーションフラグ
	std::bitset<4> mbAnimationFlag{};	// 0.カウント 1.見せ 2.アニメーション(カウント合成) 3.見せ（終了）
	int mnAnimationIndex = 0;
	float mfAnimationTime[4] = { 0.0f };	// 0.カウント 1.見せ 2.アニメーション(カウント合成) 3.見せ
	float mfAnimationDuration[4] = { 0.0f };// 0.カウント 1.見せ 2.アニメーション(カウント合成) 3.見せ
};

// a + (b - a) * t
template<typename _StartTy, typename _Ty>
inline _Ty ResultScene::Lerp(_StartTy _startVal, _Ty _val, float _t)
{
	return _Ty(static_cast<float>(_startVal) + (static_cast<float>(_val) - static_cast<float>(_startVal)) * _t);
}
