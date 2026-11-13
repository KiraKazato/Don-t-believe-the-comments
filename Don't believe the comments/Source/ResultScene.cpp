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
	mnStreamFailed = Master::mpResourceManager->LoadGraphics("Resource/ResultScene/StreamFailed.png");
	mnBackToTitle = Master::mpResourceManager->LoadGraphics("Resource/ResultScene/BackToTitle.png");

	mnViewersCountFont = Master::mpFontManager->GetFontHandle(FontManager::FONT_TETUBINN, 115);
	mnViewersStartCountFont = Master::mpFontManager->GetFontHandle(FontManager::FONT_TETUBINN, 90);
	mnConmmentsCountFont = Master::mpFontManager->GetFontHandle(FontManager::FONT_TETUBINN, 90);
	mbAnimationFlag.reset();
	
	mfAnimationDuration[0] = 1.0f;		// カウント時間
	mfAnimationDuration[1] = 0.0f;		// カウント後に見せる時間
	mfAnimationDuration[2] = 1.5f;		// カウントを合成する時間
	mfAnimationDuration[3] = 100.0f;	// 実質的な演出終わり

	Master::mpUIManager->AddUI(mnSceneBackGroundHandle, 0, 0, Master::Width, Master::Height);
	Master::mpUIManager->AddUI(mnShowResultHandle, Master::gridWidth*24, Master::gridHeight*23, Master::gridWidth*97, Master::gridHeight*93);
	Master::mpUIManager->AddUI(mnNextButtonHandle, Master::gridWidth * 47, Master::gridHeight * 95, Master::gridWidth * 74, Master::gridHeight * 112);
	Master::mpUIManager->AddUI(mnNumberOfViewers, Master::gridWidth * 48, Master::gridHeight * 25, Master::gridWidth * 72, Master::gridHeight * 34);
	Master::mpUIManager->AddUI(mnNumberOfComments, Master::gridWidth * 71, Master::gridHeight * 57, Master::gridWidth * 87, Master::gridHeight * 63);
	Master::mpUIManager->AddUI(mnNumberOfCorrectAnswers, Master::gridWidth * 36, Master::gridHeight * 57, Master::gridWidth * 47, Master::gridHeight * 63);
	
	if(Master::mpViewers->GetCurrentViewers() > 0)
	{
		Master::mpUIManager->AddUI(mnStreamEnded, Master::gridWidth * 43, Master::gridHeight * 8, Master::gridWidth * 82, Master::gridHeight * 20);
	}
	else
	{
		Master::mpUIManager->AddUI(mnStreamFailed, Master::gridWidth * 44, Master::gridHeight * 8, Master::gridWidth * 83, Master::gridHeight * 20);
	}
	Master::mpUIManager->AddUI(mnBackToTitle, Master::gridWidth * 47, Master::gridHeight * 95, Master::gridWidth * 74, Master::gridHeight * 112);

	Master::mpSoundManager->PlayBGM(SoundManager::BGM_RESULT);
}

void ResultScene::Update()
{
	//マウス座標受け取り
	Master::mpMouseManager->GetMousePosition(&mnMouseX, &mnMouseY);

	ProcessButton();

	Scene::Update();

}

void ResultScene::Draw()
{
	Master::mpUIManager->DrawUI();

	DrawViewersCount(mfAnimationDuration[0], Master::mpViewers->GetViewersCount(), Master::mpViewers->GetStartViewersCount());

	DrawCommentCount(Master::mpCommenterManager->GetCommentsCount());

	DrawCorrectAnswerCount(Master::mpQuestionManager->GetCorrectAnswerCount(), Master::mpQuestionManager->GetMaxQuestionCount());

	Scene::Draw();
}

void ResultScene::Finalize()
{
	Master::mpUIManager->ClearUI();
	Master::mpViewers->SetStartViewers(Master::mpViewers->GetCurrentViewers());
}

auto Easing = [](float x)
	{
		return x == 0
			? 0
			: x == 1
			? 1
			: x < 0.5 ? powf(2, 20 * x - 10) / 2
			: (2 - powf(2, -20 * x + 10)) / 2;
	};

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

	ViewersAnimation(drawCount, _startCount, sign, color);
}

