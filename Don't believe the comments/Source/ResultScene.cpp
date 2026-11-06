#include"ResultScene.h"
#include"Master.h"

ResultScene::ResultScene()
	:mnNextButtonHandle(0)
	,mnSceneBackGroundHandle(0)
	, mnShowResultHandle(0)
	,mnNumberOfComments(0)
	,mnNumberOfCorrectAnswers(0)
	,mnNumberOfViewers(0)
	,mnStreamEnded(0)
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
	mnStreamEnded = Master::mpResourceManager->LoadGraphics("Resource/ResultScene/StreamEnded.png");

	mnViewersCountFont = Master::mpFontManager->GetFontHandle(FontManager::FONT_TETUBINN, 115);
	mnViewersStartCountFont = Master::mpFontManager->GetFontHandle(FontManager::FONT_TETUBINN, 90);
	mnConmmentsCountFont = Master::mpFontManager->GetFontHandle(FontManager::FONT_TETUBINN, 90);
	mbAnimationFlag.reset();

	Master::mpUIManager->AddUI(mnSceneBackGroundHandle, 0, 0, Master::Width, Master::Height);
	Master::mpUIManager->AddUI(mnShowResultHandle, Master::gridWidth*24, Master::gridHeight*23, Master::gridWidth*97, Master::gridHeight*93);
	Master::mpUIManager->AddUI(mnNextButtonHandle, Master::gridWidth * 47, Master::gridHeight * 95, Master::gridWidth * 74, Master::gridHeight * 112);
	Master::mpUIManager->AddUI(mnNumberOfViewers, Master::gridWidth * 48, Master::gridHeight * 25, Master::gridWidth * 72, Master::gridHeight * 34);
	Master::mpUIManager->AddUI(mnNumberOfComments, Master::gridWidth * 71, Master::gridHeight * 57, Master::gridWidth * 87, Master::gridHeight * 63);
	Master::mpUIManager->AddUI(mnNumberOfCorrectAnswers, Master::gridWidth * 36, Master::gridHeight * 57, Master::gridWidth * 47, Master::gridHeight * 63);
	Master::mpUIManager->AddUI(mnStreamEnded, Master::gridWidth * 43, Master::gridHeight * 8, Master::gridWidth * 82, Master::gridHeight * 20);

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
		Master::mpTransitor->XFadeInit(SceneManager::SCENE_TYPE::SCENE_SELECT);
	}
	Scene::Update();

}

void ResultScene::Draw()
{
	Master::mpUIManager->DrawUI();

	/*for (size_t i = 0; i <= 120; i++)
	{
		int T = 1;
		if (i % 5 == 0)
		{
			T = 2;
		}
		if (i % 20 == 0)
		{
			T = 4;
		}

		DrawLine(
			Master::gridWidth * i, 0,
			Master::gridWidth * i, Master::Height,
			0, T);
		DrawLine(
			0, Master::gridHeight * i,
			Master::Width, Master::gridHeight * i,
			0, T);
	}*/

	DrawViewersCount(1.0f, Master::mpViewers->GetViewersCount(), Master::mpViewers->GetStartViewersCount());

	DrawCommentCount(100);

	DrawCorrectAnswerCount(10000);

	Scene::Draw();
}

void ResultScene::Finalize()
{
	Master::mpUIManager->ClearUI();

}

void ResultScene::DrawViewersCount(float _viwersCountTime, int _count, int _startCount)
{
	unsigned int color = GetColor(50, 255, 50);
	char sign = '+';
	if (Master::mpViewers->GetViewersCount() < 0)
	{
		sign = '-';
		color = GetColor(255, 50, 50);
	}

	int drawCount = ViewersCount(_viwersCountTime, _count);

	ViewersAnimation(0, drawCount, _startCount, sign, color);
}

void ResultScene::DrawCommentCount(int _count)
{
	int commentCountWidth, commentCountHeight;
	GetDrawFormatStringSizeToHandle(&commentCountWidth, &commentCountHeight, NULL, mnViewersCountFont, "%d", _count);
	
	int commentDrawPosX = Master::gridWidth * (20 * 4 - 1) - commentCountWidth / 2;
	int commentDrawPosY = Master::gridHeight * (20 * 3 + 5 * 3) - commentCountHeight / 2;
	DrawFormatStringToHandle(commentDrawPosX, commentDrawPosY, GetColor(0, 0, 0), mnViewersCountFont, "%d", _count);
}

