#include "Viewers.h"
#include "DxLib.h"
#include "Master.h"

Viewers::Viewers()
	: mnDrawY1(0)
{
}

Viewers::~Viewers()
{
}

void Viewers::Initialize()
{
	mnStringHandle = Master::mpFontManager->GetFontHandle(FontManager::FONT_TETUBINN,50);
	DecideViewers();
	mfScale = 1.0f;

	mnStartViewersCount = mnCurrentViewers;
	mnCountStartViewers = mnCurrentViewers;
}

void Viewers::Update()
{
	ViewersCount(2.0f);
	mnViewersCount = mnCurrentViewers - mnStartViewersCount;
	IdleFluctuation();
}

void Viewers::Draw()
{
	DrawFormatStringToHandle(Master::gridWidth*4, Master::gridHeight*96, GetColor(0, 0, 0), mnStringHandle, "視聴者数:%d", mnCurrentViewers);
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
	if (mnCurrentViewers <= 0)
	{
		mnCurrentViewers = 0;
		return;
	}

	if (mbIsCountFinish)
	{
		if (mnCurrentViewers != mnTargetViewers)
		{
			return;
		}

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
		t = 1.0f;
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

	//追加：拡大していたら、少しずつ元の大きさ（1.0f）に戻す
	if (mfScale > 1.0f)
	{
		mfScale -= 0.03f; // 戻るスピード（大きくすると素早く戻る）
		if (mfScale < 1.0f)
		{
			mfScale = 1.0f; // 1.0f未満にならないようにピタッと止める
		}
	}
}

void Viewers::ChangeViewers(int _changeValue)
{
	//視聴者数の部分は普通の大きさで描く
	DrawStringToHandle(10, 10, "視聴者数:", GetColor(0, 0, 0), mnStringHandle);

	// 視聴者数の横幅を調べる
	int labelWidth = GetDrawStringWidthToHandle("視聴者数:", static_cast<int>(strlen("視聴者数:")), mnStringHandle);

	//数字の部分のサイズを調べて、中心から拡大する
	int numWidth, numHeight;
	GetDrawFormatStringSizeToHandle(&numWidth, &numHeight, NULL, mnStringHandle, "%d", mnCurrentViwers);

	int baseX = 10 + labelWidth; // 数字左端X
	int baseY = 10;              // 数字のY

	// 拡大したときに中心がズレないように引き算する
	int drawX = baseX - static_cast<int>((numWidth * (mfScale - 1.0f)) / 2.0f);
	int drawY = baseY - static_cast<int>((numHeight * (mfScale - 1.0f)) / 2.0f);

	//数字だけを拡大して描く
	DrawExtendFormatStringToHandle(drawX, drawY,mfScale, mfScale,GetColor(0, 0, 0),mnStringHandle,"%d",mnCurrentViwers);
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
	mnTargetViwers += rand() % (mnMaxAddValue - mnMinAddValue + 1) + mnMinAddValue;
	mfScale = 1.4f;
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

void Viewers::IdleFluctuation()
{
	if (mnTargetViewers <= 0 || Master::mpQuestionManager->IsAllClear())
	{
		return;
	}

	//基準の時間
	static int lastTime = GetNowCount();
	static int targetTime = rand() % mnMaxFluctuation + mnMinFluctuation;
	int currentTime = GetNowCount();

	int diffTime = currentTime - lastTime;

	if (diffTime >= targetTime)
	{
		lastTime = currentTime;
		targetTime= rand() % mnMaxFluctuation + mnMinFluctuation;
		if (!(mnCurrentViewers < 250))
		{
			ChangeViewers(rand() % 41 - 20);
		}
		else
		{
			ChangeViewers(rand() % 21);
		}
	}	
}
