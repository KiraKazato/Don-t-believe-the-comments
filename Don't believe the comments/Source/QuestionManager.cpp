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

	for (size_t i = _data.second.size() - 1; i <= 0; i--)
	{
		auto& rate = _data.second[i];

		// 確率計算
		if (!(rate.rate > GetRand(99)))
		{
			continue;
		}

		// 出す問題の候補
		std::vector<int>candidate{};

		// 難易度
		difficultIndex = rate.difficultgroup[static_cast<size_t>(GetRand(static_cast<int>(rate.difficultgroup.size()) - 1))] - 1;

		for (size_t i = 0; i < mQuestions[difficultIndex].size(); i++)
		{
			// 被っていない問題だったなら
			if (!IsSpawnedQuestion(static_cast<int>(i), difficultIndex))
			{
				// 候補に入れる
				candidate.push_back(static_cast<int>(i));
			}
		}

		if (candidate.empty())
		{
			printfDx("candidate EMPTY\n");

			std::vector<int> difficultCandidate{};

			for (auto& difficulty : rate.difficultgroup)
			{
				int index = difficulty - 1;

				for (int i = 0; i < mQuestions[index].size(); i++)
				{
					if (!IsSpawnedQuestion(static_cast<int>(i), index))
					{
						difficultCandidate.push_back(index);
						break;
					}
				}
			}

			if (!difficultCandidate.empty())
			{
				do
				{
					difficultIndex = difficultCandidate[static_cast<size_t>(GetRand(static_cast<int>(difficultCandidate.size()) - 1))];

					candidate.clear();

					for (size_t i = 0; i < mQuestions[difficultIndex].size(); i++)
					{
						if (!IsSpawnedQuestion(
							static_cast<int>(i),
							difficultIndex))
						{
							candidate.push_back(static_cast<int>(i));
						}
					}
				} while (candidate.empty());
			}
		}

		// 抽選
		questionIndex = candidate[static_cast<size_t>(GetRand(static_cast<int>(candidate.size()) - 1))];

		// 出す問題をもう出した問題としてカウント
		mSpawnedQuestion.emplace_back(questionIndex, difficultIndex);

		break;
	}
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

void QuestionManager::SpawnDifficultChange(int* _loopCount, int* _difficultIndex, size_t _difficultSize)
{
	// 探索限界数
	constexpr int LOOP_MAX = 5;

	// 限界まで探しても見つからない場合
	if (LOOP_MAX <= (*_loopCount) || mQuestions[(*_difficultIndex)].empty())
	{
		(*_loopCount) = 0;
		// 入れる難易度を変える
		if ((*_difficultIndex) < _difficultSize)
		{
			(*_difficultIndex)++;
			if ((*_difficultIndex) < 0)
			{
				(*_difficultIndex) = static_cast<int>(_difficultSize - 1);
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
		std::string&& errorStr = "";
		
		if (mnCurrentDifficulty > mQuestions.size())
		{
			errorStr += "DifficultError";
		}
		
		if (mnCurrentIndex > mQuestions[mnCurrentDifficulty].size())
		{
			errorStr += "IndexError";
		}

		printfDx("Difficult:%d/%d | Index:%d/%d | Message:%s\n",
			mnCurrentDifficulty, mQuestions.size() - 1,
			mnCurrentIndex, mQuestions[mnCurrentDifficulty].size() - 1,
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
