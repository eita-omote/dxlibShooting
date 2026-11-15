#include "Game.h"
#include "MyRandom.h" 
#include "Input.h"

void Game::Init()
{// Init処理
    MyRandom::Init();
    Image::Load(); //画像の読込みとメモリ開放
    // ゲーム開始して最初はタイトルシーンに遷移
    sm.LoadScene("TitleScene");

   
};

void Game::Update()
{// 更新処理

    Input::Update(); //【注意】これ忘れるとキー入力押してもキャラ動かない 
    // シーンの更新処理
        sm.currentScene->Update();
};

void Game::Draw()
{// 描画処理
    // シーンの描画処理
    sm.currentScene->Draw();
};