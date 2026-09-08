#pragma once
#include <string>
#include <vector>
class SoundManager
{
public:    // enum, struct などの定義

	//BGMの種類
	enum SOUND_BGM
	{
		BGM_TITLE = 0, //タイトル画面

	};
	//SEの種類
	enum SOUND_SE
	{
		SE_CUT = 0,  //決定音
	};
public:

	SoundManager();

	~SoundManager();

	//初期化
	void Initialize();

	//終了処理
	void Finalize();

public:    //サウンド生成系

	//BGM再生
	//SOUND_BGM... 鳴らしたい曲の種類
	//isTop... 最初から再生するかどうか
	//bool isTop = true -> デフォルト引数と呼ばれるかきかた。関数を呼び出す際に渡さなくても認識される。
	void PlayBGM(SOUND_BGM bgm, bool isTop = false);

	//SE再生
	//se...鳴らしたいSEの種類
	void PlaySE(SOUND_SE se);

	//BGMの読み込み
	void LoadBGM(SOUND_BGM bgm, std::string filename);

	//SEの読み込み
	void LoadSE(SOUND_SE se, std::string filename);

	void StopBGM();

	

private://自分で追加

	//0~100で入力
	void ChangeSEVolume(int volume);
	//0~100で入力
	void ChangeBGMVolume(int volume);

	//truncは小数部分の切り捨て
	//0~255を0~100で返す
	int  GetSEVolume() { return (int)truncf(NowSevolume / 2.555f); }
	int  GetBGMVolume() { return (int)truncf(NowBgmvolume / 2.55f); }

private:  //メンバ変数
	SOUND_BGM mnNowPlayingBgm;  //現在再生されているBGMの種類
	SOUND_SE mnNowPlayingSe;    //現在再生されているSEの種類

	std::vector<std::pair<SOUND_BGM, int>> mnBgmHandleList;  //読み込んだBGMのハンドルリスト
	std::vector<std::pair<SOUND_SE, int>> mnSeHandleList;    //読み込んだSEのハンドルリスト

private:
	int NowPlayingBgmHandle;//今再生されてるBGMのハンドル
	int NowPlayingSeHandle;//今再生されてるSEのハンドル

private://自分で追加

	int NowBgmvolume ;//今のBGMのボリューム
	int NowSevolume ;//今のSEのボリューム
};
