#ifndef GAMEOVERSCENE_H_
#define GAMEOVERSCENE_H_

#include "Scene.h"

#include "DxLib.h"
#include "Input.h"
#include "Image.h"

#include "GameManager.h"
#include "SceneManager.h"

class GameOverScene : public Scene
{
public:
    //★【仲介者パターン】ゲーム内のもの同士はマネージャを通して一元化してやり取り
    SceneManager& sm = SceneManager::GetInstance(); //唯一のシーンマネージャへの参照(&)を得る
    GameManager& gm = GameManager::GetInstance(); //唯一のゲームマネージャへの参照(&)を得る

    // コンストラクタ
    GameOverScene() : Scene()
    {
        this->tag = "GameOverScene";
    }

    // 初期化処理
    void Initialize() override
    {
    }

    // 終了処理(大抵Initializeと同じリセットだがInitと区別して終了時だけやりたいリセットもある)
    void Finalize() override
    {

    }

    // 更新処理
    void Update() override
    {
        if (Input::GetButtonDown(PAD_INPUT_1))
        {
            sm.LoadScene("TitleScene"); //シーン遷移
            return; // シーンをロードしたらUpdateを即終了しないとUpdateの他の処理が走っちゃう
        }
    }

    // 描画処理
    void Draw() override
    {
        DrawString(0, 0, "GameOver....ボタン押下でPlaySceneへ", GetColor(255, 255, 255));
    }
};

#endif