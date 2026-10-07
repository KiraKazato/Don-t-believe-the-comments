#include "TitleScene.h"
#include "Master.h"

TitleScene::TitleScene()
	:titleBackGroundHandle(-1)
	, startBottonHandle(-1)
	, settingBottonHandle(-1)
{
}

TitleScene::~TitleScene()
{
	
}

void TitleScene::Initialize()
{
	//ハンドルの取得とUIManaerへの追加
	titleBackGroundHandle = Master::mpResourceManager->LoadGraphics("Resource/TitleScene/TitleBackGround.png");
	startBottonHandle = Master::mpResourceManager->LoadGraphics("Resource/TitleScene/StartButton.png");
	settingBottonHandle = Master::mpResourceManager->LoadGraphics("Resource/TitleScene/SettingButton.png");
	Master::mpUIManager->AddUI(titleBackGroundHandle, 0, 0, Master::Width, Master::Height);
	Master::mpUIManager->AddUI(startBottonHandle, Master::gridWidth * 30, Master::gridHeight * 90, Master::gridWidth * 57, Master::gridHeight * 107);
	Master::mpUIManager->AddUI(settingBottonHandle, Master::gridWidth * 63, Master::gridHeight * 90, Master::gridWidth * 90, Master::gridHeight * 107);
}

void TitleScene::Update()
{
	//マウス座標受け取り
	Master::mpMouseManager->GetMousePosition(&MouseX, &MouseY);
	
	//マウスがボタンの範囲内だったら
	if (Master::mpUIManager->InsideUI(startBottonHandle,MouseX,MouseY))
	{
		//少し拡大
		Master::mpUIManager->ChangeSize(startBottonHandle, 1.1f);
		//左クリック押されたとき
		if (Master::mpMouseManager->IsLeftClick())
		{
			Master::mpSceneManager->SetNextScene(SceneManager::SCENE_TYPE::SCENE_GAME);
		}
	}
	else
	{
		//元の大きさに戻しておく
		Master::mpUIManager->ResetSize(startBottonHandle);
	}


	//マウスがボタンの範囲内だったら
	if (Master::mpUIManager->InsideUI(settingBottonHandle, MouseX, MouseY))
	{
		//少し拡大
		Master::mpUIManager->ChangeSize(settingBottonHandle, 1.1f);
		if (Master::mpMouseManager->IsLeftClick())
		{
			Master::mpSceneManager->SetNextScene(SceneManager::SCENE_TYPE::SCENE_GAME);
		}
	}
	else
	{
		//元の大きさに戻しておく
		Master::mpUIManager->ResetSize(settingBottonHandle);
	}

	Scene::Update();
}

void TitleScene::Draw()
{
	Master::mpUIManager->DrawUI();
	Scene::Draw();
}

void TitleScene::Finalize()
{
	Master::mpUIManager->ClearUI();
}




