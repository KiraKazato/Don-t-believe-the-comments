#include "TitleScene.h"
#include "Master.h"

TitleScene::TitleScene()
	: mnStartBotton(0)
	, mnTitleBackGround(0)
	, mnTitleGraphPath(0)
{
	mnStartBotton = LoadGraph("Resource/StartButton.jpg");
}

TitleScene::~TitleScene()
{
	
}

void TitleScene::Initialize()
{
	mnTitleBackGround = Master::mpResourceManager->LoadGraphics("Resource/TitleBackGround.png");

	mnStartBotton = Master::mpResourceManager->LoadGraphics("Resource/StartButton.png");

	mnSettingBotton = Master::mpResourceManager->LoadGraphics("Resource/Settings.png");

}

void TitleScene::Update()
{
	Scene::Update();

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
}

void TitleScene::Draw()
{
	//タイトル背景
	DrawExtendGraph(0, 0, Master::Width, Master::Height, mnTitleBackGround, TRUE);

	int addPosition = 0;
	if (inField)
	{
		addPosition = 1;
	}

	int addPosition2 = 0;
	if (inField2)
	{
		addPosition2 = 1;
	}
	//スタートボタン
	DrawExtendGraph(
		Master::gridWidth * (30-addPosition),
		Master::gridHeight * (90 - addPosition),
		Master::gridWidth * (57 + addPosition),
		Master::gridHeight * (107+ addPosition), 
		mnStartBotton, TRUE);


	//設定ボタン
	DrawExtendGraph(
		Master::gridWidth * (63 - addPosition2),
		Master::gridHeight * (90 - addPosition2),
		Master::gridWidth*(90 + addPosition2), 
		Master::gridHeight*(107 + addPosition2), 
		mnSettingBotton, TRUE);
	

	/*for (int height = 0; height < Master::Height / Master::gridHeight; height++)
	{
		DrawLine(0, Master::gridHeight * height, Master::Width, Master::gridHeight * height, GetColor(0, 0, 0));
	}
	for (int width = 0; width < Master::Width / Master::gridWidth; width++)
	{
		DrawLine(Master::gridWidth * width, 0, Master::gridWidth * width, Master::Width , GetColor(0, 0, 0));
	}*/

	//X軸の中心線
	int CenterLine = Master::gridWidth * 60; // 画面幅の半分（中心のX座標）
	DrawLine(CenterLine, 0, CenterLine, Master::Height, GetColor(255, 0, 0));
}

void TitleScene::Finalize()
{

}




