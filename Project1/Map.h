#ifndef MAP_H_
#define MAP_H_

#include <assert.h> // 読込み失敗表示用
#include <memory> // 回し読みポインタをザコやボスの生成に使う
#include <vector> // csvを2次元vector配列に格納
#include <string> // 文字列に必要
#include <fstream> // ファイル読み出しifstreamに必要
#include <sstream> // 文字列ストリームに必要

#include "DxLib.h"
#include "Screen.h"
#include "GameManager.h"
#include "Zako0.h"
#include "Zako1.h"
#include "Zako2.h"
#include "Boss.h"
#include "MyRandom.h"

class Map
{
public:
    int CellSize = 32; // マップの1マスのピクセル数
    int Width = 0; // マップデータの横のマス数
    int Height = 0; // マップデータの縦のマス数

    //★【仲介者パターン】ゲーム内のもの同士はマネージャを通して一元化してやり取り
    GameManager& gm = GameManager::GetInstance(); //唯一のゲームマネージャへの参照(&)を得る

    std::vector<std::vector<int>> enemyData;// 敵配置データ

    float position = 0; // マップ読出し位置のスクロール量

    // コンストラクタ
    // filePath : CSVファイルのパス
    // startPosition : 開始位置（デバッグやリスタート時に、面の途中から開始できるように）
    Map(std::string filePath, float startPosition = 0)
    {
        position = startPosition;
        Load(filePath); // csvファイルをロード
    }

    //デストラクタ
    ~Map()
    {
        enemyData.clear();

    }

    // csvファイルの読み込み
    void Load(std::string filePath)
    {
        // 読み込むcsvファイルを開く(std::ifstreamのコンストラクタで開く)
        std::ifstream ifs_csv_file(filePath);

        std::string line; //1行単位でcsvファイルから文字列を読み込む

        Width = 0; //初期化
        int MaxWidth = 0; // 1行の数字の最大個数
        Height = 0; //初期化
        while (std::getline(ifs_csv_file, line)) // ファイルを行ごとに読み込む
        {
            std::vector<int> valuelist; // 1行の数字リスト
            std::istringstream linestream(line); // 各行の文字列ストリーム
            std::string splitted; // カンマで分割された文字列
            int widthCount = 0; //この行の幅をカウント
            while (std::getline(linestream, splitted, { ',' }))
            {
                std::istringstream ss; //文字列ストリームの初期化
                ss = std::istringstream(splitted); //文字列ストリーム
                int num; // 数字単体
                ss >> num; // 文字列ストリーム>>で数字へ変換
                valuelist.emplace_back(num); // 数字をこの行のリスト(valuelist)に追加
                widthCount++; //この行のカンマで区切られた数字の数をカウントアップ
            }
            // 1行の数字の数が記録を更新してMAXになるかチェック
            if (widthCount > MaxWidth) MaxWidth = widthCount; //暫定Max幅を更新

            // 数字1行分をvectorに追加
            if (valuelist.size() != 0) enemyData.emplace_back(valuelist);
            Height++; //マップの高さをカウントアップ
        }
        Width = MaxWidth; //マップ幅は一番数字の個数の多かった行に合わせる
        //↓読込んだCSVの幅と高さをチェック
        assert(Width > 0 && "マップ読込み失敗ファイル名間違いでは?" != "");
        assert(Height > 0 && "マップ読込み失敗ファイル名間違いでは?" != "");
    }

    // 画面スクロール（位置の更新）
    void Scroll(float delta)
    {
        position += delta; // スクロールする

        if (((int)position) % 50 == 0)
        {

            int id = MyRandom::Range(0, 2); // ザコを抽選

            float randomY = MyRandom::Range(0.0f, 32.0f * 16.0f);

            if(id == 0) gm.enemies.emplace_back(std::make_shared<Zako0>(Vector3(CellSize * 30, randomY)));
            else if (id == 1) gm.enemies.emplace_back(std::make_shared<Zako1>(Vector3(CellSize * 30, randomY)));
            else if (id == 2) gm.enemies.emplace_back(std::make_shared<Zako2>(Vector3(CellSize * 30, randomY)));
        }
#if 0
        // スクロールする前の、画面右端のセルの列番号（x）
        int prevRightCell = (int)(position + Screen::Width) / CellSize;

        position += delta; // スクロールする

        // スクロールした後の、画面右端のセルの列番号（x）
        int currentRightCell = (int)(position + Screen::Width) / CellSize;

        // スクロールした後の画面右端のセルがマップの範囲外に出ちゃったら何もしないで終了
        if (currentRightCell >= Width)
            return;

        // スクロールする前と後で、画面右端のセルが同じ場合は、何もしないで終了
        if (prevRightCell == currentRightCell)
            return;

        // 画面右端のセルの左端のx座標
        float x = currentRightCell * CellSize - position;

        // 上端のセルから下端のセルまで舐めて、敵データが配置されていたら、敵を生成する
        for (int cellY = 0; cellY < Height; cellY++)
        {
            float y = cellY * CellSize;

            //【★注意!】2次元vectorではxとyがC#と逆！[行Y][列X]⇒行列の順と覚えよう
            int id = enemyData[cellY][currentRightCell];

            // 番号に応じて敵を生成する
            if (id == -1) continue; // -1は空白なので、何もしない
            else if (id == 0) gm.enemies.emplace_back(std::make_shared<Zako0>(Vector3(x + 32, y + 32)));
            else if (id == 1) gm.enemies.emplace_back(std::make_shared<Zako1>(Vector3(x + 32, y + 32)));
            else if (id == 2) gm.enemies.emplace_back(std::make_shared<Zako2>(Vector3(x + 32, y + 32)));
            else if (id == 100) gm.enemies.emplace_back(std::make_shared<Boss>(Vector3(x + 90, y + 32))); // ボスは横幅180pxだから90px右に生成
            else assert("まだ追加してない敵IDがマップCSVにありました。" == "");
        }
#endif
    }
};
#endif