void ResultScene::DrawCommentCount(int _count)
{
	int commentCountWidth, commentCountHeight;
	GetDrawFormatStringSizeToHandle(&commentCountWidth, &commentCountHeight, NULL, mnConmmentsCountFont, "%d", _count);
	
	int commentDrawPosX = Master::gridWidth * 79 - commentCountWidth / 2;
	int commentDrawPosY = Master::gridHeight * 75 - commentCountHeight / 2;
	DrawFormatStringToHandle(commentDrawPosX, commentDrawPosY, GetColor(0, 0, 0), mnConmmentsCountFont, "%d", _count);
}

void ResultScene::DrawCorrectAnswerCount(int _count, int _maxCount)
{
	int correctAnswerCountWidth, correctAnswerCountHeight;
	int divWidth, divHeight;
	int maxCountWidth, maxCountHeight;
	GetDrawFormatStringSizeToHandle(&correctAnswerCountWidth, &correctAnswerCountHeight, NULL, mnConmmentsCountFont, "%d", _count);
	GetDrawFormatStringSizeToHandle(&divWidth, &divHeight, NULL, mnConmmentsCountFont, "/");
	GetDrawFormatStringSizeToHandle(&maxCountWidth, &maxCountHeight, NULL, mnConmmentsCountFont, "%d", _maxCount);

	int correctAnswerCountDrawPosX = Master::gridWidth * 41 + (Master::gridWidth / 2) - correctAnswerCountWidth / 2 - Master::gridWidth * 4;
	int correctAnswerCountDrawPosY = Master::gridHeight * 75 - correctAnswerCountHeight / 2;

	int divDrawPosX = correctAnswerCountDrawPosX + correctAnswerCountWidth + divWidth / 2 - Master::gridWidth;
	int divDrawPosY = Master::gridHeight * 75 - divHeight / 2;

	int maxCountDrawPosX = divDrawPosX + divWidth;
	int maxCountDrawPosY = Master::gridHeight * 75 - maxCountHeight / 2;

	DrawFormatStringToHandle(correctAnswerCountDrawPosX, correctAnswerCountDrawPosY, GetColor(0, 0, 0), mnConmmentsCountFont, "%d", _count);
	DrawFormatStringToHandle(divDrawPosX, divDrawPosY, GetColor(0, 0, 0), mnConmmentsCountFont, "/");
	DrawFormatStringToHandle(maxCountDrawPosX, maxCountDrawPosY, GetColor(0, 0, 0), mnConmmentsCountFont, "%d", _maxCount);
}

int ResultScene::ViewersCount(float _duration, int _count)
{
	// 0.カウント
	if (mbAnimationFlag.test(0))
	{
		return abs(_count);
	}

	constexpr float frameTime = 1.0f / 60.0f;
	float t = mfViwersCountTime / _duration;
	mfViwersCountTime += frameTime;
	if (mfViwersCountTime >= _duration)
	{
		mfViwersCountTime = _duration;
		mbAnimationFlag.set(mnAnimationIndex);
		mnAnimationIndex++;
	}
	return Lerp(0, abs(_count), Easing(t));
}

