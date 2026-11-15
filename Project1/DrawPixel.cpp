#include "DxLib.h"
#include <assert.h> 
#include "Screen.h"
#include "Game.h" 


// 設定画面の【リンカー】→【システム】の【サブシステム】をWindows(SUBSYSTEM:WINDOWS)に設定するとWinMainからプログラムが開始する
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // 画面モードの設定
    SetGraphMode(Screen::Width, Screen::Height, 32); // 画面サイズ960×540のカラービット数32ビットで起動
    SetWindowSize(Screen::Width, Screen::Height);// ウィンドウサイズ960×540(こことSetGraphModeのサイズが異なると画像がゆがむ)
    ChangeWindowMode(TRUE);//フルスクリーン表示かウィンドウ表示か
    SetMouseDispFlag(TRUE);// ここをFALSEにするとマウスカーソル非表示
    SetMainWindowText("ゲームのウィンドウ名を変えるときはここ");//この行でエラーになったら【設定】マルチバイト文字セットが間違ってるかも
    //↑ここまでの設定は↓下のDXライブラリ初期化より先にやらないとDxLib_Init()中は画面がフルスクリーンになって終わってからウィンドウサイズが変更になり見苦しい

    // ＤＸライブラリの初期化
    if (DxLib_Init() < 0)
    {
        // DXの初期化にエラーが発生したらプログラム自体を終了(returnで)
        return -1;
    }


    SetDrawScreen(DX_SCREEN_BACK);

    // Init初期化処理
    Game game; //Gameの定義(定義と同時に初期化コンストラクタが実行される)
    game.Init(); // gameのInit準備

    ScreenFlip();

    // アニメーション(パラパラ漫画)するにはWhile文
    while (ProcessMessage() == 0)
    {// ProcessMessage() == 0になるのは×ボタン押したときなど
        ClearDrawScreen();// 一旦キャンバスをきれいにまっさらに

        // Update更新処理
        game.Update(); // gameの更新処理

        // Draw描画処理
        game.Draw();


        ScreenFlip(); //隠れて裏側で描いておいた画像を表面に入れ替え
    }


    // キー入力待ちをする
    WaitKey();
    // ＤＸライブラリの後始末
    DxLib_End();
    // ソフトの終了
    return 0;
}