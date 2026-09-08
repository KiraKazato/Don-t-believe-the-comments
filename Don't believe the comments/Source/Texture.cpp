#include "Texture.h"
#include "DxLib.h"

Texture::Texture(std::string filename, VECTOR centerPosition, int transFlag)
	: mnHandle(-1)
	, mvPosition(centerPosition)
	, mnSizeX(0)
	, mnSizeY(0)
	, mnTransFlag(transFlag)
{
	// 画像の読み込み
	mnHandle = LoadGraph(filename.c_str());

	//画像のサイズを取得する
	GetGraphSize(mnHandle, &mnSizeX, &mnSizeY);

	//半径を設定 (とりあえず共通で幅の半分を半径として億
	mfRadius = (float)mnSizeX / 2.0f;
}
Texture::~Texture()
{
	if (mnHandle >= 0)
	{
		DeleteGraph(mnHandle);
		mnHandle = -1;
	}
}

void Texture::Draw()
{
	// 中心座標を計算
	int sizeX = GetSizeX();
	int sizeY = GetSizeY();

	DrawRotaGraph3(
		static_cast<int>(mvPosition.x), // 中心X
		static_cast<int>(mvPosition.y), // 中心Y
		sizeX / 2,                     // 中心X（相対）
		sizeY / 2,                     // 中心Y（相対）
		mScaleX, mScaleY,              // スケール
		0.0f,                          // 回転角
		mnHandle,                     // ハンドル
		TRUE                           // 透過
	);
	//enemyの初期位置の設定
	DrawGraph(static_cast<int>(mvPosition.x - (mnSizeX / 2)), static_cast<int>( mvPosition.y - (mnSizeY / 2)), mnHandle, mnTransFlag);
}

void Texture::Update()
{

}

void Texture::SetScale(float scaleX, float scaleY)
{
	mScaleX = scaleX;
	mScaleY = scaleY;
}


void Texture::ChangeTexture(const std::string& filename)
{
	// すでにハンドルがあるなら削除
	if (mnHandle >= 0) {
		DeleteGraph(mnHandle);
	}

	// 新しい画像を読み込む
	mnHandle = LoadGraph(filename.c_str());
}