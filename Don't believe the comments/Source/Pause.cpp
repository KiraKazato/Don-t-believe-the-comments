#include "Pause.h"
#include "DxLib.h"
#include "Master.h"
Pause::Pause()
{

}

Pause::~Pause()
{

}

void Pause::Initialize()
{
	mnBackToTitleHandle = Master::mpResourceManager->LoadGraphics("Resource/GameScene/Button/BackToTitle.png");
	mnBackToPauseHandle = Master::mpResourceManager->LoadGraphics("Resource/GameScene/Button/BackToGame.png");
	mnPauseBackGroundHandle = Master::mpResourceManager->LoadGraphics("Resource/GameScene/BackGround/PauseBackGround.png");

	mnX1BackToGame = Master::gridWidth * 42;
	mnY1BackToGame = Master::gridHeight * 20;
	mnX2BackToGame = Master::gridWidth * 78;
	mnY2BackToGame = Master::gridHeight * 100;

	mnX1BackToTitle = Master::gridWidth * 48;
	mnY1BackToTitle = Master::gridHeight * 70;
	mnX2BackToTitle = Master::gridWidth * 72;
	mnX2BackToTitle = Master::gridHeight * 85;
}

void Pause::Update()
{
	if (Master::mpKeyInputManager->KeyInputDown(KEY_INPUT_ESCAPE) != 0)
	{
		mbPause = !mbPause;
	}

	if (mbPause)
	{
		ClearInputCharBuf();
		Master::mpMouseManager->GetMousePosition(&mnMouseX, &mnMouseY);
	}

	mnX1BackToGame = Master::gridWidth * 50;
	mnY1BackToGame = Master::gridHeight * 40;
	mnX2BackToGame = Master::gridWidth * 70;
	mnY2BackToGame = Master::gridHeight * 55;

	mnX1BackToTitle = Master::gridWidth * 50;
	mnY1BackToTitle = Master::gridHeight * 70;
	mnX2BackToTitle = Master::gridWidth * 70;
	mnY2BackToTitle = Master::gridHeight * 85;
}

void Pause::Draw()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 120);
	DrawBox(0, 0, Master::Width, Master::Height, GetColor(0, 0, 0), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	int backToTitleDrawX1 = mnX1BackToGame;
	int backToTitleDrawX2 = mnX2BackToGame;
	int backToTitleDrawY1 = mnY1BackToGame;
	int backToTitleDrawY2 = mnY2BackToGame;

	float mul = 1.1f;
	bool insideBackToGameButton = Master::mpUIManager->InsideUI(mnX1BackToGame, mnY1BackToGame, mnX2BackToGame, mnY2BackToGame, mnMouseX, mnMouseY);
	if (insideBackToGameButton)
	{
		backToTitleDrawX1 *= mul;
		backToTitleDrawX2 *= mul;
		backToTitleDrawY1 *= mul;
		backToTitleDrawY2 *= mul;
	}/**/

	DrawExtendGraph(Master::gridWidth * 42, Master::gridHeight * 20, Master::gridWidth * 78, Master::gridHeight * 100, mnPauseBackGroundHandle, TRUE);
	DrawExtendGraph(backToTitleDrawX1, backToTitleDrawY1, backToTitleDrawX2, backToTitleDrawY2, mnBackToPauseHandle, TRUE);
	DrawExtendGraph(mnX1BackToTitle, mnY1BackToTitle, mnX2BackToTitle, mnY2BackToTitle, mnBackToTitleHandle, TRUE);

	//ここでタイトルに戻れるようにしたり、操作説明。

	for (size_t i = 0; i < 120; i++)
	{
		int T = 1;
		if (i % 20 == 0)
		{
			T = 4;
		}
		else if (i % 5 == 0)
		{
			T = 2;
		}

		DrawLine(0, Master::gridHeight * i, Master::Width, Master::gridHeight * i, GetColor(0, 128, 0), T);
		DrawLine(Master::gridWidth * i, 0, Master::gridWidth * i, Master::Height, GetColor(0, 128, 0), T);
	}/**/
}

void Pause::StartPause()
{
	mbPause = true;
}

void Pause::EndPause()
{
	mbPause = false;
}

bool Pause::IsPause()
{
	return mbPause;
}