void ResultScene::DrawCorrectAnswerCount(int _count)
{
	int correctAnswerCountWidth, correctAnswerCountHeight;
	GetDrawFormatStringSizeToHandle(&correctAnswerCountWidth, &correctAnswerCountHeight, NULL, mnViewersCountFont, "%d", _count);

	int correctAnswerCountDrawPosX = Master::gridWidth * (20 * 2 + 1) + (Master::gridWidth / 2) - correctAnswerCountWidth / 2;
	int correctAnswerCountDrawPosY = Master::gridHeight * (20 * 3 + 5 * 3) - correctAnswerCountHeight / 2;
	DrawFormatStringToHandle(correctAnswerCountDrawPosX, correctAnswerCountDrawPosY, GetColor(0, 0, 0), mnViewersCountFont, "%d", _count);
}

int ResultScene::ViewersCount(float _duration, int _count)
{
	constexpr float frameTime = 1.0f / 60.0f;
	float t = mfViwersCountTime / _duration;
	mfViwersCountTime += frameTime;
	if (mfViwersCountTime >= _duration)
	{
		mfViwersCountTime = _duration;
	}
	return Lerp(0, _count, t);
}

void ResultScene::ViewersAnimation(size_t _phase, float _duration, int _count, int _startCount, char _sign, unsigned int _color)
{
	if (mbAnimationFlag.size() <= _phase)
	{
		return;
	}

	constexpr float frameTime = 1.0f / 60.0f;

	int signWidth, plusHeight;
	int startCountWidth, startCountHeight;
	int countWidth, countHeight;

	// 情報取得
	GetDrawFormatStringSizeToHandle(&signWidth, &plusHeight, NULL, mnViewersCountFont, "%c", _sign);
	GetDrawFormatStringSizeToHandle(&startCountWidth, &startCountHeight, NULL, mnViewersStartCountFont, "%d人", _startCount);
	GetDrawFormatStringSizeToHandle(&countWidth, &countHeight, NULL, mnViewersCountFont, "%d人", _count);

	constexpr int tmp = Master::gridHeight * (20 * 2 + 5);

	const float t = mfAnimationTime / _duration;

	int signDrawPosX = Master::gridWidth * (20 * 3) - (signWidth / 2);
	int signDrawPosY = tmp - (plusHeight / 2);

	int startCountDrawPosX = signDrawPosX - startCountWidth;
	int startCountDrawPosY = tmp - (startCountHeight / 2);

	int countDrawPosX = signDrawPosX + signWidth;
	int countDrawPosY = tmp - (countHeight / 2);

	int startCountAnimation = Lerp(startCountDrawPosX, Master::gridWidth * (20 * 3) - (startCountWidth / 2), t);
	int countAnimation = Lerp(countDrawPosX, Master::gridWidth * (20 * 3), t);

	if(mbAnimationFlag.test(_phase) && _phase==3)
	{
		mfAnimationTime += frameTime;
		if (mfAnimationTime <= _duration)
		{
			mfAnimationTime = _duration;
			mbAnimationFlag.set(3);
		}
	}

	// 表示
	if (_phase <= 3)
	{
		if (_phase <= 2)
		{
			DrawFormatStringToHandle(signDrawPosX, signDrawPosY + Master::gridHeight, _color, mnViewersCountFont, "%c", _sign);
		}
		else if (_phase == 3)
		{
			DrawFormatStringToHandle(startCountAnimation - Master::gridWidth, startCountDrawPosY + Master::gridHeight, GetColor(0, 0, 0), mnViewersStartCountFont, "%d人", _startCount);
			DrawFormatStringToHandle(countDrawPosX + Master::gridHeight * 2, countDrawPosY, _color, mnViewersCountFont, "%d人", _count);
			return;
		}

		DrawFormatStringToHandle(startCountDrawPosX - Master::gridWidth, startCountDrawPosY + Master::gridHeight, GetColor(0, 0, 0), mnViewersStartCountFont, "%d人", _startCount);
		DrawFormatStringToHandle(countDrawPosX + Master::gridHeight * 2, countDrawPosY, _color, mnViewersCountFont, "%d人", _count);
		return;
	}

	int allCountWidth, allCountHeight;
	GetDrawFormatStringSizeToHandle(&allCountWidth, &allCountHeight, NULL, mnViewersCountFont, "%d", _startCount + _count);

	int allCountDrawPosX = Master::gridWidth * (20 * 3 + 3) - allCountWidth / 2;
	int allCountDrawPosY = Master::gridHeight * (20 * 2 + 5) - allCountHeight / 2;

	DrawFormatStringToHandle(signDrawPosX, signDrawPosY + Master::gridHeight, _color, mnViewersCountFont, "%d", _startCount + _count);
}
