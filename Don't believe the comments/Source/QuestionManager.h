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

private:
	std::vector<std::vector<QuestionData>> mQuestions{};	// クイズ
	int mnCurrentIndex = 0;			// 今の問題の番号
	int mnCurrentDifficulty = 0;	// 今の問題の難易度の数値
	std::vector<std::pair<int, int>>mSpawnedQuestion{};		// 既に出した問題

public:
	QuestionManager() = default;
	~QuestionManager() = default;

public:
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

private:
	// 読み込み
	void Load(const std::string& _filePath);

	// コメント読み込み
	// 特殊なため分ける
	const std::vector<std::string> LoadComment(std::stringstream&, std::string&);

private:
	//ステージと、そのステージで出る問題の難易度と難易度の出る確率
	std::vector<std::pair<StageNumber, std::vector<GroupRate>>> stageData;

	//確率の最大値
	static const int maxRate = 90;
	//確率の最小値
	static const int minRate = 10;
};