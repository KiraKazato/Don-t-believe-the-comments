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
	mnNumberOfComments = Master::mpResourceManager->LoadGraphics("Resource/ResultScene/Number Of Comments.png");
	mnNumberOfCorrectAnswers = Master::mpResourceManager->LoadGraphics("Resource/ResultScene/Number Of Correct Answers.png");
	mnNumberOfViewers = Master::mpResourceManager->LoadGraphics("Resource/ResultScene/Number Of Viewers.png");

	Master::mpUIManager->AddUI(mnSceneBackGroundHandle, 0, 0, Master::Width, Master::Height);
	Master::mpUIManager->AddUI(mnShowResultHandle, Master::gridWidth*23, Master::gridHeight*22, Master::gridWidth*98, Master::gridHeight*93);
	Master::mpUIManager->AddUI(mnNextButtonHandle, Master::gridWidth * 47, Master::gridHeight * 95, Master::gridWidth * 74, Master::gridHeight * 112);
	Master::mpUIManager->AddUI(mnNumberOfViewers, Master::gridWidth * 48, Master::gridHeight * 24, Master::gridWidth * 72, Master::gridHeight * 33);
	Master::mpUIManager->AddUI(mnNumberOfComments, Master::gridWidth * 71, Master::gridHeight * 57, Master::gridWidth * 87, Master::gridHeight * 63);
	Master::mpUIManager->AddUI(mnNumberOfCorrectAnswers, Master::gridWidth * 36, Master::gridHeight * 57, Master::gridWidth * 47, Master::gridHeight * 63);

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
