#ifndef IMAGE_H_
#define IMAGE_H_

#include "DxLib.h"
#include <assert.h> // 画像読み込みの読込み失敗表示用
#include <unordered_map> // ロード済の画像辞書に使う
#include <string> // 文字列string
#include <memory> // 辞書の指す先を回し読みポインタにする
#include <vector> // シーンのリストに使う
#include <algorithm> // シーンのリストからのfindに使う
#include "DivImage.h" // 分割画像の読込みに使う

class Image
{
public:
	Image() {}; // 初期化コンストラクタ:定義と{}空の処理
	~Image() {}; // 破棄する処理デストラクタ:定義と{}空の処理
	static void Load(); // 画像ロード
	static int LoadDivGraph(const TCHAR* FileName, DivImage& divImage);

	static int bossImage; //ボス画像のハンドラ(読込画像番号)
	static int player; //プレイヤ画像のハンドラ
	static int playerBullet;
	static int enemyBullet16; //敵弾 画像のハンドラ(読込画像番号)
	static int zako0; //ザコ0 画像のハンドラ(読込画像番号)
	static int zako1; //ザコ1 画像のハンドラ(読込画像番号)
	static int zako2; //ザコ2 画像のハンドラ(読込画像番号)
	static int zako3; //ザコ3 画像のハンドラ(読込画像番号)
	static int boss1; //ボス通常 画像のハンドラ(読込画像番号)
	static int boss2; //ボス2気絶 画像のハンドラ(読込画像番号)
	static int boss3; //ボス3発狂 画像のハンドラ(読込画像番号)
	static DivImage explosion; // [分割画像]爆発エフェクト
	static int item0; //アイテム0 画像のハンドラ(読込画像番号)
	static int item1; //アイテム1 画像のハンドラ(読込画像番号)

private:

};
#endif