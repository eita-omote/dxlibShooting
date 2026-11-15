#include "Sound.h"

// メモリ再生音源管理番号
int Sound::start; // ピロピロリン
int Sound::bomb; // ボン！
// ディスク再生音源管理番号
int Sound::title; // タイトル画面BGM
int Sound::gameplay; // プレイ画面BGM
int Sound::ending; // ゲームオーバ画面BGM

std::unordered_map<std::string, int> Sound::loadDic; // ロード済の音声辞書
std::unordered_map<int, std::string> Sound::SE_Dic; // SEファイル名辞書
std::unordered_map<int, std::string> Sound::BGM_Dic; // BGMファイル名辞書
std::unordered_map<std::string, float> Sound::volumeDic; // 【音声ごと】の[音量]辞書

float Sound::volumeSE{ 100 }; // 効果音のベース音量(0～100%で指定)
float Sound::volumeBGM{ 100 }; // 音楽BGMのベース音量(0～100%で指定)
std::string Sound::playingMusic; // 再生中のBGM(同一BGMをプレイ指定しても初めから再生しない)

int Sound::musicId{ -3 }; // 音楽ID -3,-4,-5..メモリ上にのせないが番号を管理し個別に音量設定できるよう

void Sound::Load(std::string scene)
{
    start = Sound::LoadSoundMem("SE/start.ogg", scene, { "TitleScene" });
    assert(start != -1); // 読込失敗、ファイル名かフォルダ名が間違ってる
    // オプションで同時[8つ]爆発音再生を可能に(デフォルトは3)falseをtrueにするとBGM扱い
    bomb = Sound::LoadSoundMem("SE/bomb.wav", scene, { "TitleScene","PlayScene" }, false, 8);
    assert(bomb != -1); // 読込失敗、ファイル名かフォルダ名が間違ってる

    title = SetMusicFile("BGM/title.mp3");
    gameplay = SetMusicFile("BGM/gameplay.mp3");
    ending = SetMusicFile("BGM/ending.mp3");
}

// メモリ上の音声を再生する。
void Sound::Play(int handle, int PlayType)
{
    assert(handle != -1 && "プレイする音声がメモリにない?" != "");

    // 音量の更新
    UpdateVolume(handle);
    // メモリ上から音声再生
    PlaySoundMem(handle, PlayType);
}

// ディスク上の音楽を直接再生する(メモリ上に展開しない)
void Sound::PlayMusic(std::string file, int PlayType)
{
    // プレイ中のBGMを再指定しても再生し直さず即時return
    if (file == playingMusic) return;

    playingMusic = file; // プレイ中のBGM名を記録

    DxLib::PlayMusic(file.c_str(), PlayType); //ディスクから直接再生(メモリにのせない)
    Sound::UpdateMusicVolume(); // // 音量の更新(プレイ【中】の音楽だから↑PlayMusicしてから)
}
// IDからディスク上の音楽を直接再生する(メモリ上に展開しない)
void Sound::PlayMusic(int musicId, int PlayType)
{
    if (musicId > -1) return;//音楽IDはマイナスのID番号を割り振ったはず

    std::string file = "";
    // 辞書から対応するファイル名を取得
    if (BGM_Dic.count(musicId) == 1)
        file = BGM_Dic[musicId];
    else if (SE_Dic.count(musicId) == 1)
        file = SE_Dic[musicId];

    if (file == "") return;//対応する音楽ファイルが辞書に未登録
    // ファイル名から音楽ファイルを再生
    Sound::PlayMusic(file, PlayType);
}

// ファイルを辞書登録しmusicIDで管理する(メモリにはのせずファイル名とIDだけ登録)
int Sound::SetMusicFile(std::string file, bool isSE)
{    // 既に辞書登録済なら即時return
    if (loadDic.count(file) == 1) return loadDic[file];

    // 新規に辞書にファイル登録
    loadDic[file] = musicId; //IDの辞書に登録
    if (isSE) SE_Dic[musicId] = file; //SEの辞書に登録
    else BGM_Dic[musicId] = file; //BGMの辞書に登録

    return musicId--; // 後ろに--つけるとカウントダウン前をreturn
}

// 音声ファイルごとのボリューム設定
void Sound::SetVolume(float VolumeParcent, int SoundHandle)
{
    if (SoundHandle == -1) return;

    if (SE_Dic.count(SoundHandle) == 1)
    {    // ハンドル番号がSEの辞書で見つかれば
        std::string file = SE_Dic[SoundHandle];
        volumeDic[file] = VolumeParcent;//個別音量を辞書登録
    }
    else if (BGM_Dic.count(SoundHandle) == 1)
    {    // ハンドル番号がBGMの辞書で見つかれば
        std::string file = BGM_Dic[SoundHandle];
        volumeDic[file] = VolumeParcent;//個別音量を辞書登録
    }

    // セットした後の音声には音量が反映されるが
    // すでに再生中の音量は変わらないので↓UpdateVolumeとセットで実行
}

