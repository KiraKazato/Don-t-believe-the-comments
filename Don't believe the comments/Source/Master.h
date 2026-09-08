#pragma once
#include "SceneManager.h"
#include "SoundManager.h"
#include "ResourceManager.h"
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
public:
	static const int Width = 1920;
	static const int Height = 1080;
};