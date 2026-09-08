#pragma once
#include"DxLib.h"
#include <string>
#include "Object2D.h"
#include "TextureAnimation.h"
class TextureAnimation
{
public:
	TextureAnimation(
		VECTOR position,
		std::string filename,
		int allNum,
		int xNum,
		int yNum,
		int interval
    );
	//コンストラクタ
	~TextureAnimation();     //デストラクタ

	void Update();  //更新

	void Draw();   //描画

	void SetPosition(VECTOR centerPosition) { mvPosition ; }

	VECTOR GetPosition() { return mvPosition; }
	int GetSizeX() { return mnSizeX; }
	int GetSizeY() { return mnSizeY; }
	float GetRadius() { return mfRadius; }

private:
	VECTOR mvPosition; // ポジション
	int mnCounter;  //アニメーションカウンタ
	int mnInterval;   //テクスチャ切り替えのフレーム数
	int mnCurrentNum;  //何番目のテクスチャを表示するか
	int* mnHandleList; //分割されたテクスチャのハンドルリスト
	int mnAllNum;      // テクスチャ分割数
	int mnSizeX;
	int mnSizeY;
	int mnTransFlag;
	float mfRadius;

};

