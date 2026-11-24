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
		Position4 targetPosition{};		// 移動先座標
		Position4 originMovePosition{};	// 移動時の原点座標
		int digit = 0;					// 桁

		int currentNumber = 0;			// 今の数
		int startNumber = 0;			// アニメーション開始時数

		// 初期化
		VerticalNumberPos(int x1, int y1, int x2, int y2, int digit)
			:digit(digit - 1)
		{
			currentNumber = startNumber = 0;
			position = targetPosition = originMovePosition = Position4(x1, y1, x2, y2);
		}

		void LerpMove(float _t)
		{
			int y1 = this->originMovePosition.y1 + int(float(this->targetPosition.y1 - this->originMovePosition.y1) * _t);
			int y2 = this->originMovePosition.y2 + int(float(this->targetPosition.y2 - this->originMovePosition.y2) * _t);
			this->position.y1 = y1;
			this->position.y2 = y2;
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
	void NumberToRollPosition(int _digits, int _number);
	Position4 NumberToPosition(int _number, int _digits) const;

private:
	int mnCountNumberHandle{};
	int mnCorronFontHandle{};

private:
	int mnDigits{};
	int mnNumberWidth{};	// 数の横の大きさ
	int mnNumberHeight{};	// 数一つの縦の大きさ
	int mnAllNumberHeight{};// 全数字の高さ
	int mnBlankWidth{};		// 空白の幅
	int mnAllWidth{};		// 空白と数字の幅の合計
	int mnCorronWidth{};	// コロンの幅

	Position4 mDrawArea;
	
	std::array<std::vector<VerticalNumberPos>, 2> mNumberInfo;
	std::vector<int> mUpperImageIndex;

private:
	int mnSubscribersCount{};		//登録者数
	float mfCountTime{};			//移動時間

};