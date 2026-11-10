#include "Viewers.h"
#include "DxLib.h"
#include "Master.h"

Viewers::Viewers()
{
}

Viewers::~Viewers()
{
}

void Viewers::Initialize()
{
	mnStringHandle = Master::mpFontManager->GetFontHandle(FontManager::FONT_TETUBINN,50);
	DecideViewers();

	mnStartViewersCount = mnCurrentViewers;
	mnCountStartViewers = mnCurrentViewers;
}

void Viewers::Update()
{
	ViewersCount(2.0f);

	mnViewersCount = mnCurrentViewers - mnStartViewersCount;
}

void Viewers::Draw()
{
	DrawFormatStringToHandle(10, 10, GetColor(0, 0, 0), mnStringHandle, "視聴者数:%d", mnCurrentViewers);
}

void Viewers::Finalize()
{

}

void Viewers::ComeViewers()
{
	int changeValue = rand() % (mnMaxAddValue - mnMinAddValue + 1) + mnMinAddValue;
	ChangeViewers(changeValue);
}

void Viewers::LeaveViewers()
{
	int changeValue = rand() % (mnMaxSubtractValue - mnMinSubtractValue + 1) + mnMinSubtractValue;
	changeValue *= -1;
	ChangeViewers(changeValue);
}

void Viewers::DecideViewers()
{
	mnTargetViewers = rand() % (mnMaxDefauktViewers - mnMinDefauktViewers + 1) + mnMinDefauktViewers;
	mnCurrentViewers = mnTargetViewers;
	
}

void Viewers::ViewersCount(float _duration)
{
	if (mbIsCountFinish)
	{
		return;
	}

	if (mnCurrentViewers <= 0)
	{
		mnCurrentViewers = 0;
		return;
	}

	//1フレームの時間
	constexpr float frameTime = 1.0f / 60.0f;
	//経過時間と求められる時間の割合
	float t = mfViewersCountTime / _duration;
	mfViewersCountTime += frameTime;
	if (mbIsCountFinish = (mfViewersCountTime >= _duration))
	{
		//経過した（計測終了）
		mfViewersCountTime = _duration;
	}
	//線形補完
	//@{ 
	int subViewers = mnTargetViewers - mnCountStartViewers; //目標値と開始値の差
	mnCurrentViewers = mnCountStartViewers + static_cast<int>(static_cast<float>(subViewers) * t);
	//@}
	if (mbIsCountFinish)
	{
		mnCountStartViewers = mnCurrentViewers;
	}
}

void Viewers::ChangeViewers(int _changeValue)
{
	mnTargetViewers += _changeValue;
	mfViewersCountTime = 0.0f;
	//終わっていなかったら、開始値を増やす
	if (!mbIsCountFinish)
	{
		mnCountStartViewers = mnCurrentViewers;
	}
	else
	{
		//計測開始
		mbIsCountFinish = false;
	}
}

void Viewers::SetStartViewers(int _count)
{
	mnStartViewersCount = _count;
}

int Viewers::GetStartViewersCount()
{
	return mnStartViewersCount;
}

int Viewers::GetViewersCount()
{
	return mnViewersCount;
}

int Viewers::GetTargetViewers()
{
	return mnTargetViewers;
}

int Viewers::GetCurrentViewers()
{
	return mnCurrentViewers;
}
