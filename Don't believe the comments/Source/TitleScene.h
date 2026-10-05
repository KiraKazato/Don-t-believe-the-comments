#pragma once
#include "Scene.h"
#include <vector>
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
	int mnTitleBackGround;

	int mnStartBotton;

	int mnTitleGraphPath;

	int mnSettingBotton;

	int MouseX, MouseY;

	bool inField = false;

	bool inField2 = false;


};

