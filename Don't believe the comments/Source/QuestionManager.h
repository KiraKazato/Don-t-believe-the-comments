#pragma once
#include <string>
#include <vector>
#include <sstream>

struct QuestionData
{
	std::string question = "";	// 問題
	std::string answer = "";	// 答え
	int difficulty = 0;			// 難易度
	std::vector<std::string> trueComment;		// 真コメント
	std::vector<std::string> falseComment;		// 偽コメント

	// コンストラクタ
	// データを作る際、明示的に作ることができる
	QuestionData(const std::string& question, const std::string& answer, int difficulty, const std::vector<std::string>& trueComment, const std::vector<std::string>& falseComment)
		: question(question), answer(answer), difficulty(difficulty), trueComment(trueComment), falseComment(falseComment)
	{
	}
	// 空データを作成可能
	// QuestionData(void)
	QuestionData(void) = default;
};

class QuestionManager
{
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

	// クイズ出現（出題） ※要素数は同数にしてください
	// @param difficulties	出す難易度				書き方例）{5,4,3,2,1}
	// @param probability	その難易度の問題が出る確率（単位:％）	書き方例）{5,20,40,50,60}
	void SpawnQuiz(const std::vector<int>& _difficulties, const std::vector<int>& _probability);

	// 問題番号を指定して問題を出す
	void SetQuestion(int _index, int _difficulty);
	
	// 現在の問題の情報を得る
	const QuestionData& GetQuestionData();

	// ロードが終わったか
	bool IsLoadFinish() const { return mbIsLoadFinish; };

private:
	// 読み込み
	void Load(const std::string& _filePath);
	
	// コメント読み込み
	// 特殊なため分ける　
	const std::vector<std::string> LoadComment(std::stringstream&, std::string&);


	bool mbIsLoadFinish = false;	// ロード処理を作る時に使えるはず
};