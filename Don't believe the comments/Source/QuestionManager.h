#pragma once
#include <string>
#include <vector>
#include <sstream>

//ステージ番号
enum StageNumber
{
	STAGE_1 = 0,
};

struct GroupRate
{
	int group;
	std::vector<int>difficultgroup;
	int rate;
	int changeRate;
};

struct QuestionData
{
	int questionGraphHandle = -1;	// 問題
	std::string answer{};	// 答え
	int difficulty = 0;		// 難易度
	std::vector<std::string> trueComment{};		// 真コメント
	std::vector<std::string> falseComment{};	// 偽コメント

	// コンストラクタ
	// データを作る際、明示的に作ることができる
	QuestionData(int questionGraphHandle, const std::string& answer, int difficulty, const std::vector<std::string>& trueComment, const std::vector<std::string>& falseComment)
		: questionGraphHandle(questionGraphHandle), answer(answer), difficulty(difficulty), trueComment(trueComment), falseComment(falseComment)
	{
	}
	// 空データを作成可能
	// QuestionData(void)
	QuestionData(void) = default;
};

class QuestionManager
{
private:
	static const size_t DIFFICULT_MAX = 10;

public:
	QuestionManager() = default;
	~QuestionManager() = default;

	void Intialize();
	
	void Finalize();

	//ステージごとの確率設定
	// クイズ出現（出題）
	// @param stage	出すステージ
	// @param group	その難易度とその確率
	void SetQuestionRate(StageNumber _stage, std::vector<GroupRate> _group);

	// クイズの出現
	void SpawnQuestion(StageNumber _number, int _questionNumber);

	// 既に出た問題のリストをクリアする
	void ClearSpawnedQuestion();

	// 問題番号を指定して問題を出す
	void SetQuestion(int _index, int _difficulty);
	
	// 現在の問題の情報を得る
	const QuestionData& GetQuestionData();

	// 乱数設定（乱数完全ランダム化）
	void SetRandomNumber();

private:
	// 読み込み
	void Load(const std::string& _filePath);

	// コメント読み込み
	// 特殊なため分ける
	const std::vector<std::string> LoadComment(std::stringstream&, std::string&);

	// 確率の確定
	// @return 出題に進んで行けないか
	bool RateDecision(std::pair<StageNumber, std::vector<GroupRate>>& _data, StageNumber _stageNumber);

	// 問題の確定
	void Spawn(std::pair<StageNumber, std::vector<GroupRate>> _data, int* _questionIndex, int* _difficultIndex);

	// 難易度の選出
	std::vector<int> DifficultCandidate(const std::vector<int>& _group);

	// 問題が存在する難易度の選出
	std::vector<std::vector<QuestionData>> QuestionBeingDifficultCandidate(const std::vector<std::vector<QuestionData>>& _QuestionList);

	// 出題済み問題の探索
	// @return 出題済みか
	bool IsSpawnedQuestion(int _questionIndex, int _difficultIndex);

private:
	std::vector<std::pair<std::vector<QuestionData>, bool>> mQuestions{};	// クイズ

	int mnCurrentIndex = 0;			// 今の問題の番号
	int mnCurrentDifficulty = 0;	// 今の問題の難易度の数値
	
	std::vector<std::pair<int, int>>mSpawnedQuestion{};		// 既に出した問題

private:
	//ステージと、そのステージで出る問題の難易度と難易度の出る確率
	std::vector<std::pair<StageNumber, std::vector<GroupRate>>> mStageData;

	//確率の最大値
	static const int maxRate = 90;
	//確率の最小値
	static const int minRate = 5;
};