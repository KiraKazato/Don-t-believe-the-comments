#pragma once
#include <string>
#include "DxLib.h"
class Texture
{
public:
	//コンストラクタ
	Texture(std::string filename, VECTOR centerPosition, int transFlag);

	~Texture();

	void Draw();  //描画
	void Update(); //更新

	void SetPosition(VECTOR centerPosition) { mvPosition = centerPosition; }
	VECTOR GetPosition() { return mvPosition; }

	int GetHandle() const { return mnHandle; }

	//サイズ取得
	int GetSizeX() { return mnSizeX; }
	int GetSizeY() { return mnSizeY; }

	float GetRadius() { return mfRadius; }

	void SetScale(float scaleX, float scaleY);

	void ChangeTexture(const std::string& filename);
	void Load(const std::string& filename) { ChangeTexture(filename); }
	float GetScaleX() const { return mScaleX; }
	float GetScaleY() const { return mScaleY; }

	void SetChangeTexture(bool flag) { mbChangeTexture = flag; }
	bool GetChangeTexture() const { return mbChangeTexture; }

private:
	int mnHandle;   //読み込んだ画像のハンドル  
	VECTOR mvPosition;//ポジション
	int mnSizeX;      //画像の幅
	int mnSizeY;      //画像の高さ
	int mnTransFlag;  //画面の透過を有効にするか
	float mfRadius;  //半径
	float mScaleX = 1.0f;
	float mScaleY = 1.0f;
	bool mbChangeTexture = true;

};