#include "TitleScene.h"
#include "Master.h"

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
	Master::mpUIManager->AddUI(mnTitleBackGroundHandle, 0, 0, Master::Width, Master::Height);
	Master::mpUIManager->AddUI(mnStartBottonHandle, Master::gridWidth * 30, Master::gridHeight * 90, Master::gridWidth * 57, Master::gridHeight * 107);
	Master::mpUIManager->AddUI(mnSettingBottonHandle, Master::gridWidth * 63, Master::gridHeight * 90, Master::gridWidth * 90, Master::gridHeight * 107);
}

void TitleScene::Update()
{
	//マウス座標受け取り
	Master::mpMouseManager->GetMousePosition(&mnMouseX, &mnMouseY);
	
	//マウスがボタンの範囲内だったら
	if (Master::mpUIManager->InsideUI(mnStartBottonHandle,mnMouseX,mnMouseY))
	{
		//少し拡大
		Master::mpUIManager->ChangeUISize(mnStartBottonHandle, 1.1f);
		//左クリック押されたとき
		if (Master::mpMouseManager->IsLeftClick())
		{
			Master::mpSceneManager->SetNextScene(SceneManager::SCENE_TYPE::SCENE_SELECT);
		}
	}
	else
	{
		//元の大きさに戻しておく
		Master::mpUIManager->ResetUISize(mnStartBottonHandle);
	}


	//マウスがボタンの範囲内だったら
	if (Master::mpUIManager->InsideUI(mnSettingBottonHandle, mnMouseX, mnMouseY))
	{
		//少し拡大
		Master::mpUIManager->ChangeUISize(mnSettingBottonHandle, 1.1f);
		if (Master::mpMouseManager->IsLeftClick())
		{
			Master::mpSceneManager->SetNextScene(SceneManager::SCENE_TYPE::SCENE_SETTING);
		}
	}
	else
	{
		//元の大きさに戻しておく
		Master::mpUIManager->ResetUISize(mnSettingBottonHandle);
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




