#include "TitleScene.h"
#include "Master.h"
#include "Transitor.h"

TitleScene::TitleScene()
	:mnTitleBackGroundHandle(-1)
	, mnStartBottonHandle(-1)
	, mnSettingBottonHandle(-1)
{
}

TitleScene::~TitleScene()
{
	
}

void TitleScene::Initialize()
{
	//ハンドルの取得とUIManaerへの追加
	mnTitleBackGroundHandle = Master::mpResourceManager->LoadGraphics("Resource/TitleScene/TitleBackGround.png");
	mnStartBottonHandle = Master::mpResourceManager->LoadGraphics("Resource/TitleScene/StartButton.png");
	mnSettingBottonHandle = Master::mpResourceManager->LoadGraphics("Resource/TitleScene/SettingButton.png");
	mnReturnToDesktop = Master::mpResourceManager->LoadGraphics("Resource/TitleScene/ReturnToDesktop.png");
	Master::mpUIManager->AddUI(mnTitleBackGroundHandle, 0, 0, Master::Width, Master::Height);
	Master::mpUIManager->AddUI(mnStartBottonHandle, Master::gridWidth * 48, Master::gridHeight * 80, Master::gridWidth * 72, Master::gridHeight * 95);
	Master::mpUIManager->AddUI(mnSettingBottonHandle, Master::gridWidth * 68, Master::gridHeight * 100, Master::gridWidth * 92, Master::gridHeight * 115);
	Master::mpUIManager->AddUI(mnReturnToDesktop, Master::gridWidth * 28, Master::gridHeight * 100, Master::gridWidth * 52, Master::gridHeight * 115);
	Master::mpTransitor->XFadeInit(true);

	Master::mpSoundManager->PlayBGM(SoundManager::BGM_TITLE);

	Master::mpViewers->Initialize();
	Master::mpQuestionManager->ResetCorrectAnswer();
	Master::mpSoundManager->PlayBGM(SoundManager::BGM_TITLE);
}

void TitleScene::Update()
{
	//マウス座標受け取り
	Master::mpMouseManager->GetMousePosition(&mnMouseX, &mnMouseY);
	
	bool insideStartButton = Master::mpUIManager->InsideUI(mnStartBottonHandle, mnMouseX, mnMouseY);
	//マウスがボタンの範囲内だったら
	if (insideStartButton)
	{
		//少し拡大
		Master::mpUIManager->ChangeUISize(mnStartBottonHandle, 1.1f);
		
	}
	else
	{
		//元の大きさに戻しておく
		Master::mpUIManager->ResetUISize(mnStartBottonHandle);
	}
	//左クリック押されたとき
	if (Master::mpUIManager->ClickedUI(mnStartBottonHandle, insideStartButton))
	{
		Master::mpSoundManager->PlaySE(SoundManager::SE_CUT);
		Master::mpTransitor->XFadeInit(SceneManager::SCENE_TYPE::SCENE_SELECT);
	}


	bool insideSettingButton = Master::mpUIManager->InsideUI(mnSettingBottonHandle, mnMouseX, mnMouseY);
	//マウスがボタンの範囲内だったら
	if (insideSettingButton)
	{
		//少し拡大
		Master::mpUIManager->ChangeUISize(mnSettingBottonHandle, 1.1f);
	}
	else
	{
		//元の大きさに戻しておく
		Master::mpUIManager->ResetUISize(mnSettingBottonHandle);
	}
	if (Master::mpUIManager->ClickedUI(mnSettingBottonHandle, insideSettingButton))
	{
		Master::mpSoundManager->PlaySE(SoundManager::SE_CUT);
		Master::mpTransitor->XFadeInit(SceneManager::SCENE_TYPE::SCENE_SETTING);
	}

	bool insideReturnToDesktopButton = Master::mpUIManager->InsideUI(mnReturnToDesktop, mnMouseX, mnMouseY);
	//マウスがボタンの範囲内だったら
	if (insideReturnToDesktopButton)
	{
		//少し拡大
		Master::mpUIManager->ChangeUISize(mnReturnToDesktop, 1.1f);
	}
	else
	{
		//元の大きさに戻しておく
		Master::mpUIManager->ResetUISize(mnReturnToDesktop);
	}
	if (Master::mpUIManager->ClickedUI(mnReturnToDesktop, insideReturnToDesktopButton))
	{
		Master::mpSoundManager->PlaySE(SoundManager::SE_CUT);
		Master::EndGame = true;
	}

	//バックスペースキーでタイトルに戻る
	if (CheckHitKey(KEY_INPUT_SPACE))
	{
		//BGMを止めてシーン遷移
		Master::mpSoundManager->StopBGM();
		Master::mpSceneManager->SetNextScene(SceneManager::SCENE_TYPE::SCENE_RESULT);
	}

	Scene::Update();
}

void TitleScene::Draw()
{
	Master::mpUIManager->DrawUI();

	/*for (size_t i = 0; i < 120; i++)
	{
		int T = 1;
		if (!(i % 20))
		{
			T = 5;
		}
		else if (!(i % 10))
		{
			T = 3;
		}
		else if (!(i % 5))
		{
			T = 2;
		}

		DrawLine(Master::gridWidth * i, 0, Master::gridWidth * i, Master::Height, GetColor(0, 0, 0), T);
		DrawLine(0, Master::gridHeight * i, Master::Width, Master::gridHeight * i, GetColor(0, 0, 0), T);
	}*/

	Scene::Draw();
}

void TitleScene::Finalize()
{
	Master::mpUIManager->ClearUI();
}




