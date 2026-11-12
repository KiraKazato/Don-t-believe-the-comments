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
	mnBackToGameHandle = Master::mpResourceManager->LoadGraphics("Resource/GameScene/Button/BackToGame.png");
	mnPauseBackGroundHandle = Master::mpResourceManager->LoadGraphics("Resource/GameScene/BackGround/PauseBackGround.png");
}

void Pause::Update()
{
	mnX1BackToGame = Master::gridWidth * 50;
	mnY1BackToGame = Master::gridHeight * 40;
	mnX2BackToGame = Master::gridWidth * 70;
	mnY2BackToGame = Master::gridHeight * 55;

	mnX1BackToTitle = Master::gridWidth * 50;
	mnY1BackToTitle = Master::gridHeight * 70;
	mnX2BackToTitle = Master::gridWidth * 70;
	mnY2BackToTitle = Master::gridHeight * 85;

	if (Master::mpKeyInputManager->KeyInputDown(KEY_INPUT_ESCAPE) != 0)
	{
		mbPause = !mbPause;
	}

	if (mbPause)
	{
		ClearInputCharBuf();
		Master::mpMouseManager->GetMousePosition(&mnMouseX, &mnMouseY);
	}


	mnDrawX1BackToGame = mnX1BackToGame;
	mnDrawX2BackToGame = mnX2BackToGame;
	mnDrawY1BackToGame = mnY1BackToGame;
	mnDrawY2BackToGame = mnY2BackToGame;

	constexpr float half = 0.5f;
	constexpr float mul = 1.1f;
	int centerX, centerY;
	int halfWidth, halfHeight;

	bool insideBackToGameButton = Master::mpUIManager->InsideUI(mnX1BackToGame, mnY1BackToGame, mnX2BackToGame, mnY2BackToGame, mnMouseX, mnMouseY);
	if (insideBackToGameButton)
	{
		centerX = int(float(mnX1BackToGame + mnX2BackToGame) * half);
		centerY = int(float(mnY1BackToGame + mnY2BackToGame) * half);

		halfWidth = int(float(mnX2BackToGame - mnX1BackToGame) * half * mul);
		halfHeight = int(float(mnY2BackToGame - mnY1BackToGame) * half * mul);

		mnDrawX1BackToGame = centerX - halfWidth;
		mnDrawX2BackToGame = centerX + halfWidth;

		mnDrawY1BackToGame = centerY - halfHeight;
		mnDrawY2BackToGame = centerY + halfHeight;
	}/**/

	if (Master::mpUIManager->ClickedUI(&mbIsBackToGameButtonClicked, insideBackToGameButton))
	{
		EndPause();
	}

	bool insideBackToTitleButton = Master::mpUIManager->InsideUI(mnX1BackToTitle, mnY1BackToTitle, mnX2BackToTitle, mnY2BackToTitle, mnMouseX, mnMouseY);

	mnDrawX1BackToTitle = mnX1BackToTitle;
	mnDrawX2BackToTitle = mnX2BackToTitle;
	mnDrawY1BackToTitle = mnY1BackToTitle;
	mnDrawY2BackToTitle = mnY2BackToTitle;

	if (insideBackToTitleButton)
	{
		centerX = int(float(mnX1BackToTitle + mnX2BackToTitle) * half);
		centerY = int(float(mnY1BackToTitle + mnY2BackToTitle) * half);

		halfWidth = int(float(mnX2BackToTitle - mnX1BackToTitle) * half * mul);
		halfHeight = int(float(mnY2BackToTitle - mnY1BackToTitle) * half * mul);

		mnDrawX1BackToTitle = centerX - halfWidth;
		mnDrawX2BackToTitle = centerX + halfWidth;

		mnDrawY1BackToTitle = centerY - halfHeight;
		mnDrawY2BackToTitle = centerY + halfHeight;
	}/**/

	if (Master::mpUIManager->ClickedUI(&mbIsBackToTitleButtonClicked, insideBackToTitleButton))
	{
		// 初期化
		{
			Master::mpTextInputManager->SetNoInput();
			Master::mpTextInputManager->Finalize();

			// 評価を初期化
			Master::mpCommenterManager->ResetCommenterReputation();

			Master::mpViewers->Initialize();
		}

		EndPause();

		Master::mpTransitor->XFadeInit(SceneManager::SCENE_TITLE);
	}
}

void Pause::Draw()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 120);
	DrawBox(0, 0, Master::Width, Master::Height, GetColor(0, 0, 0), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawExtendGraph(Master::gridWidth * 42, Master::gridHeight * 20, Master::gridWidth * 78, Master::gridHeight * 100, mnPauseBackGroundHandle, TRUE);
	DrawExtendGraph(mnDrawX1BackToGame, mnDrawY1BackToGame, mnDrawX2BackToGame, mnDrawY2BackToGame, mnBackToGameHandle, TRUE);
	DrawExtendGraph(mnDrawX1BackToTitle, mnDrawY1BackToTitle, mnDrawX2BackToTitle, mnDrawY2BackToTitle, mnBackToTitleHandle, TRUE);

	//ここでタイトルに戻れるようにしたり、操作説明。
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
