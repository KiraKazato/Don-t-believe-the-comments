#include "UIManager.h"

std::vector<UIData> UIManager::mUIList;

void UIManager::AddUI(int _handle, float _x1, float _y1, float _x2, float _y2, float _ratio)
{
	float half = 0.5f;//半分
	float centerX, centerY;//オリジナル座標での真ん中
	float halfWidth, halfHeight;//比率を適用した座標の真ん中
	float newX1, newX2, newY1, newY2;//比率を適用した座標を入れる変数
	//X、Yの真ん中を求める
	centerX = (_x1 + _x2) * half;
	centerY = (_y1 + _y2) * half;
	//元の幅を求めて、半分にし、比率をかける。
	halfWidth = (_x2 - _x1) * half * _ratio;
	halfHeight = (_y2 - _y1) * half * _ratio;
	//元の真ん中から比率適用した真ん中からの長さを引く
	newX1 = centerX - halfWidth;
	newY1 = centerY - halfHeight;
	//元の真ん中に比率適用した真ん中からの長さを足す
	newX2 = centerX + halfWidth;
	newY2 = centerY + halfHeight;
	//比率を適用した座標を入れる
	mUIList.push_back({ _handle,newX1,newY1,newX2,newY2 });
}

void UIManager::DrawUI()
{
	//中身を参照しつつ、勝手な変更バグをなくす
	for (const auto& ui : mUIList)
	{
		//画像の表示
		DrawExtendGraphF(ui.x1, ui.y1, ui.x2, ui.y2, ui.handle, TRUE);
	}
}

void UIManager::ClearUI()
{
	mUIList.clear();
}