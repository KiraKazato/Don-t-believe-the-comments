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

}

void Viewers::Update()
{
	if (mnCurrentViwers < mnTargetViwers)
	{
		mnCurrentViwers++;
		if (mnCurrentViwers >= mnTargetViwers)
		{
			mnCurrentViwers = mnTargetViwers;
		}
	}
	if (mnCurrentViwers > mnTargetViwers)
	{
		mnCurrentViwers--;
		if (mnCurrentViwers <= mnTargetViwers)
		{
			mnCurrentViwers = mnTargetViwers;
		}
	}
	if (mnCurrentViwers <= 0)
	{
		mnCurrentViwers = 0;
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

void Viewers::Draw()
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
}

void Viewers::Finalize()
{

}

void Viewers::ComeViewers()
{
	mnTargetViwers += rand() % (mnMaxAddValue - mnMinAddValue + 1) + mnMinAddValue;
	mfScale = 1.4f;
}

void Viewers::LeaveViewers()
{
	mnTargetViwers -= rand() % (mnMaxSubtractValue - mnMinSubtractValue + 1) + mnMinSubtractValue;
}

void Viewers::DecideViewers()
{
	mnTargetViwers = rand() % (mnMaxDefauktViewers - mnMinDefauktViewers + 1) + mnMinDefauktViewers;
	mnCurrentViwers = mnTargetViwers;
}
