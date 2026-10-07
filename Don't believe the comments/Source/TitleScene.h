#pragma once
#include "Scene.h"
#include "SoundManager.h"

class TitleScene :public Scene
{
public:
	TitleScene();
	~TitleScene();
	virtual void Initialize()override;
	virtual void Update()override;
	virtual void Draw()override;
	virtual void Finalize()override;

private:
	//画像ハンドル
	int titleBackGroundHandle;
	int startBottonHandle;
	int settingBottonHandle;

	//マウスの座標を受け取る
	int MouseX{};
	int MouseY{};
};

