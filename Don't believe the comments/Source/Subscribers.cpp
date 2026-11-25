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
				mDrawArea.x1 + mnAllWidth * i + corron,
				mDrawArea.y1 + mnAllNumberHeight * j,
				mDrawArea.x1 + mnAllWidth * (i + 1) + corron,
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
	Roll(3.0f);

	for (int i = 1; i <= mnDigits; i++)
	{
		// —Ö‚Ì‚æ‚¤‚É
		Ring(i);
	}
}

void Subscribers::Finalize()
{
}

void Subscribers::DecideSubscribers()
{
	const int randMin = 100;
	const int randMax = 300;

	mnTargetSubscribersCount = 199;
	mnStartSubscribersCount = 0;
}

void Subscribers::Roll(float _duration)
{
	if (mbIsRollingFinished)
	{
		mnStartSubscribersCount = mnTargetSubscribersCount;
		return;
	}

	auto easing = [](float _t) 
		{
			return _t == 1.0f ? 1.0f : 1.0f - powf(2.0f, -10.0f * _t);
		};

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
	t = easing(t);

	for (size_t i = 0; i < mNumberInfo.size(); i++)
	{
		for (size_t j = 1; j <= mnDigits; j++)
		{
			auto& info = mNumberInfo[i][j - 1ull];
			info.LerpMove(t);

			if (t >= 1.0f)
			{
				mbIsRollingFinished = true;
				info.currentNumber = info.targetNumber;
			}
		}
	}
}

void Subscribers::RollInitialize()
{
	mfCountTime = 0.0f;
	mbIsRollingFinished = false;

	for (int i = 1; i < mnDigits; i++)
	{
		int digitNum = 1;

		for (int j = 1; j < i; j++)
		{
			digitNum *= 10;
		}

		int moveDigits = (mnTargetSubscribersCount / digitNum) - (mnStartSubscribersCount / digitNum);

		int targetNumber = (mnTargetSubscribersCount / digitNum) % 10;

		NumberToRollPosition(i, targetNumber, moveDigits);
	}
}

void Subscribers::AddSubscribers()
{
	const int randMin = 250;
	const int randMax = 500;

	mnTargetSubscribersCount += rand() % (randMax - randMin) + randMin;
	RollInitialize();
}

int Subscribers::GraphToNumber(int _graphNumber, int _digits)
{
	const auto& number = mNumberInfo[_graphNumber][size_t(_digits) - 1];

	// ˆÚ“®‚µ‚½”
	int moveNumber = number.prevMoveY / mnNumberHeight;

	return (number.startNumber + moveNumber) % 10;
}

void Subscribers::NumberToRollPosition(int _digits, int _number, int _moveDigits)
{
	int digitIndex = mnDigits - size_t(_digits);
	auto& info = mNumberInfo[GraphIndex(_digits)][digitIndex];
	auto& nextInfo = mNumberInfo[GraphNoIndex(_digits)][digitIndex];
	_number = std::min(std::max(0, _number), 9);
	auto [x1, y1, x2, y2] = mDrawArea;

	int moveY = _moveDigits * mnNumberHeight;

	auto SetMovePosition = [moveY,_number](VerticalNumberPos& _info)
		{
			_info.moveDistance = moveY;
			_info.prevMoveY = 0;
			_info.startNumber = _info.currentNumber;
			_info.targetNumber = _number;
		};

	SetMovePosition(info);
	SetMovePosition(nextInfo);
}

Subscribers::Position4 Subscribers::NumberToPosition(int _number, int _digits) const
{
	int digitIndex = mnDigits - _digits;

	auto [x1, y1, x2, y2] = mDrawArea;

	int corron = mnCorronWidth * (digitIndex / 3);

	x1 += mnNumberWidth * digitIndex + corron;
	y1 -= mnNumberHeight * _number;

	x2 = x1 + mnNumberWidth;
	y2 = y1 + mnNumberHeight;

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

	// ‘ÎÛ‚Ì‰æ‘œ‚Ìˆê”Ô‰º‚ª•\Ž¦‹æŠÔ‚ð’Ê‚è”²‚¯‚½
	if (pos.y2 <= mDrawArea.y1)
	{
		// í‚ÉŒq‚ª‚é‚æ‚¤‚É‚·‚é
		pos.y1 = nextPos.y2;
		pos.y2 = pos.y1 + mnAllNumberHeight;

		mUpperImageIndex[size_t(_digits) - 1] = 1 - mUpperImageIndex[size_t(_digits) - 1];
	}
}