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

	mnStartViewersCount = mnCurrentViwers;
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
	else if (mnCurrentViwers > mnTargetViwers)
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

	mnViewersCount = mnCurrentViwers - mnStartViewersCount;
}

void Viewers::Draw()
{
	DrawFormatStringToHandle(10, 10, GetColor(0, 0, 0), mnStringHandle, "Ž‹’®ŽÒ”:%d", mnCurrentViwers);
}

void Viewers::Finalize()
{

}

void Viewers::ComeViewers()
{
	mnTargetViwers += rand() % (mnMaxAddValue - mnMinAddValue + 1) + mnMinAddValue;
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

int Viewers::GetStartViewersCount()
{
	return mnStartViewersCount;
}

int Viewers::GetViewersCount()
{
	return mnViewersCount;
}