void ResultScene::ViewersAnimation(int _count, int _startCount, char _sign, unsigned int _color)
{
	if (mbAnimationFlag.size() <= mnAnimationIndex)
	{
		return;
	}

	constexpr float frameTime = 1.0f / 60.0f;

	int signWidth, signHeight;
	int startCountWidth, startCountHeight;
	int countWidth, countHeight;

	// 情報取得
	GetDrawFormatStringSizeToHandle(&signWidth, &signHeight, NULL, mnViewersCountFont, "%c", _sign);
	GetDrawFormatStringSizeToHandle(&startCountWidth, &startCountHeight, NULL, mnViewersStartCountFont, "%d人", _startCount);
	GetDrawFormatStringSizeToHandle(&countWidth, &countHeight, NULL, mnViewersCountFont, "%d人", _count);

	constexpr int tmp = Master::gridHeight * 45;

	float t = mfAnimationTime[mnAnimationIndex] / mfAnimationDuration[mnAnimationIndex];
	t = std::min(std::max(0.0f, t), 1.0f);

	int signDrawPosX = Master::gridWidth * 60 - (signWidth / 2);
	int signDrawPosY = tmp - (signHeight / 2);

	int startCountDrawPosX = signDrawPosX - startCountWidth;
	int startCountDrawPosY = tmp - (startCountHeight / 2);

	int countDrawPosX = signDrawPosX + signWidth;
	int countDrawPosY = tmp - (countHeight / 2);

	int startCountAnimation = startCountDrawPosX;
	int countAnimation = countDrawPosX;

	float easingVal = Easing(t);
	if (mnAnimationIndex == 2)
	{ 
		startCountAnimation = Lerp(startCountDrawPosX, Master::gridWidth * 60 - (startCountWidth / 2), easingVal);
		countAnimation = Lerp(countDrawPosX, Master::gridWidth * 60 - (countWidth / 2), easingVal);
	}

	if (!mbAnimationFlag.test(mnAnimationIndex) && mnAnimationIndex != 0 && mnAnimationIndex != 3)
	{
		mfAnimationTime[mnAnimationIndex] += frameTime;
		bool easingFinish = (mnAnimationIndex == 2) && easingVal >= 0.9f;
		if (mfAnimationTime[mnAnimationIndex] >= mfAnimationDuration[mnAnimationIndex] || easingFinish)
		{
			mbAnimationFlag.set(mnAnimationIndex);
			mfAnimationTime[mnAnimationIndex] = mfAnimationDuration[mnAnimationIndex];
			mnAnimationIndex++;
		}
	}

	// 表示
	if (mnAnimationIndex <= 2)
	{
		int tmp = countDrawPosX;
		countDrawPosX = countAnimation;
		startCountDrawPosX = startCountAnimation;
		int footmp = signDrawPosX;
		signDrawPosX += countAnimation - tmp;
		if (mnAnimationIndex <= 1)
		{
			signDrawPosX = footmp;
			easingVal = 0.0f;
		}

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255.0f * (1.0f - easingVal)));
		DrawFormatStringToHandle(signDrawPosX, signDrawPosY + Master::gridHeight, _color, mnViewersCountFont, "%c", _sign);
		DrawFormatStringToHandle(startCountDrawPosX - Master::gridWidth, startCountDrawPosY + Master::gridHeight, GetColor(0, 0, 0), mnViewersStartCountFont, "%d人", _startCount);
		DrawFormatStringToHandle(countDrawPosX + Master::gridHeight * 2, countDrawPosY, _color, mnViewersCountFont, "%d人", _count);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		return;
	}

	if (_sign == '-')
	{
		_count *= -1;
	}
	int allCount  = _count + _startCount;
	int allCountWidth, allCountHeight;
	GetDrawFormatStringSizeToHandle(&allCountWidth, &allCountHeight, NULL, mnViewersCountFont, "%d", allCount);

	int allCountDrawPosX = Master::gridWidth * 60 - allCountWidth / 2;
	int allCountDrawPosY = Master::gridHeight * 45 - allCountHeight / 2;

	DrawFormatStringToHandle(allCountDrawPosX, allCountDrawPosY + Master::gridHeight, GetColor(0, 0, 0), mnViewersCountFont, "%d", allCount);
}

void ResultScene::ProcessButton()
{
	bool isClear = (Master::mpViewers->GetCurrentViewers() > 0);

	// もしコメントが0より大きいのなら次へのボタンを表示
	int handle =  mnBackToTitle;

	bool insideStartButton = Master::mpUIManager->InsideUI(handle, mnMouseX, mnMouseY);
	//マウスがボタンの範囲内だったら
	if (insideStartButton)
	{
		//少し拡大
		Master::mpUIManager->ChangeUISize(handle, 1.1f);

	}
	else
	{
		//元の大きさに戻しておく
		Master::mpUIManager->ResetUISize(handle);
	}
	//左クリック押されたとき
	if (Master::mpUIManager->ClickedUI(handle, insideStartButton))
	{
		Master::mpSoundManager->StopBGM();
		Master::mpTransitor->XFadeInit(SceneManager::SCENE_TYPE::SCENE_TITLE);
	}
}
