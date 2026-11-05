#pragma once
#include"Scene.h"

class ResultScene : public Scene
{
public:
	ResultScene();
	~ResultScene();
	virtual void Initialize()override;
	virtual void Update()override;
	virtual void Draw()override;
	virtual void Finalize()override;

private:

	//画像ハンドル
	int mnNextButtonHandle;
	int mnSceneBackGroundHandle;
	int mnShowResultHandle;
	int mnNumberOfComments;
	int mnNumberOfCorrectAnswers;
	int mnNumberOfViewers;

	//マウスの座標を受け取る
	int mnMouseX{};
	int mnMouseY{};

};