// メモリ音源の音量の更新(再生中に減らせばフェードアウト)
int Sound::UpdateVolume(int SoundHandle)
{
    // 特に指定がなければすべての音量を更新
    if (SoundHandle == -1)
    {    // 辞書のfor文はキーとバリューがペアになってる
        int result = 0, resultAll = 0;
        for (const auto& kv : loadDic)
        {    // 辞書のキーがkv.first,バリューがkv.second
            int handle = kv.second;
            // 【★勉強】自分自身を呼出す奥の手
            if (handle != -1) //↓【再起呼出し】
                result = Sound::UpdateVolume(handle);
            if (result == -1)
                resultAll += -1; // 失敗した数ぶん-1がカウントされる
        }
        return resultAll; // 全音量更新完了【再起呼出しループで】
    }

    // 音源指定があるときはhandleを指定して音量を更新
    int handle = SoundHandle;

    int volume = 255;
    float volumeParcent = 100;
    std::string file = "";
    if (SE_Dic.count(handle) == 1)
    {    // サウンド番号がSE辞書で見つかれば
        file = SE_Dic[handle];
        volumeParcent = volumeSE; //音量をSEのベース音量に
    }
    else if (BGM_Dic.count(handle) == 1)
    {    // サウンド番号がBGM辞書で見つかれば
        file = BGM_Dic[handle];
        volumeParcent = volumeBGM; //音量をSEのベース音量に
    }

    if (volumeDic.count(file) == 1)
    {    // 音量辞書をファイル名で検索して
        float fileParcent = volumeDic[file];
        volumeParcent *= fileParcent / 100; //個別音量%割合反映
    }

    if (volumeParcent < 100) // 音量に%パーセントを掛ける
        volume = (int)(((float)volume) * volumeParcent / 100);
    // 音量反映
    int result = ChangeVolumeSoundMem(volume, handle);

    if (result == 0) // 音量変更成功
        return volume;
    else return result; // 音量変更失敗
}

// 再生中のディスク音楽の音量の更新
int Sound::UpdateMusicVolume()
{
    std::string file = playingMusic;

    int volume = 255;
    float volumeParcent = volumeBGM;
    if (volumeDic.count(file) == 1)
    {    // 音量辞書をファイル名で検索して
        float fileParcent = volumeDic[file];
        volumeParcent *= fileParcent / 100; //個別音量%割合反映
    }

    if (volumeParcent < 100) // 音量に%パーセントを掛ける
        volume = (int)(((float)volume) * volumeParcent / 100);

    // 音量反映
    int result = SetVolumeMusic(volume);

    if (result == 0) // 音量変更成功
        return volume;
    else return result; // 音量変更失敗
}

// 音声をメモリへ読込み番号GET or シーンで使わない場合はメモリから解放
int Sound::LoadSoundMem(const TCHAR* FileName, std::string scene, std::vector<std::string> useScenes, bool isBGM, int BufferNum)
{
    int result = -1;
    // 次のシーンで使う音声かの判別、useScenesリストにあれば使う判定
    bool use = std::find(useScenes.begin(), useScenes.end(), scene) != useScenes.end();
    if (use)
    {    // ロード済み辞書に指定したファイルがある(count=1)のとき
        if (loadDic.count(FileName) == 1 && loadDic[FileName] != -1)
        {    // ロード済みなら音声ハンドルを返す(ロード時間短縮)
            return loadDic[FileName];
        }

        // DXライブラリで音声をロード、BufferNumは同時再生可能数
        result = DxLib::LoadSoundMem(FileName, BufferNum);
        if (result != -1)
        {    // 辞書にサウンドハンドル番号を保管
            int handle = result; //ロード結果からハンドル番号取得
            loadDic[FileName] = handle;//ロード済辞書に登録
            if (isBGM) BGM_Dic[handle] = FileName;//BGMの辞書に登録
            else SE_Dic[handle] = FileName;//SEの辞書に登録
        }

        return result; // ロードした結果を返す
    } // ↓ここまでの↑returnに引っかからずif抜けてきたら【シーン利用しないサウンド】

    // 今回のシーンでは利用しないサウンドの時はメモリからサウンドを開放して節約
    if (loadDic.count(FileName) == 1 && loadDic[FileName] != -1)
    {    // メモリからサウンド開放 https://dxlib.xsrv.jp/function/dxfunc_sound.html#R8N8
        int handle = loadDic[FileName];
        result = DxLib::DeleteSoundMem(handle);
        assert(result != -1 && "音声メモリ開放失敗" != "");
        // 辞書からハンドルを削除
        if (SE_Dic.count(handle) == 1)
            SE_Dic.erase(handle);//SEの辞書から削除
        else if (BGM_Dic.count(handle) == 1)
            BGM_Dic.erase(handle);//BGMの辞書から削除
        loadDic.erase(FileName); // ID辞書からも索引削除
    }
    // 今回のシーンでは利用しないサウンドの時は-2を返す
    return -2; // サウンド読込み失敗の-1と区別するため-2を返します
}

