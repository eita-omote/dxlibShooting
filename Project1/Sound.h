#ifndef SOUND_H_
#define SOUND_H_

#include "DxLib.h"
#include <assert.h> // 音声読み込みの読込み失敗表示用
#include <string>
#include <unordered_map> // 音声の辞書【map高速版】https://qiita.com/sileader/items/a40f9acf90fbda16af51
#include <vector> // シーンのリストに使う
#include <algorithm> // シーンのリストからのfindに使う

class Sound
{
public:
    Sound() {}; // 初期化コンストラクタの定義
    ~Sound() {}; // 破棄処理デストラクタの定義

    // メモリ再生音源管理番号
    static int start; // ピロピロリン
    static int bomb; // ボン！
    // ディスク再生音源管理番号
    static int title; // タイトル画面BGM
    static int gameplay; // プレイ画面BGM
    static int ending; // ゲームオーバ画面BGM

    static std::unordered_map<std::string, int> loadDic; // ロード済の音声辞書
    static std::unordered_map<int, std::string> SE_Dic; // SEファイル名辞書
    static std::unordered_map<int, std::string> BGM_Dic; // BGMファイル名辞書
    static std::unordered_map<std::string, float> volumeDic; // 【音声ごと】の[音量]辞書

    static float volumeSE; // 効果音のベース音量(0～100%で指定)
    static float volumeBGM; // 音楽BGMのベース音量(0～100%で指定)
    static std::string playingMusic; // 再生中のBGM(同一BGMをPlayMusic指定しても再生し直さない)
    static int musicId;
    static void Load(std::string scene = "");
    // メモリから音声を再生(個別の音量設定も効かせられる)
    static void Play(int handle, int PlayType = DX_PLAYTYPE_BACK);
    // ディスク上の音楽を直接再生する(メモリ上に展開しない)
    static void PlayMusic(std::string file, int PlayType = DX_PLAYTYPE_LOOP);
    // IDからディスク上の音楽を直接再生する(メモリ上に展開しない)
    static void PlayMusic(int musicId, int PlayType = DX_PLAYTYPE_LOOP);
    // ファイルを辞書登録しmusicIDで管理する(メモリにはのせずファイル名とIDだけ登録)
    static int SetMusicFile(std::string file, bool isSE = false);
    // 音声ファイルごとのボリューム設定
    static void SetVolume(float VolumeParcent, int SoundHandle);
    // メモリ音源の音量の更新(再生中に減らせばフェードアウト)
    static int UpdateVolume(int SoundHandle = -1);
    // 再生中のディスク音楽の音量の更新
    static int UpdateMusicVolume();
    // 音声をメモリへ読込み番号GET or シーンで使わない場合はメモリから解放
    static int LoadSoundMem(const TCHAR* FileName, std::string scene, std::vector<std::string> useScenes, bool isBGM = false, int BufferNum = 3);

private:

};
#endif


