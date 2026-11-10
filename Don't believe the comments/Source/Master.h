#pragma once
#include "SceneManager.h"
#include "SoundManager.h"
#include "ResourceManager.h"
#include "EffectManager.h"
#include "FontManager.h"
#include "QuestionManager.h"
#include "UIManager.h"
#include "MouseManager.h"
#include "CommenterManager.h"
#include "Viewers.h"
#include "Transitor.h"
#include "Pause.h"

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
	static FontManager* mpFontManager;//フォントマネージャーのポインタ
	static QuestionManager* mpQuestionManager;//問題生成マネージャーのポインタ
	static UIManager* mpUIManager;//UI管理クラスのポインタ
	static MouseManager* mpMouseManager;//マウスマネージャーのポインタ
	static CommenterManager* mpCommenterManager;//コメント表示のポインタ
	static Viewers* mpViewers;//視聴者クラスのポインタ
	static Transitor* mpTransitor;//トランジションのポインタ
	static Pause* mpPause;//ポーズのポインタ
public:
	static const int Width = 1920;
	static const int Height = 1080;

	//120分割したときの1列目
	static const int gridWidth = 16;
	static const int gridHeight = 9;
};