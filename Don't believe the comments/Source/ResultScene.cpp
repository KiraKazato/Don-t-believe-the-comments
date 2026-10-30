#include"ResultScene.h"
#include"Master.h"

ResultScene::ResultScene()
	:mnNextButton(0)
	,mnEndButton(0)
	,mnSceneBackGround(0)
{

}

ResultScene::~ResultScene()
{

}

void ResultScene::Initialize()
{
	mnEndButton = Master::mpResourceManager->LoadGraphics("Resource/ResultScene/EndButton.png");
	mnNextButton = Master::mpResourceManager->LoadGraphics("Resource/ResultScene/NextButton.png");
	mnSceneBackGround = Master::mpResourceManager->LoadGraphics("Resource/ResultScene/ResultBackGround.png");

	Master::mpUIManager->AddUI(mnSceneBackGround, 0, 0, Master::Width, Master::Height);
	Master::mpUIManager->AddUI(mnNextButton, Master::gridWidth * 47, Master::gridHeight * 90, Master::gridWidth * 74, Master::gridHeight * 107);
	

}

void ResultScene::Update()
{
	//マウス座標受け取り
	Master::mpMouseManager->GetMousePosition(&mnMouseX, &mnMouseY);

	bool insideStartButton = Master::mpUIManager->InsideUI(mnNextButton, mnMouseX, mnMouseY);
	//マウスがボタンの範囲内だったら
	if (insideStartButton)
	{
		//少し拡大
		Master::mpUIManager->ChangeUISize(mnNextButton, 1.1f);

	}
	else
	{
		//元の大きさに戻しておく
		Master::mpUIManager->ResetUISize(mnNextButton);
	}
	//左クリック押されたとき
	if (Master::mpUIManager->ClickedUI(mnNextButton, insideStartButton))
	{
		Master::mpSceneManager->SetNextScene(SceneManager::SCENE_TYPE::SCENE_SELECT);
	}
	Scene::Update();

}

void ResultScene::Draw()
{
	Master::mpUIManager->DrawUI();
	Scene::Draw();
}

void ResultScene::Finalize()
{
	Master::mpUIManager->ClearUI();

}
