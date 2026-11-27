#pragma once
#include <vector>
#include <array>



class Subscribers
{
	struct Position4
	{
		int x1 = 0, y1 = 0, x2 = 0, y2 = 0;

		Position4() = default;

		Position4(int x1, int y1, int x2, int y2)
			: x1(x1), y1(y1), x2(x2), y2(y2)
		{
		}
	};

	struct VerticalNumberPos
	{
		Position4 position{};			// 現在座標
		//Position4 targetPosition{};		// 移動先座標
		//Position4 originMovePosition{};	// 移動時の原点座標
		int digit = 0;					// 桁

		int currentNumber = 0;			// 今の数
		int targetNumber = 0;			// アニメーション目標数
		int startNumber = 0;

		int moveDistance = 0;			// アニメーション移動距離
		int prevMoveY = 0;

		// 初期化
		VerticalNumberPos(int x1, int y1, int x2, int y2, int digit)
			:digit(digit - 1)
		{
			currentNumber = startNumber = targetNumber = 0;
			position = Position4(x1, y1, x2, y2);
			prevMoveY = 0;
			moveDistance = 0;
		}

		void LerpMove(float _t)
		{
			_t = std::min(std::max(0.0f, _t), 1.0f);

			int currentMoveY = int(float(this->moveDistance) * _t);

			int deltaY = currentMoveY - this->prevMoveY;

			this->position.y1 -= deltaY;
			this->position.y2 -= deltaY;

			prevMoveY = currentMoveY;
		}

		VerticalNumberPos() = default;
	};

public:
	Subscribers();
	~Subscribers();

	void Initialize();
	void Draw();
	void Update();
	void Finalize();

	// 初期状態
	void DecideSubscribers();

	void Roll(float _duration);

	void RollInitialize();

	void AddSubscribers();

	void UpdateDigits();

	void SetGameEnd(bool _flag);

	int GetSubscribersCount() const;

	int GetGameStartSubscriberCount() const;
private:
	// 縦 2 つの画像をループさせるように
	// 1 つの画像が輪になるようなイメージを持てるように
	void Ring(int _digits);

	// @return 今映っている画像がどっちの画像なのか
	size_t GraphIndex(int _digits);
	// @return 今映っていない画像がどっちの画像なのか（GraphIndex()の反転）
	size_t GraphNoIndex(int _digits);
	
	// @return 画像からの数字
	int GraphToNumber(int _graphNumber, int _digits);

	// 移動先の座標を決める
	void NumberToRollPosition(int _digits, int _number, int _moveDigits, bool _isNewDigits = false);
	Position4 NumberToPosition(int _number, int _digits) const;


private:
	int mnCountNumberHandle{};
	
	int mnCommaFontHandle{};
	int mnLabelFontHandle{};

	int mnFrameGraphHandle{};

private:
	int mnDigits{};				// 桁数
	int mnTopDigits{};			// 最上位桁
	bool mbIsNewTopDigits{};	// 最上位桁が出来たか
	const int mnMaxDigits{6};	// 最大桁数

	int mnNumberWidth{};	// 数の横の大きさ
	int mnNumberHeight{};	// 数一つの縦の大きさ
	int mnAllNumberHeight{};// 全数字の高さ
	int mnBlankWidth{};		// 空白の幅
	int mnAllWidth{};		// 空白と数字の幅の合計
	int mnCommaWidth{};		// コンマの幅

	Position4 mDrawArea;
	
	std::array<std::vector<VerticalNumberPos>, 2> mNumberInfo;
	std::vector<int> mUpperImageIndex;

private:
	int mnTargetSubscribersCount{};			// 登録者数
	int mnGameStartSubscribersCount{};		// 開始時点の登録者数
	int mnStartSubscribersCount{};			// アニメーション開始時の登録者数
	int mnPendingSubscribersCount{};		// 余り部分（カウント中数値）
	float mfCountTime{};					// 移動時間
	bool mbIsRollingFinished{};				// カウント終了か

	bool mbIsGameEnd{};
	float mfEndEasingTime{};
	bool mbIsEndEasingFinish{};
};