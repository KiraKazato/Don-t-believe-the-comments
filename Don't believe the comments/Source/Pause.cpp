#include "Pause.h"
#include "DxLib.h"
#include "Master.h"
Pause::Pause()
{

}

Pause::~Pause()
{

}

void Pause::Update()
{

}

void Pause::Draw()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 120);
	DrawBox(0, 0, Master::Width, Master::Height, GetColor(0,0,0),TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//ここでタイトルに戻れるようにしたり、操作説明。
}

void Pause::StartPause()
{
	mbPause = true;
}

bool Pause::GetStartPause()
{
	return mbPause;
}
