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
	mnFrameGraphHandle = Master::mpResourceManager->LoadGraphics("Resource/GameScene/BackGround/ScoreFrame.png");

	mnCommaFontHandle = Master::mpFontManager->GetFontHandle(FontManager::FONT_TETUBINN, 60);
	mnLabelFontHandle = Master::mpFontManager->GetFontHandle(FontManager::FONT_TETUBINN, 60);


	constexpr float tmp = 0.8f;
	mnNumberWidth = int(63.0f * tmp);
	mnNumberHeight = int(144.0f * tmp);
	mnAllNumberHeight = mnNumberHeight * 10;
	mnDigits = 1;
	mnBlankWidth = 0;
	mnAllWidth = mnNumberWidth + mnBlankWidth;
	mnCommaWidth = GetDrawStringWidthToHandle(",", int(strlenDx(",")), mnCommaFontHandle);
	mfEndEasingTime = 0.0f;

	int baseX = Master::gridWidth * 8 + GetDrawStringWidthToHandle("ああああ: ", static_cast<int>(strlenDx("ああああ: ")), mnLabelFontHandle);;
	int baseY = Master::gridHeight * 101 - Master::gridHeight / 2;
	mDrawArea = Position4(baseX, baseY, baseX + mnAllWidth * mnMaxDigits + (mnCommaWidth * ((mnMaxDigits - 1) / 3)), baseY + mnNumberHeight);
	mfCountTime = 0.0f;

	mNumberInfo[0].resize(mnMaxDigits, VerticalNumberPos());
	mNumberInfo[1].resize(mnMaxDigits, VerticalNumberPos());
	mUpperImageIndex.resize(mnMaxDigits, 0);

	for (int i = 0; i < mnMaxDigits; i++)
	{
		for (int j = 0; j < mNumberInfo.size(); j++)
		{
			int corron = int(mnCommaWidth * (i / 3));
			mNumberInfo[j][i] = VerticalNumberPos
			(
				mDrawArea.x1 + mnAllWidth * i + corron,
				mDrawArea.y1 + mnAllNumberHeight * j,
				mDrawArea.x1 + mnAllWidth * (i + 1) + corron,
				mDrawArea.y1 + mnAllNumberHeight * (j + 1),
				mnMaxDigits - i
			);
		}
	}

	DecideSubscribers();
	RollInitialize();
}

void Subscribers::Draw()
{
	if (mbIsGameEnd)
	{
		float t = 1.0f;
		t -= mfEndEasingTime;
		mfEndEasingTime += 1.0f / 60.0f;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255.0f * t));
		if (mfEndEasingTime >= 1.0f)
		{
			mfEndEasingTime = 1.0f;
			return;
		}
	}

	const auto& [areaX1, areaY1, areaX2, areaY2] = mDrawArea;
	//DrawExtendGraph(areaX1 - Master::gridWidth * 2, areaY1 - Master::gridHeight * 4, areaX2 + Master::gridWidth * 2, areaY2, mnFrameGraphHandle, TRUE);

	std::string labelStr = "登録者数: ";
	int width, height;
	GetDrawStringSizeToHandle(&width, &height, NULL, labelStr.c_str(), static_cast<int>(labelStr.size()), mnLabelFontHandle);
	DrawStringToHandle(Master::gridWidth * 8, Master::gridHeight * 104 , labelStr.c_str(), GetColor(0, 0, 0), mnLabelFontHandle);

	//DrawBox(areaX1, areaY1, areaX2, areaY2, GetColor(120, 120, 120), TRUE);

	SetDrawArea(areaX1, areaY1, areaX2, areaY2);

	// コンマの数
	int commaCount = (mnDigits - 1) / 3;

	// 数字の幅
	int dispWidth = mnDigits * mnNumberWidth + commaCount * mnCommaWidth;

	// 表示始点
	int startX = areaX1 + ((areaX2 - areaX1 - dispWidth) >> 1);

	for (int i = 0; i < mnDigits; i++)
	{
		// 桁のインデックス
		int digitIndex = mnMaxDigits - mnDigits + i;

		// コンマの位置
		int commaBefore = ((mnDigits - 1) / 3) - ((mnDigits - 1 - i) / 3);

		// 描画位置
		int drawX = startX + (i * mnNumberWidth) + (commaBefore * mnCommaWidth);

		for (size_t j = 0; j < mNumberInfo.size(); j++)
		{
			const auto& [x1, y1, x2, y2] = mNumberInfo[j][digitIndex].position;
			
			DrawExtendGraph(drawX, y1, drawX + mnNumberWidth, y2, mnCountNumberHandle, TRUE);

			if (!(mbIsNewTopDigits && (mnTopDigits - 1) == i))continue;
			
			DrawBox(drawX, y1, drawX + mnNumberWidth, y1 + mnNumberHeight, GetColor(255, 255, 255), TRUE);
		}
		
		if ((i < mnDigits - 1) && ((mnDigits - 1 - i) % 3 == 0))
		{
			DrawStringToHandle(drawX + mnNumberWidth, areaY2 - 70, ",", GetColor(0, 0, 0), mnCommaFontHandle);
		}
	}

	SetDrawAreaFull();

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void Subscribers::Update()
{
	Roll(3.0f);

	for (int i = 1; i <= mnMaxDigits; i++)
	{
		// 輪のように
		Ring(i);
	}

	// 終了時点で余りがある状態ならばまた回す
	if (mbIsRollingFinished && mnPendingSubscribersCount > 0)
	{
		mnTargetSubscribersCount += mnPendingSubscribersCount;
		mnPendingSubscribersCount = 0;
		RollInitialize();
	}
}

