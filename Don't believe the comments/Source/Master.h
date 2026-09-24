#pragma once
#include "SceneManager.h"
#include "SoundManager.h"
#include "ResourceManager.h"
#include "EffectManager.h"
#include "FontManager.h"
#include "QuestionManager.h"

// ゲーム内で1つだけしか存在しないものを管理するクラス
//例えば、
//SceneManagerだったり
//SoundManagerだったり
//ResourceManagerだったり

enum class EnemyType;

class Master
{
public:
	static SceneManager* mpSceneManager;  //シーンマネージャーのポインタ
	static SoundManager* mpSoundManager;  //サウンドマネージャーのポインタ
	static ResourceManager* mpResourceManager;  //サウンドマネージャーのポインタ
	static EffectManager* mpEffectManager;//エフェクトマネージャーのポインタ
	static FontManager* mpFontManager;
	static QuestionManager* mpQuestionManager;

public:
	static const int Width = 1920;
	static const int Height = 1080;

	//120分割したときの1列目
	static const int gridWidth = 16;
	static const int gridHeight = 9;
};