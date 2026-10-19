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
	void Finalize();

	//テンプレコメントの読み込み
	void LoadTemplateComment(const std::string& _filepath);
	//コメントする人の名前の読み込み
	void LoadCommenterName(const std::string& _filepath);
	//コメントをする人の生成
	void GenerateCommenter();

	//表示するコメンターを選ぶ
	void ChooseDisplayCommenter();
	//表示するコメントを受け取る
	std::string GetDisplayComment(int _state);

	void Update();
	void Draw();

	//newしたコメンターをリストにいれる
	void SetCommenter(Commenter* _commenter);
	//newした、表示するコメンターをリストに入れる
	void SetDisplayCommenter(Commenter* _commenter);

	//基準座標取得
	void GetBasePosition(int* _x, int* _y);
private:
	//コメンターの情報
	std::vector<Commenter*>mCommenters;

	//表示するコメンターの情報
	std::vector<Commenter*>mDisplayCommenters;

	//テンプレコメント
	std::vector<std::string>mTemplateComments;
	//テンプレコメントと問題専用コメントの選択


	//名前の配列(使い捨て
	std::vector<std::string>mCommenterNames;

	//コメントの表示基準場所
	int mnBaseCommentGridPositionX = 89;
	int mnBaseCommentGridPositionY = 7;


	bool mbOneSecond = false;
};