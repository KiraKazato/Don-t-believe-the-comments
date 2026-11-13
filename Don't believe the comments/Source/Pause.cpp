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
	mnReturnToDesktopHandle = Master::mpResourceManager->LoadGraphics("Resource/GameScene/Button/ReturnToDesktop.png");

	{
		auto& [x1, y1, x2, y2] = mBackToGame;
		x1 = Master::gridWidth * 48;
		y1 = Master::gridHeight * 35;
		x2 = Master::gridWidth * 72;
		y2 = Master::gridHeight * 50;
	}

	{
		auto& [x1, y1, x2, y2] = mBackToTitle;
		x1 = Master::gridWidth * 48;
		y1 = Master::gridHeight * 55;
		x2 = Master::gridWidth * 72;
		y2 = Master::gridHeight * 70;
	}

	{
		auto& [x1, y1, x2, y2] = mReturnToDesktop;
		x1 = Master::gridWidth * 48;
		y1 = Master::gridHeight * 75;
		x2 = Master::gridWidth * 72;
		y2 = Master::gridHeight * 90;
	}
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
}

void Pause::Draw()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 120);
	DrawBox(0, 0, Master::Width, Master::Height, GetColor(0, 0, 0), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawExtendGraph(Master::gridWidth * 42, Master::gridHeight * 20, Master::gridWidth * 78, Master::gridHeight * 100, mnPauseBackGroundHandle, TRUE);

	bool insideBackToGameButton = Master::mpUIManager->InsideUI(mBackToGame.x1, mBackToGame.y1, mBackToGame.x2, mBackToGame.y2, mnMouseX, mnMouseY);
	if (insideBackToGameButton)
	{
		// 構造化束縛と呼ばれる書き方
		// 構造体を分解しながら代入という手法が可能である ※C++17からである
		auto [x1, y1, x2, y2] = GetChangeSize(mBackToGame, 1.1f);
		DrawExtendGraph(x1, y1, x2, y2, mnBackToGameHandle, TRUE);
	}
	else
	{
		DrawExtendGraph(mBackToGame.x1, mBackToGame.y1, mBackToGame.x2, mBackToGame.y2, mnBackToGameHandle, TRUE);
	}

	if (Master::mpUIManager->ClickedUI(&mbIsBackToGameButtonClicked, insideBackToGameButton))
	{
		EndPause();
	}

	bool insideBackToTitleButton = Master::mpUIManager->InsideUI(mBackToTitle.x1, mBackToTitle.y1, mBackToTitle.x2, mBackToTitle.y2, mnMouseX, mnMouseY);

	if (insideBackToTitleButton)
	{
		auto [x1, y1, x2, y2] = GetChangeSize(mBackToTitle, 1.1f);
		DrawExtendGraph(x1, y1, x2, y2, mnBackToTitleHandle, TRUE);
	}
	else
	{
		DrawExtendGraph(mBackToTitle.x1, mBackToTitle.y1, mBackToTitle.x2, mBackToTitle.y2, mnBackToTitleHandle, TRUE);
	}

	if (Master::mpUIManager->ClickedUI(&mbIsBackToTitleButtonClicked, insideBackToTitleButton))
	{
		// 初期化
		std::function<void()> afterFunction = [&]()
			{
				Master::mpTextInputManager->SetNoInput();
				Master::mpTextInputManager->Finalize();

				// 評価を初期化
				Master::mpCommenterManager->ResetCommenterReputation();

				Master::mpViewers->Initialize();
				Master::mpTextInputManager->Initialize();
			};

		Master::mpTransitor->AddTransitorAfterUpdate("PauseAfter", afterFunction);

		EndPause();

		Master::mpTransitor->XFadeInit(SceneManager::SCENE_TITLE);
	}

	bool insideEndToGameButton = Master::mpUIManager->InsideUI(mReturnToDesktop.x1, mReturnToDesktop.y1, mReturnToDesktop.x2, mReturnToDesktop.y2, mnMouseX, mnMouseY);
	if (insideEndToGameButton)
	{
		auto [x1, y1, x2, y2] = GetChangeSize(mReturnToDesktop, 1.1f);
		DrawExtendGraph(x1, y1, x2, y2, mnReturnToDesktopHandle, TRUE);
	}
	else
	{
		DrawExtendGraph(mReturnToDesktop.x1, mReturnToDesktop.y1, mReturnToDesktop.x2, mReturnToDesktop.y2, mnReturnToDesktopHandle, TRUE);
	}


	if (Master::mpUIManager->ClickedUI(&mbIsReturnToDesktopButtonClicked, insideEndToGameButton))
	{
		Master::EndGame = true;
		Master::mpTransitor->XFadeInit(SceneManager::SCENE_TITLE);
	}
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

ButtonPosition Pause::GetChangeSize(ButtonPosition _position, float _ratio)
{
	constexpr float half = 0.5f;//半分
	int centerX, centerY;//オリジナル座標での真ん中
	int halfWidth, halfHeight;//比率を適用した座標の真ん中
	//X、Yの真ん中を求める
	centerX = int(float(_position.x1 + _position.x2) * half);
	centerY = int(float(_position.y1 + _position.y2) * half);
	//元の幅を求めて、半分にし、比率をかける。
	halfWidth = int(float(_position.x2 - _position.x1) * half * _ratio);
	halfHeight = int(float(_position.y2 - _position.y1) * half * _ratio);
	//元の真ん中から比率適用した真ん中からの長さを引く
	_position.x1 = centerX - halfWidth;
	_position.y1 = centerY - halfHeight;
	//元の真ん中に比率適用した真ん中からの長さを足す
	_position.x2 = centerX + halfWidth;
	_position.y2 = centerY + halfHeight;

	return _position;
}
