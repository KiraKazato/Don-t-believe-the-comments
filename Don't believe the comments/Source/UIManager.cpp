#include "UIManager.h"

void UIManager::AddUI(int _handle, float _x1, float _y1, float _x2, float _y2)
{
	mUIList.push_back({ _handle,_x1,_y1,_x2,_y2,_x1,_y1,_x2,_y2 });
}

void UIManager::DrawUI()
{
	//中身を参照しつつ、勝手な変更バグをなくす
	for (const auto& ui : mUIList)
	{
		//画像の表示
		DrawExtendGraphF(ui.drawX1, ui.drawY1, ui.drawX2, ui.drawY2, ui.handle, TRUE);
	}
}

void UIManager::ClearUI()
{
	mUIList.clear();
}

void UIManager::ChangeSize(int _handle, float _ratio)
{
	float half = 0.5f;//半分
	float centerX, centerY;//オリジナル座標での真ん中
	float halfWidth, halfHeight;//比率を適用した座標の真ん中
	for (auto& ui : mUIList)
	{
		if (ui.handle == _handle)
		{
			//X、Yの真ん中を求める
			centerX = (ui.originalX1 + ui.originalX2) * half;
			centerY = (ui.originalY1 + ui.originalY2) * half;
			//元の幅を求めて、半分にし、比率をかける。
			halfWidth = (ui.originalX2 - ui.originalX1) * half * _ratio;
			halfHeight = (ui.originalY2 - ui.originalY1) * half * _ratio;
			//元の真ん中から比率適用した真ん中からの長さを引く
			ui.drawX1 = centerX - halfWidth;
			ui.drawY1 = centerY - halfHeight;
			//元の真ん中に比率適用した真ん中からの長さを足す
			ui.drawX2 = centerX + halfWidth;
			ui.drawY2 = centerY + halfHeight;
			break;
		}
	}
}


void UIManager::ResetSize(int _handle)
{
	for (auto&ui : mUIList)
	{
		if (ui.handle == _handle)
		{
			ui.drawX1 = ui.originalX1;
			ui.drawX2 = ui.originalX2;
			ui.drawY1 = ui.originalY1;
			ui.drawY2 = ui.originalY2;
			break;
		}
	}
}

bool UIManager::InsideUI(int _handle, int _objectX, int _objectY)
{
	bool isInsideX{}, isInsideY{};
	for (auto& ui : mUIList)
	{
		if (ui.handle == _handle)
		{
			isInsideX = (ui.drawX1 < _objectX && _objectX < ui.drawX2);
			isInsideY = (ui.drawY1 < _objectY && _objectY < ui.drawY2);
			break;
		}
		
	}
	if (isInsideX && isInsideY)
	{
		return true;
	}
	return false;
}

