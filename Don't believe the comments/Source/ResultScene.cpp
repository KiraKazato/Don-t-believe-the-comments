#include"ResultScene.h"
#include"Master.h"

ResultScene::ResultScene()
	:mnNextButtonHandle(0)
	,mnSceneBackGroundHandle(0)
	, mnShowResultHandle(0)
{

}

ResultScene::~ResultScene()
{

}

void ResultScene::Initialize()
{
	mnNextButtonHandle = Master::mpResourceManager->LoadGraphics("Resource/ResultScene/NextButton.png");
	mnSceneBackGroundHandle = Master::mpResourceManager->LoadGraphics("Resource/ResultScene/ResultBackGround.png");
	mnShowResultHandle = Master::mpResourceManager->LoadGraphics("Resource/ResultScene/ShowResult.png");
	Master::mpUIManager->AddUI(mnSceneBackGroundHandle, 0, 0, Master::Width, Master::Height);
	Master::mpUIManager->AddUI(mnShowResultHandle, Master::gridWidth*22, Master::gridHeight*9, Master::gridWidth*99, Master::gridHeight*90);
	Master::mpUIManager->AddUI(mnNextButtonHandle, Master::gridWidth * 47, Master::gridHeight * 95, Master::gridWidth * 74, Master::gridHeight * 112);
}

void ResultScene::Update()
{
	//マウス座標受け取り
	Master::mpMouseManager->GetMousePosition(&mnMouseX, &mnMouseY);

	bool insideStartButton = Master::mpUIManager->InsideUI(mnNextButtonHandle, mnMouseX, mnMouseY);
	//マウスがボタンの範囲内だったら
	if (insideStartButton)
	{
		//少し拡大
		Master::mpUIManager->ChangeUISize(mnNextButtonHandle, 1.1f);

	}
	else
	{
		//元の大きさに戻しておく
		Master::mpUIManager->ResetUISize(mnNextButtonHandle);
	}
	//左クリック押されたとき
	if (Master::mpUIManager->ClickedUI(mnNextButtonHandle, insideStartButton))
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
