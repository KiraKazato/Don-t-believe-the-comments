#include "TextureAnimation.h"


TextureAnimation::TextureAnimation(
	VECTOR position,
	std::string filename,
	int allNum,
	int xNum,
	int yNum,
	int interval
)
	: mvPosition(position)
	, mnCounter(0)
	, mnInterval(interval)
	, mnCurrentNum(0)
	, mnAllNum(allNum)
{
	mnHandleList = new int[allNum];


	int handle = LoadGraph(filename.c_str());
	if (handle == -1)
	{
		return; //読み込み失敗していたら以降は処理しない
	}

	//サイズ取得
	int sizeX, sizeY;
	GetGraphSize(handle, &sizeX, &sizeY);

	int success = LoadDivGraph(
		filename.c_str(),
		allNum,
		xNum,
		yNum,
		sizeX / xNum,
		sizeY / yNum,
		mnHandleList
	);
}
//コンストラクタ
TextureAnimation::~TextureAnimation()     //デストラクタ
{

}

void TextureAnimation::Update()  //更新
{
	//カウンタをインクリメント
	mnCounter++;
	if (mnCounter % mnInterval == 0)
	{
		mnCounter = 0;  //カウンタを戻す
		mnCurrentNum++; //テクスチャ番号を決める
		if (mnCurrentNum >= mnAllNum)   // 分割数を超えるならループさせる
		{
			mnCurrentNum = 0;  //ループさせる
		}
	}
}

void TextureAnimation::Draw()   //描画
{
	DrawGraph((int)mvPosition.x, (int)mvPosition.y, mnHandleList[mnCurrentNum], true);
}