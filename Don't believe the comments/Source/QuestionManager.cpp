#include "QuestionManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "DxLib.h"
#include "Master.h"

// {1,2,3} {4,5,6} {7.8}  {9,10}
// 減少大　減少中  増加大 増加中

void QuestionManager::Intialize()
{
	Load("Resource/QuizData/Quiz.csv");
	std::vector<GroupRate> initializeGroup =
	{
		GroupRate({1,{1,2,3},50,-5}),
		GroupRate({2,{4,5,6},30,-2}),
		GroupRate({3,{7,8},15,2}),
		GroupRate({4,{9,10},5,5})
	};
	SetQuestionRate(STAGE_1, initializeGroup);
}

void QuestionManager::Finalize()
{
	mStageData.clear();
	ClearSpawnedQuestion();
}

void QuestionManager::SetQuestionRate(StageNumber _stage, std::vector<GroupRate> _group)
{
	mStageData.push_back({ _stage,_group });
}

void QuestionManager::SpawnQuestion(StageNumber _number, int _questionNumber)
{
	//乱数をランダムに
	SRand((int)time(NULL));

	int setQuestionIndex = 0;
	int setDifficultIndex = 0;
	// stageData探索
	for (auto& data : mStageData)
	{
		// ステージ番号を確認
		if (RateDecision(data, _number))
		{
			continue;
		}

		Spawn(data, &setQuestionIndex, &setDifficultIndex);
	}

	// 問題を設定
	SetQuestion(setQuestionIndex, setDifficultIndex);
}

void QuestionManager::ClearSpawnedQuestion()
{
	mSpawnedQuestion.clear();
}

void QuestionManager::Load(const std::string& _filePath)
{
	mQuestions.resize(DIFFICULT_MAX);

	// 入れるためのデータを用意する
	QuestionData inData = QuestionData();

	std::ifstream ifs(_filePath, std::ios::in);

	if (!ifs)
	{
		printfDx("クイズが読み込めませんでした");
		return;
	}

	std::string line;

	// 最初の行は項目名のため飛ばす
	getline(ifs, line);

	while (getline(ifs, line))
	{
		size_t pos = 0;

		// 行の最後は\n\rとなるため、\rを消す
		while ((pos = line.find("\r")) != std::string::npos)
		{
			line.replace(pos, 2, "");
		}

		std::stringstream ss(line);
		std::string cell;
		// 問題
		if (getline(ss, cell, ','))
		{
			int questionGraphID = -1;
			try
			{
				// 問題IDを得る
				questionGraphID = stoi(cell);
			}
			catch (...)
			{
				questionGraphID = -1;

				inData.questionGraphHandle = -1;
			}

			if (questionGraphID != -1)
				inData.questionGraphHandle = Master::mpResourceManager->LoadGraphics("Resource/QuizImage/Quiz" + std::to_string(questionGraphID) + ".png");
		}

		// 答え
		if (getline(ss, cell, ',')) inData.answer = cell;
		
		// 難易度
		if (getline(ss, cell, ',')) 
		{
			// エラー発生時の処理
			try
			{
				inData.difficulty = stoi(cell); 
			}
			catch (...)
			{
				inData.difficulty = 0;
			}
		}

		// 真コメント
		if (getline(ss, cell, ','))
		{
			std::stringstream ssComent(cell);
			std::string cellComment;

			inData.trueComment = LoadComment(ssComent, cellComment);

		}

		// 偽コメント
		if (getline(ss, cell, ','))
		{
			std::stringstream ssComent(cell);
			std::string cellComment;

			inData.falseComment = LoadComment(ssComent, cellComment);
		}

		// 入れる
		mQuestions[inData.difficulty - 1].emplace_back(inData);
	}
}

const std::vector<std::string> QuestionManager::LoadComment(std::stringstream& _ss, std::string& _cell)
{
	std::vector<std::string> ret;

	// } で区切る
	while (getline(_ss, _cell, '}'))
	{
		size_t pos = 0;

		// 処理をする上で {～ というようになっているため削除
		while ((pos = _cell.find("{")) != std::string::npos)
		{
			_cell.replace(pos, 1, "");
		}

		// 入れる
		ret.emplace_back(_cell);
	}
	return ret;
}

