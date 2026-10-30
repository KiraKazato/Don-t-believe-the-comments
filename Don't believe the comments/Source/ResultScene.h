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
	int mnNextButton;
	int mnEndButton;
	int mnSceneBackGround;

	//マウスの座標を受け取る
	int mnMouseX{};
	int mnMouseY{};

};

