#pragma once
#include"Scene.h"
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
	void DrawCorrectAnswerCount(int _count);

	int ViewersCount(float _duration, int _count);

	void ViewersAnimation(size_t _phase, float _duration, int _count, int _startCount, char _sign, unsigned int _color);

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

	//フォントハンドル
	int mnViewersCountFont = -1;
	int mnViewersStartCountFont = -1;
	int mnConmmentsCountFont = -1;

	//マウスの座標を受け取る
	int mnMouseX{};
	int mnMouseY{};

	//視聴者数カウント
	float mfViwersCountTime = 0.0f;

	//視聴者数アニメーションフラグ
	std::bitset<5> mbAnimationFlag{};
	float mfAnimationTime = 0.0f;
};

template<typename _StartTy, typename _Ty>
inline _Ty ResultScene::Lerp(_StartTy _startVal, _Ty _val, float _t)
{
	_t = (((0.0f) < ((((_t) > (1.0f)) ? (_t) : (1.0f)))) ? (0.0f) : ((((_t) > (1.0f)) ? (_t) : (1.0f))));
	return _Ty(static_cast<float>(_startVal + (_val - _startVal)) * _t);
}
