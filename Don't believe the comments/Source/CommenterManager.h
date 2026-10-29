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
	void GetCurrentPosition(int* _x, int* _y);


	//基準座標変更
	void SetCurrentWidthPosition(int _x);
	void SetCurrentHightPosition(int _y);

	int GetMaxWidthPosition() { return mnMaxCommentGridPositionX; }
	int GetMaxHeightPosition() { return mnMaxCommentGridPositionY; }

	//問題が出ているかを知る
	void QuestionStarted(bool _start);

	//問題が変わった時の処理
	void ChangeQuestion();

	//一秒を測る関数
	bool IsOneSecond();
private:
	//コメンターの情報
	std::vector<Commenter*>mCommenters;

	//表示するコメンターの情報
	std::vector<Commenter*>mDisplayCommenters;

	//テンプレコメント
	std::vector<std::pair<std::string,int>>mTemplateComments;
	
	//名前の配列(使い捨て
	std::vector<std::string>mCommenterNames;

	//すでに出した問題コメント
	std::vector<std::string>mUsedComments;

	//コメントの表示基準場所
	int mnBaseCommentGridPositionX = 89;
	int mnBaseCommentGridPositionY = 12;
	int mnCurrentCommentGridPositionX = 89;
	int mnCurrentCommentGridPositionY = 12;

	//コメント表示の最大場所
	int mnMaxCommentGridPositionX = 110;
	int mnMaxCommentGridPositionY = 113;

	//ゲームがスタートしたかのフラグ
	bool mbQuestionStarted = false;

};