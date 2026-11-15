#ifndef TITLESCENE_H_
#define TITLESCENE_H_

#include "Scene.h"
#include "Screen.h"
#include "DxLib.h"
#include "Input.h"
#include "Image.h"
#include "Sound.h"

#include "GameManager.h"
#include "SceneManager.h"

class TitleScene : public Scene
{
public:
    // 設定モード
    enum class SettingMode
    {
        BGM = 0, // BGM設定モード
        SE, // SE設定モード

        NUM // 列挙数[enumの数を得る] https://qiita.com/k-satoda/items/2985e81216b3ee500956
    };
    SettingMode settingMode = SettingMode::BGM; // 現在の設定モード
    //★【仲介者パターン】ゲーム内のもの同士はマネージャを通して一元化してやり取り
    SceneManager& sm = SceneManager::GetInstance(); //唯一のシーンマネージャへの参照(&)を得る
    GameManager& gm = GameManager::GetInstance(); //唯一のゲームマネージャへの参照(&)を得る

    bool isStartPushed = false;

    // テスト用のボス画像をタイトル画面で左右移動させる
    int x = 0; //staticじゃないintはここで=0で初期化できる
    int vx = 0;
    // コンストラクタ
    TitleScene() : Scene()
    {
        this->tag = "TitleScene";
    }

    // 初期化処理
    void Initialize() override
    {
        Sound::Load("TitleScene"); //音声の読込みとメモリ開放
        // テスト用のボス画像をタイトル画面で左右移動させる
        Sound::PlayMusic(Sound::title); //タイトルBGMを再生
        isStartPushed = false; // ボタンのフラグをリセット
        x = 100; // Xの初期位置
        vx = 10; // ボスの初期速度
    }

    // 終了処理(大抵Initializeと同じリセットだがInitと区別して終了時だけやりたいリセットもある)
    void Finalize() override
    {

    }
    // ボタン入力に応じた処理
    void HandleInput()
    {
        if (Input::GetButtonDown(PAD_INPUT_1))
        {    // ボタンを押したらスタート音声を再生
            Sound::Play(Sound::start);
            isStartPushed = true; // 画面遷移トリガーON
        }
        if (Input::GetButtonDown(PAD_INPUT_UP))
        {    //上を押すと設定モードを-1する
            int index = ((int)settingMode - 1);
            if (index < 0) //↓番号ループ 0→最大数-1
                index = ((int)SettingMode::NUM) - 1;
            settingMode = (SettingMode)index;
        }
        else if (Input::GetButtonDown(PAD_INPUT_DOWN))
        {    //下を押すと設定モードを+1する
            int index = ((int)settingMode + 1);
            if (index >= ((int)SettingMode::NUM))
                index = 0; //番号ループ
            settingMode = (SettingMode)index;
        }

        if (Input::GetButton(PAD_INPUT_RIGHT))
        {
            if (settingMode == SettingMode::BGM)
            {
                Sound::volumeBGM++;//音量をプラス
                if (Sound::volumeBGM > 100)
                    Sound::volumeBGM = 100;
            }
            else if (settingMode == SettingMode::SE)
            {
                Sound::volumeSE++;//音量をプラス
                if (Sound::volumeSE > 100)
                    Sound::volumeSE = 100;
                Sound::Play(Sound::bomb); //効果音を聞きながら調節
            }
            Sound::UpdateMusicVolume(); //再生中の音量の更新
        }
        else if (Input::GetButton(PAD_INPUT_LEFT))
        {
            if (settingMode == SettingMode::BGM)
            {
                Sound::volumeBGM--;//音量をマイナス
                if (Sound::volumeBGM < 0)
                    Sound::volumeBGM = 0;
            }
            else if (settingMode == SettingMode::SE)
            {
                Sound::volumeSE--;//音量をマイナス
                if (Sound::volumeSE < 0)
                    Sound::volumeSE = 0;
                Sound::Play(Sound::bomb); //効果音を聞きながら調節
            }
            Sound::UpdateMusicVolume(); //再生中の音量の更新
        }
    }
    // 更新処理
    void Update() override
    {
        // ボタン入力に応じた処理
       // スタートの効果音の再生が終わるまで画面遷移を待つ
        if (isStartPushed && CheckSoundMem(Sound::start) == 0)
        {    // スタート音声再生終了をトリガーとして画面遷移
            sm.LoadScene("PlayScene"); //シーン遷移
        }

        HandleInput(); // キー入力処理

        // テスト用のボス画像をタイトル画面で左右移動させる
        if (x < 0)
        {
            vx = 10; //速度の変更
        }
        else if (x > Screen::Width)
        {
            vx = -10; //画面端で移動方向反転
        }
        x += vx; // ボス画像のX位置の更新
    }

    // 描画処理
    void Draw() override
    {
        // テスト用のボス画像をタイトル画面で左右移動させる
        DrawRotaGraphF((float)x, 200, 0.9f, 0, Image::bossImage, TRUE);
        DrawString(0, 0, "TitleSceneです。移動は矢印キー、射撃はZキーです。Zボタン押下でPlaySceneへ。", GetColor(255, 255, 255));
        if (sm.scoreMax >= 0)
        {
            DrawString(0, 200, "MAXスコア: " + sm.scoreMax, GetColor(255, 255, 255));
        }
    }
};

#endif
