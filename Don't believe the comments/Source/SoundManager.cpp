#include "SoundManager.h"
#include "DxLib.h"
SoundManager::SoundManager()
	: mnNowPlayingBgm((SOUND_BGM)-1)   //初期状態は何も再生されていない状態
	, mnNowPlayingSe((SOUND_SE)-1)     //初期状態は何も再生されていない状態
	, NowPlayingBgmHandle(-1)   //初期状態は何も再生されていない状態
	, NowPlayingSeHandle(-1)     //初期状態は何も再生されていない状態
	,NowBgmvolume(255)
	,NowSevolume(255)
{

}

SoundManager::~SoundManager()
{

}

void SoundManager::Initialize()
{

}

void SoundManager::Finalize()
{
	//BGMの破棄
	for (auto it = mnBgmHandleList.begin(); it != mnBgmHandleList.end(); it++)
	{
		DeleteSoundMem(it->second);
	}

	for (auto it = mnSeHandleList.begin(); it != mnSeHandleList. end(); it++)
	{
		DeleteSoundMem(it->second);
	}
}

void SoundManager::PlayBGM(SOUND_BGM bgm, bool isTop)
{
	if (mnNowPlayingBgm == bgm && !isTop)
	{
		return;
	}

	StopBGM();

	for (auto it = mnBgmHandleList.begin(); it != mnBgmHandleList.end(); it++)
	{
		if (it->first == bgm)
		{
			NowPlayingBgmHandle = it->second;
			ChangeVolumeSoundMem(NowBgmvolume, it->second);
			PlaySoundMem(it->second, DX_PLAYTYPE_LOOP, TRUE);
			mnNowPlayingBgm = bgm;

			return;
		}
	}
}

void SoundManager::StopBGM()
{
	for (auto it = mnBgmHandleList.begin(); it != mnBgmHandleList.end(); it++)
	{
		StopSoundMem(it->second);
	}
	NowPlayingBgmHandle = -1;
	mnNowPlayingBgm = (SOUND_BGM)-1;
}

void SoundManager::PlaySE(SOUND_SE se)
{
	for (auto it = mnSeHandleList.begin(); it != mnSeHandleList.end(); it++)
	{
		if (it->first == se)
		{
			NowPlayingSeHandle = it->second;
			ChangeVolumeSoundMem(NowSevolume, it->second);
			PlaySoundMem(it->second, DX_PLAYTYPE_BACK);
			mnNowPlayingSe = se;
			break;
		}
	}
}



void SoundManager::LoadBGM(SOUND_BGM bgm, std::string filename)
{
	bool check = false; //重複して読み込んでいるかどうか
	for (auto it = mnBgmHandleList.begin(); it != mnBgmHandleList.end(); it++)
	{
		//一致した種類のBGMがあれば
		if (it->first == bgm)
		{
			check = true;
			break;
		}
	}
	//重複して読み込まれていたら何もしない
	if (check)
	{
		return;
	}
	//ファイル読み込み
	int handle = LoadSoundMem(filename.c_str());
	if (handle == -1)
	{
		return;  //読み込み失敗していたら何もしない
	}

	//読み込んだハンドルをリストに追加
	mnBgmHandleList.push_back(std::pair<SOUND_BGM, int>(bgm, handle));

}

void SoundManager::LoadSE(SOUND_SE se, std::string filename)
{
	bool check = false; //重複して読み込んでいるかどうか
	for (auto it = mnSeHandleList.begin(); it != mnSeHandleList.end(); it++)
	{
		//一致した種類のBGMがあれば
		if (it->first == se)
		{
			check = true;
			break;
		}
	}
	//重複して読み込まれていたら何もしない
	if (check)
	{
		return;
	}
	//ファイル読み込み
	int handle = LoadSoundMem(filename.c_str());
	if (handle == -1)
	{
		return;  //読み込み失敗していたら何もしない
	}

	//読み込んだハンドルをリストに追加
	mnSeHandleList.push_back(std::pair<SOUND_SE, int>(se, handle));
}

void SoundManager::ChangeSEVolume(int volume)
{
	//ceilfは小数部分の切り上げ
	//0~100を0~255に
	NowSevolume = (int)ceilf(volume * 2.55f);
}

void SoundManager::ChangeBGMVolume(int volume)
{
	//ceilfは小数部分の切り上げ
	// //0~100を0~255に
	NowBgmvolume = (int)ceilf(volume * 2.55f);

	//再生中でも変わるように。
	if (NowPlayingBgmHandle != -1)
	{
		ChangeVolumeSoundMem(NowBgmvolume, NowPlayingBgmHandle);
	}
}
