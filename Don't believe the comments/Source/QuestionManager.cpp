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
	for (size_t i = 0; i < _group.size(); i++)
	{
		for (size_t j = 0; j < _group[i].difficultgroup.size(); j++)
		{
			int index = _group[i].difficultgroup[j] - 1;
			if (!mQuestions[index].second)
			{
				_group[i].difficultgroup.erase(_group[i].difficultgroup.begin() + j);
			}
		}
	}

	mStageData.push_back({ _stage,_group });
}

void QuestionManager::SpawnQuestion(StageNumber _number, int _questionNumber)
{
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
		mQuestions[static_cast<size_t>(inData.difficulty - 1)].first.emplace_back(inData);
	}

	for (auto& question : mQuestions)
	{
		question.second = !question.first.empty();
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

	// 累積確率
	int rateCumulative = 0;
	// 既に選んだ難易度グループ
	int rateSelected = 0;
	for (size_t i = 0; i < _data.second.size(); i++)
	{
		// 既にその難易度のグループを選んでいた場合
		if (rateSelected & (1 << _data.second[i].group))
		{
			continue;
		}

		auto& rate = _data.second[i];
		// 累積する
		rateCumulative += rate.rate;

		// 累積確率計算
		if (!(rateCumulative > GetRand(99)) || rateCumulative == 0)
		{
			continue;
		}

		// 出す問題の候補
		std::vector<int>candidate{};

		// 難易度の候補
		std::vector<int>&& difficultCandidate = DifficultCandidate(rate.difficultgroup);
		
		// 難易度候補なし（全て出題した）
		if (difficultCandidate.empty() && rateSelected == 0)
		{
			// 強制探索開始
			// もう調べた問題番号を足す
			rateSelected += 1 << _data.second[i].group;
			i = 0;
			continue;
		}
		else if (difficultCandidate.empty())
		{
			// 強制探索
			// もう調べた問題番号を足す
			rateSelected += 1 << _data.second[i].group;
			continue;
		}

		// 選んだ難易度の中で抽選する
		int index = static_cast<size_t>(GetRand(static_cast<int>(difficultCandidate.size()) - 1));

		// 難易度
		difficultIndex = difficultCandidate[index];

		for (size_t j = 0; j < mQuestions[difficultIndex].first.size(); j++)
		{
			// 被っていない問題だったなら
			if (!IsSpawnedQuestion(static_cast<int>(j), difficultIndex))
			{
				// 候補に入れる
				candidate.push_back(static_cast<int>(j));
			}
		}

		// 候補なし（全て出題した）
		if (candidate.empty() && rateSelected == 0)
		{
			// 強制探索開始
			// もう調べた問題番号を足す
			rateSelected += 1 << _data.second[i].group;
			i = 0;
			continue;
		}
		else if (candidate.empty())
		{
			// 強制探索
			// もう調べた問題番号を足す
			rateSelected += 1 << _data.second[i].group;
			continue;
		}

		// 抽選
		questionIndex = candidate.at(static_cast<size_t>(GetRand(static_cast<int>(candidate.size()) - 1)));

		// 出す問題をもう出した問題としてカウント
		mSpawnedQuestion.emplace_back(questionIndex, difficultIndex);

		break;
	}
}

std::vector<int> QuestionManager::DifficultCandidate(const std::vector<int>& _group)
{
	std::vector<int> ret{};

	for (const auto& difficulty : _group)
	{
		int index = difficulty - 1;

		// その難易度の問題を探索
		for (int i = 0; (i < mQuestions[index].first.size()); i++)
		{
			if (!IsSpawnedQuestion(static_cast<int>(i), index))
			{
				// 出ていない問題なら候補にする
				ret.push_back(index);
				break;
			}
		}
	}

	return ret;
}

std::vector<std::vector<QuestionData>> QuestionManager::QuestionBeingDifficultCandidate(const std::vector<std::vector<QuestionData>>& _QuestionList)
{
	std::vector<std::vector<QuestionData>> ret{};
	for (const auto& question : _QuestionList)
	{
		// 問題が存在しているのならば
		if (!question.empty())
		{
			ret.emplace_back(question);
		}
	}

	return ret;
}

bool QuestionManager::IsSpawnedQuestion(int _questionIndex, int _difficultIndex)
{
	const size_t size = mSpawnedQuestion.size();

	for (int i = 0; i < size; i++)
	{
		const auto& spawned = mSpawnedQuestion[i];
		if (spawned.first == _questionIndex && spawned.second == _difficultIndex)
		{
			return true;
		}
	}
	return false;
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
		return mQuestions.at(mnCurrentDifficulty).first.at(mnCurrentIndex);
	}
	catch (...)
	{
		std::string&& errorStr = "";
		
		if (mnCurrentDifficulty > mQuestions.size())
		{
			errorStr += "DifficultError";
		}
		
		if (mnCurrentIndex > mQuestions[mnCurrentDifficulty].first.size())
		{
			errorStr += "IndexError";
		}

		printfDx("Difficult:%d/%d | Index:%d/%d | Message:%s\n",
			mnCurrentDifficulty, mQuestions.size() - 1,
			mnCurrentIndex, mQuestions[mnCurrentDifficulty].first.size() - 1,
			errorStr.c_str()
		);

		
		static const QuestionData ErrorData = QuestionData(-1, "NULL_ERROR", INT_MAX, { "これはエラー用コメントです" }, { "これはエラー用コメントです" });
		return  ErrorData;
	}
}

void QuestionManager::SetRandomNumber()
{
	//乱数をランダムに
	SRand((int)time(NULL));
}