bool QuestionManager::RateDecision(std::pair<StageNumber, std::vector<GroupRate>>& _data, StageNumber _stageNumber)
{
	if (_data.first != _stageNumber)
	{
		return true;
	}

	//確率の合計
	int totalRate = 0;
	//グループの確率の確定
	//問題数が進んでいたならここで確率の増減を適用する。
	for (auto& rate : _data.second)
	{
		rate.rate += (_stageNumber - 1) * rate.changeRate;
		rate.rate = std::min(std::max(minRate, rate.rate), maxRate);
		totalRate += rate.rate;
	}

	for (auto& rate : _data.second)
	{
		float normalizedRate = (float)rate.rate / totalRate;

		rate.rate = (int)(normalizedRate * 100);
	}

	return false;
}

void QuestionManager::Spawn(std::pair<StageNumber, std::vector<GroupRate>> _data, int* _questionIndex, int* _difficultIndex)
{
	int& questionIndex = *_questionIndex;
	int& difficultIndex = *_difficultIndex;

	int difficultGroupIndex = 0;
	for (auto& rate : _data.second)
	{
		// 確率計算
		if (!(rate.rate > GetRand(100)))
		{
			continue;
		}

		// 難易度
		difficultIndex = rate.difficultgroup[static_cast<size_t>(GetRand(static_cast<int>(rate.difficultgroup.size()) - 1))] - 1;
		difficultGroupIndex = difficultIndex;

		// 抽選
		questionIndex = GetRand(static_cast<int>(mQuestions[difficultIndex].size()) - 1);

		bool isFound = true;
		// 何回ループしたか
		int loopCount = 0;
		// 既に出題した問題があったか
		while (true)
		{
			if (mSpawnedQuestion.empty() || !IsSpawnedQuestion(questionIndex, difficultIndex))
			{
				break;
			}

			// もう一度抽選
			questionIndex = GetRand(static_cast<int>(mQuestions[difficultIndex].size()) - 1);
			loopCount++;
			
			// 限界まで探しても見つからない場合
			SpawnDifficultChange(&loopCount, _difficultIndex, _data.second.size());
		}

		// 出す問題をもう出した問題としてカウント
		mSpawnedQuestion.emplace_back
		(
			std::make_pair
			(
				questionIndex,
				difficultIndex
			)
		);
	}
	difficultGroupIndex++;
}

bool QuestionManager::IsSpawnedQuestion(int _questionIndex, int _difficultIndex)
{
	for (auto& spawned : mSpawnedQuestion)
	{
		if (spawned.first == _questionIndex && spawned.second == _difficultIndex)
		{
			return true;
		}
	}
	return false;
}

void QuestionManager::SpawnDifficultChange(int* _loopCount, int* _difficultIndex, size_t _difficultSize)
{
	// 探索限界数
	constexpr int LOOP_MAX = 5;

	// 限界まで探しても見つからない場合
	if (LOOP_MAX <= *_loopCount)
	{
		*_loopCount = 0;
		// 入れる難易度を変える
		if (*_difficultIndex < _difficultSize)
		{
			*_difficultIndex++;
			if (*_difficultIndex < 0)
			{
				*_difficultIndex = static_cast<int>(_difficultSize - 1);
			}
		}
	}
}

void QuestionManager::SetQuestion(int _index, int _difficulty)
{
	mnCurrentIndex = _index;
	mnCurrentDifficulty = _difficulty;
}

const QuestionData& QuestionManager::GetQuestionData()
{
	// mnCurrentIndexがエラー部分を参照したらエラーデータを返す
	try
	{
		return mQuestions.at(mnCurrentDifficulty).at(mnCurrentIndex);
	}
	catch (...)
	{
		static const QuestionData ErrorData = QuestionData(-1, "NULL_ERROR", INT_MAX, { "これはエラー用コメントです" }, { "これはエラー用コメントです" });
		return  ErrorData;
	}
}
