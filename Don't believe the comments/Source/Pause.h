#pragma once
#include <vector>

struct ButtonPosition
{
	int x1, y1;
	int x2, y2;
};

class Pause
{
public:
	Pause();
	~Pause();
	void Initialize();
	void Update();
	void Draw();

	void StartPause();
	void EndPause();
	bool IsPause();

	ButtonPosition GetChangeSize(ButtonPosition _position, float _ratio);

private:
	bool mbPause = false;

private:
	int mnPauseBackGroundHandle = -1;
	int mnBackToTitleHandle = -1;
	int mnBackToGameHandle = -1;
	int mnReturnToDesktopHandle = -1;

private:
	ButtonPosition mBackToTitle{};
	
	bool mbIsBackToTitleButtonClicked = false;

private:
	ButtonPosition mBackToGame{};
	
	bool mbIsBackToGameButtonClicked = false;

private:
	ButtonPosition mReturnToDesktop{};

	bool mbIsReturnToDesktopButtonClicked = false;

private:
	int mnMouseX{};
	int mnMouseY{};
};