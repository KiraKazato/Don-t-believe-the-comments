#pragma once

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

private:
	bool mbPause = false;

private:
	int mnPauseBackGroundHandle = -1;
	int mnBackToTitleHandle = -1;
	int mnBackToGameHandle = -1;

private:
	int mnX1BackToTitle = 0;
	int mnY1BackToTitle = 0;
	int mnX2BackToTitle = 0;
	int mnY2BackToTitle = 0;
	int mnDrawX1BackToTitle = 0;
	int mnDrawY1BackToTitle = 0;
	int mnDrawX2BackToTitle = 0;
	int mnDrawY2BackToTitle = 0;
	int mnX1BackToGame = 0;
	int mnY1BackToGame = 0;
	int mnX2BackToGame = 0;
	int mnY2BackToGame = 0;
	int mnDrawX1BackToGame = 0;
	int mnDrawY1BackToGame = 0;
	int mnDrawX2BackToGame = 0;
	int mnDrawY2BackToGame = 0;

	bool mbIsBackToTitleButtonClicked = false;
	bool mbIsBackToGameButtonClicked = false;

private:
	int mnMouseX{};
	int mnMouseY{};
};