void Subscribers::Finalize()
{
}

void Subscribers::DecideSubscribers()
{
	const int randMin = 100;
	const int randMax = 300;

	mnTargetSubscribersCount = rand() % (randMax - randMin) + randMin;
	mnStartSubscribersCount = 0;
	mnGameStartSubscribersCount = mnTargetSubscribersCount;
}

void Subscribers::Roll(float _duration)
{
	if (mbIsRollingFinished)
	{
		return;
	}

	auto easing = [](float _t) 
		{
			return -(cosf(DX_PI_F * _t) - 1.0f) / 2.0f;
		};

	constexpr float frameTime = 1.0f / 60.0f;
	if (_duration == 0.0f)
	{
		_duration = 0.001f;
	}

	float t = mfCountTime / _duration;
	mfCountTime += frameTime;

	if (t >= 1.0f)
	{
		mfCountTime = _duration;
		t = 1.0f;
	}
	t = easing(t);

	for (size_t i = 0; i < mNumberInfo.size(); i++)
	{
		for (size_t j = 1; j <= mnMaxDigits; j++)
		{
			auto& info = mNumberInfo[i][j - 1ull];
			info.LerpMove(t);
			if (t >= 1.0f)
			{
				mnStartSubscribersCount = mnTargetSubscribersCount;
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

	int oldDigit = mnDigits;
	UpdateDigits();

	bool isNewDigit = mnDigits > oldDigit;
	if (isNewDigit)
	{
		mbIsNewTopDigits = isNewDigit;
	}

	for (int i = 1; i <= mnMaxDigits; i++)
	{
		int digitNum = 1;

		// 桁
		for (int j = 1; j < i; j++)
		{
			digitNum *= 10;
		}

		// その桁はどれだけ動くか
		int moveDigits = (mnTargetSubscribersCount / digitNum) - (mnStartSubscribersCount / digitNum);

		// その桁の数字
		int targetNumber = (mnTargetSubscribersCount / digitNum) % 10;

		bool isTopNewDigit = isNewDigit && (i == mnDigits);

		NumberToRollPosition(i, targetNumber, moveDigits, isTopNewDigit);
	}
}

void Subscribers::AddSubscribers()
{
	const int randMin = 500;
	const int randMax = 750;

	int addCount = rand() % (randMax - randMin) + randMin;

	// アニメーション中なら
	if (!mbIsRollingFinished)
	{
		// 初期化せず足された値を保存
		mnPendingSubscribersCount += addCount;
		return;
	}

	mnTargetSubscribersCount += addCount;
	RollInitialize();
}

void Subscribers::UpdateDigits()
{
	mnDigits = 1;

	int count = mnTargetSubscribersCount;
	while (count >= 10)
	{
		count /= 10;
		mnDigits++;
	}
}

int Subscribers::GraphToNumber(int _graphNumber, int _digits)
{
	const auto& number = mNumberInfo[_graphNumber][size_t(_digits) - 1];

	// 移動した数
	int moveNumber = number.prevMoveY / mnNumberHeight;

	return (number.startNumber + moveNumber) % 10;
}

void Subscribers::NumberToRollPosition(int _digits, int _number, int _moveDigits, bool _isNewDigits)
{
	int digitIndex = mnMaxDigits - size_t(_digits);
	if (mbIsNewTopDigits)
	{
		mnTopDigits = digitIndex + 1;
	}
	auto& info = mNumberInfo[GraphIndex(_digits)][digitIndex];
	auto& nextInfo = mNumberInfo[GraphNoIndex(_digits)][digitIndex];
	_number = std::min(std::max(0, _number), 9);

	int moveY = _moveDigits * mnNumberHeight;

	auto SetMovePosition = [moveY, _number, _isNewDigits, this](VerticalNumberPos& _info)
		{
			_info.moveDistance = moveY;
			_info.prevMoveY = 0;

			if (_isNewDigits)
			{
				_info.currentNumber = 0;
				_info.startNumber = 0;
				_info.targetNumber = 1;
				return;
			}

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

	int corron = mnCommaWidth * (digitIndex / 3);

	x1 += mnNumberWidth * digitIndex + corron;
	y1 -= mnNumberHeight * _number;

	x2 = x1 + mnNumberWidth;
	y2 = y1 + mnNumberHeight;

	return Position4(x1, y1, x2, y2);
}

void Subscribers::SetGameEnd(bool _flag)
{
	mbIsGameEnd = _flag;
}

int Subscribers::GetSubscribersCount() const
{
	return mnTargetSubscribersCount;
}

int Subscribers::GetGameStartSubscriberCount() const
{
	return mnGameStartSubscribersCount;
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
	int digitIndex = mnMaxDigits - _digits;

	// 対象の画像の一番下が表示区間を通り抜けた
	while (true)
	{
		auto& number = mNumberInfo[GraphIndex(_digits)][digitIndex];
		auto& nextNumber = mNumberInfo[GraphNoIndex(_digits)][digitIndex];

		auto& pos = number.position;
		auto& nextPos = nextNumber.position;

		if (pos.y2 > mDrawArea.y1)
		{
			break;
		}

		// 繋げる
		pos.y1 = nextPos.y2;
		pos.y2 = pos.y1 + mnAllNumberHeight;

		mUpperImageIndex[size_t(_digits) - 1] = 1 - mUpperImageIndex[size_t(_digits) - 1];
	}
}