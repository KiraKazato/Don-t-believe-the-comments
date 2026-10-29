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
		Master::mpSceneManager->SetNextScene(SceneManager::SCENE_TYPE::SCENE_SELECT);
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
		Master::mpSceneManager->SetNextScene(SceneManager::SCENE_TYPE::SCENE_SETTING);
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




