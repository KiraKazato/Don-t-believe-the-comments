#pragma once
#include "DxLib.h"
#include "QuestionManager.h"
#include "TextInputManager.h"

class GameCycle
{
public:
	GameCycle();
	~GameCycle();
	void Initialize();
	void Update();
	void Draw();
	void Finalize();
	void SetStage(StageNumber _stageNumeber);
private:
	void CommentDraw();//コメントの表示
	
	void QuestionNumberDraw();//何問目かの表示

	void QuestionDraw();//問題文の表示
	
	void InputAnswer();//回答の入力
	
	bool AnswerJudge();//回答の判定

	void GameEnd();
private:
	int inputStringHandle;
	int questionNumber = 1;
	int maxQuestionNumber = 7;
	
private:
	bool isQuestionNumberDraw = false;
	bool isQuestionDraw = false;
	bool isInputAnswer = false;
	bool QuestionSpawned = false;
private:
	std::string answer{};
private:
	TextInputManager mTextInputManager;
	StageNumber stageNumber = STAGE_1;
};