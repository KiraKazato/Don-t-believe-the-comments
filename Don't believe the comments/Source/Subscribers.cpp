#include "Subscribers.h"
#include "Master.h"

Subscribers::Subscribers()
{
}

Subscribers::~Subscribers()
{
}

void Subscribers::Initialize()
{
	mnCountNumberHandle = Master::mpResourceManager->LoadGraphics("Resource/GameScene/NumberV.png");

	constexpr float a = 0.8f;
	mnNumberWidth = int(63.0f * a);
	mnNumberHeight = int(144.0f * a);
	mnAllNumberHeight = mnNumberHeight * 10;
	mnDigits = 9;
	mnBlankWidth = 0;
	mnAllWidth = mnNumberWidth + mnBlankWidth;
	mnCorronWidth = 30;
	mDrawArea = Position4(Master::gridWidth * 10, Master::gridHeight * 100, Master::gridWidth * 10 + mnAllWidth * mnDigits + (mnCorronWidth * ((mnDigits-1) / 3)), Master::gridHeight * 100 + mnNumberHeight);
	mfCountTime = 0.0f;

	mNumberInfo[0].resize(mnDigits, VerticalNumberPos());
	mNumberInfo[1].resize(mnDigits, VerticalNumberPos());
	mUpperImageIndex.resize(mnDigits, 0);

	for (int i = 0; i < mnDigits; i++)
	{
		for (int j = 0; j < mNumberInfo.size(); j++)
		{
			int corron = int(mnCorronWidth * (i / 3));
			mNumberInfo[j][i] = VerticalNumberPos
			(
				mDrawArea.x1 + mnNumberWidth * i + corron,
				mDrawArea.y1 + mnAllNumberHeight * j,
				mDrawArea.x1 + mnNumberWidth * (i + 1) + corron,
				mDrawArea.y1 + mnAllNumberHeight * (j + 1),
				mnDigits - i
			);
		}
	}

	DecideSubscribers();
	RollInitialize();
}

void Subscribers::Draw()
{
	const auto& [areaX1, areaY1, areaX2, areaY2] = mDrawArea;

	DrawBox(areaX1, areaY1, areaX2, areaY2, GetColor(255, 255, 255), TRUE);

	SetDrawArea(areaX1, areaY1, areaX2, areaY2);
	
	for (size_t i = 0; i < mnDigits; i++)
	{
		for (size_t j = 0; j < mNumberInfo.size(); j++)
		{
			const auto& [x1, y1, x2, y2] = mNumberInfo[j][i].position;

			DrawExtendGraph(x1, y1, x2, y2, mnCountNumberHandle, TRUE);
		}
	}

	SetDrawAreaFull();
}

void Subscribers::Update()
{
	Roll(1.0f);

	for (int j = 1; j <= mnDigits; j++)
	{
		// 輪のように
		Ring(j);
	}
}

void Subscribers::Finalize()
{
}

void Subscribers::DecideSubscribers()
{
	const int randMin = 100;
	const int randMax = 300;

	mnSubscribersCount = 999;
}

void Subscribers::Roll(float _duration)
{
	constexpr float frameTime = 1.0f / 60.0f;
	if (_duration == 0.0f)
	{
		_duration = 0.001f;
	}

	float t = mfCountTime / _duration;
	mfCountTime += frameTime;

	if (t > 1.0f)
	{
		mfCountTime = _duration;
		t = 1.0f;
	}

	for (size_t i = 0; i < mNumberInfo.size(); i++)
	{
		for (size_t j = 1; j <= mnDigits; j++)
		{
			mNumberInfo[i][j - 1ull].LerpMove(t);
			mNumberInfo[i][j - 1ull].currentNumber = GraphToNumber(i, j);
		}
	}
}

void Subscribers::RollInitialize()
{
	// カウントを文字列へ変換
	std::string countString = std::to_string(mnSubscribersCount);

	size_t size = countString.size();

	std::vector<int> digitNumList;
	digitNumList.reserve(size);

	for (auto& c : countString)
	{
		digitNumList.push_back(c - '0');
	}

	int numberIndex = mnDigits - 1;
	for (size_t i = 0; i < digitNumList.size(); i++)
	{
		numberIndex = mnDigits - 1;
		numberIndex -= int(i + digitNumList.size()) - 1;

		NumberToRollPosition(int(digitNumList.size() - i), digitNumList[i]);
	}
}

int Subscribers::GraphToNumber(int _graphNumber, int _digits)
{
	const auto& number = mNumberInfo[_graphNumber][size_t(_digits) - 1];

	// 描画エリアからの移動量
	// どれくらい上に行ったか
	int y = mDrawArea.y2 - number.position.y1;
	y--;
	if (y < 0)
	{
		y = 0;
	}
	return y / mnNumberHeight;
}

void Subscribers::NumberToRollPosition(int _digits, int _number)
{
	int digitIndex = mnDigits - size_t(_digits);
	auto& info = mNumberInfo[GraphIndex(_digits)][digitIndex];
	auto& nextInfo = mNumberInfo[GraphNoIndex(_digits)][digitIndex];
	_number = std::min(std::max(0, _number), 9);
	auto [x1, y1, x2, y2] = mDrawArea;

	int distance = ((_number - info.currentNumber + 10) % 10);
	if (distance == 0)
	{
		distance = 10;
	}

	int moveY = distance * mnNumberWidth;

	auto SetMovePosition = [moveY](VerticalNumberPos& _info)
		{
			_info.targetPosition = _info.position;
			_info.originMovePosition = _info.position;

			_info.targetPosition.y1 -= moveY;
			_info.targetPosition.y2 -= moveY;
		};

	SetMovePosition(info);
	SetMovePosition(nextInfo);
}

Subscribers::Position4 Subscribers::NumberToPosition(int _number, int _digits) const
{
	_digits--;
	auto [x1, y1, x2, y2] = mDrawArea;

	x1 += mnNumberWidth * (mnDigits - _digits);
	y1 -= mnNumberHeight * _number;
	x2 = x1 + mnNumberWidth;
	y2 -= mnNumberHeight * _number;

	return Position4(x1, y1, x2, y2);
}

size_t Subscribers::GraphIndex(int _digits)
{
	return mUpperImageIndex[size_t(_digits) - 1];
}

size_t Subscribers::GraphNoIndex(int _digits)
{
	return 1ull - mUpperImageIndex[size_t(_digits) - 1];
}

void Subscribers::Ring(int _digits)
{
	auto& number = mNumberInfo[GraphIndex(_digits)][size_t(_digits) - 1];
	auto& nextNumber = mNumberInfo[GraphNoIndex(_digits)][size_t(_digits) - 1];

	auto& pos = number.position;

	auto& nextPos = nextNumber.position;

	// 対象の画像の一番下が表示区間を通り抜けた
	if (mDrawArea.y1 >= pos.y2)
	{
		auto& targetPos = number.targetPosition;
		auto& origMovePos = number.originMovePosition;

		// 常に繋がるようにする
		pos.y1 = nextPos.y2;
		pos.y2 = pos.y1 + mnAllNumberHeight;

		// 次の画像の一番下へ
		pos.y1 = nextNumber.position.y1 + mnAllNumberHeight;
		pos.y2 = nextNumber.position.y2 + mnAllNumberHeight;
		targetPos = origMovePos = pos;
		mUpperImageIndex[size_t(_digits) - 1] = 1 - mUpperImageIndex[size_t(_digits) - 1];
	}
}