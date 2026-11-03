#include "Viewers.h"

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
}

void Viewers::Update()
{

}

void Viewers::Draw()
{
	DrawFormatStringToHandle(10, 10, GetColor(0, 0, 0), mnStringHandle, "éãíÆé“êî:%d", mnCurrentViwer);
}

void Viewers::Finalize()
{

}

void Viewers::ComeViewers()
{
	mnCurrentViwer += rand() % (mnMaxAddValue - mnMinAddValue + 1) + mnMinAddValue;
}

void Viewers::LeaveViewers()
{
	mnCurrentViwer -= rand() % (mnMaxSubtractValue - mnMinSubtractValue + 1) + mnMinSubtractValue;
}

void Viewers::DecideViewers()
{
	mnCurrentViwer = rand() % (mnMaxDefauktViewers - mnMinDefauktViewers + 1) + mnMinDefauktViewers;
}
