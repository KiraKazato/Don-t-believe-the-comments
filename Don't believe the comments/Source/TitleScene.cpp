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
	titleBackGroundHandle = Master::mpResourceManager->LoadGraphics("Resource/TitleScene/TitleBackGround.png");
	startBottonHandle = Master::mpResourceManager->LoadGraphics("Resource/TitleScene/StartButton.png");
	settingBottonHandle = Master::mpResourceManager->LoadGraphics("Resource/TitleScene/SettingButton.png");
	Master::mpUIManager->AddUI(titleBackGroundHandle, 0, 0, Master::Width, Master::gridHeight);
}

void TitleScene::Update()
{
	

	GetMousePoint(&MouseX, &MouseY);

	int mouseInput = GetMouseInput();


	//ボタンの範囲取得(Startボタン)
	int buttonLeft = Master::gridWidth * 30;
	int buttonRight = Master::gridWidth * 57;
	int buttonTop = Master::gridHeight * 90;
	int buttonBottom = Master::gridHeight * 107;

	//マウスがボタンの範囲内だったら
	if (MouseX >= buttonLeft && MouseX <= buttonRight && MouseY >= buttonTop && MouseY <= buttonBottom)
	{
		inField = true;
		if ((mouseInput & MOUSE_INPUT_LEFT) != 0)
		{
			Master::mpSceneManager->SetNextScene(SceneManager::SCENE_TYPE::SCENE_GAME);
		}
	}
	else
	{
		inField = false;
	}

	//ボタンの範囲取得(設定ボタン)
	int buttonLeft2 = Master::gridWidth * 63;
	int buttonRight2 = Master::gridWidth * 90;
	int buttonTop2 = Master::gridHeight * 90;
	int buttonBottom2 = Master::gridHeight * 107;

	//マウスがボタンの範囲内だったら
	if (MouseX >= buttonLeft2 && MouseX <= buttonRight2 &&
		MouseY >= buttonTop2 && MouseY <= buttonBottom2)
	{
		inField2 = true;
		if ((mouseInput & MOUSE_INPUT_LEFT) != 0)
		{
			Master::mpSceneManager->SetNextScene(SceneManager::SCENE_TYPE::SCENE_GAME);
		}
	}
	else
	{
		inField2 = false;
	}
	if (inField)
	{
		Master::mpUIManager->ChangeSize(startBottonHandle, 1.2f);
	}
	else
	{
		Master::mpUIManager->ResetSize(startBottonHandle);
	}


	if (inField2)
	{

		Master::mpUIManager->ChangeSize(settingBottonHandle, 1.2f);
	}
	else
	{
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




