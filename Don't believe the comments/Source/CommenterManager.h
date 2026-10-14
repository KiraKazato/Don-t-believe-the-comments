#pragma once
#include "DxLib.h"
#include <vector>
#include <string>

class Commenter;

class CommenterManager
{
public:
	CommenterManager();
	~CommenterManager();

	void Initialize();

	//テンプレコメントの読み込み
	void LoadTemplateComment();
	//コメントする人の名前の読み込み
	void LoadCommenterName();
	//コメントをする人の生成
	void GenerateCommenter();

	void Update();
	void Draw();

	//newしたコメンターをリストにいれる
	void SetCommenter(Commenter* _commenter);
private:
	//コメンターの情報
	std::vector<Commenter*>mCommenters;

	//テンプレコメント
	std::vector<std::string>mTemplateComments;

	//名前の配列(使い捨て
	std::vector<std::string>mCommenterNames;

	bool isOneSecond = false;
};