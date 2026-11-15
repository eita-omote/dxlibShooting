#include "Image.h"


int Image::bossImage{ -1 }; // Load終わっても-1(初期値)のままだと画像ロードが失敗してますね
int Image::player{ -1 };
int Image::playerBullet{ -1 };
int Image::enemyBullet16{ -1 };
int Image::zako0{ -1 };
int Image::zako1{ -1 };
int Image::zako2{ -1 };
int Image::zako3{ -1 };
int Image::boss1{ -1 };
int Image::boss2{ -1 };
int Image::boss3{ -1 };
int Image::item1{ -1 };
// ★分割画像は初期化のときに{X方向画像数, Y方向画像数, 画像横幅XSize,画像縦幅YSize}を指定
DivImage Image::explosion{ 8, 2, 64, 64 };
int Image::item0{ -1 };
void Image::Load()
{
	
    //player = LoadGraph("Image/Player.png", scene, { "TitleScene", "PlayScene" });
	player = MV1LoadModel("Image/fighter.mqoz");
    assert(player != -1); // 画像読込失敗、ファイル名かフォルダ名が間違ってる

	playerBullet = LoadGraph("Image/player_bullet 1.png");
	assert(playerBullet != -1); // 画像読込失敗、ファイル名かフォルダ名が間違ってる
	enemyBullet16 = LoadGraph("Image/enemy_bullet_16.png");
	assert(enemyBullet16 != -1); // 画像読込失敗、ファイル名かフォルダ名が間違ってる

	//zako0 = LoadGraph("Image/zako0.png");
	zako0 = MV1LoadModel("Image/fighter2.mv1");
	assert(zako0 != -1); // 画像読込失敗、ファイル名かフォルダ名が間違ってる

	zako1 = MV1LoadModel("Image/zako1.mv1");
	assert(zako1 != -1); // 画像読込失敗、ファイル名かフォルダ名が間違ってる

	zako2 = MV1LoadModel("Image/fighter3.mv1");
	assert(zako2 != -1); // 画像読込失敗、ファイル名かフォルダ名が間違ってる

	zako3 = LoadGraph("Image/zako3.png");
	assert(zako3 != -1); // 画像読込失敗、ファイル名かフォルダ名が間違ってる

	boss1 = LoadGraph("Image/spaceShips_005.png");
	assert(boss1 != -1); // 画像読込失敗、ファイル名かフォルダ名が間違ってる

	boss2 = LoadGraph("Image/spaceShips_005.png");
	assert(boss2 != -1); // 画像読込失敗、ファイル名かフォルダ名が間違ってる

	boss3 = LoadGraph("Image/spaceShips_005.png");
	assert(boss3 != -1); // 画像読込失敗、ファイル名かフォルダ名が間違ってる

	// div分割画像をロード
	if (Image::LoadDivGraph("Image/explosion.png", explosion) != -1)
		for (int i = 0; i < explosion.AllNum; i++)
		{ // 画像読込失敗
		if (explosion.HandleArray[i] == -1) assert("爆発分割画像読込失敗" == "");
		}
	item0 = LoadGraph("Image/heart.png");
	assert(item0 != -1); // 画像読込失敗、ファイル名かフォルダ名が間違ってる

	item1 = LoadGraph("Image/star.png");
	assert(item1 != -1); // 画像読込失敗、ファイル名かフォルダ名が間違ってる
}

// DXライブラリのLoadDivGraphを使いやすくラッピング
// ★関数に参照渡しを行う
// 関数の引数に参照渡しを行うことでdivImageの中身を書き換えることができる(値渡しでは書換不可)
//　https://qiita.com/agate-pris/items/05948b7d33f3e88b8967
// https://qiita.com/RuthTaro/items/f35c3a26779c0ca1a41a
int Image::LoadDivGraph(const TCHAR* FileName, DivImage& divImage)
{
	return DxLib::LoadDivGraph(FileName, divImage.XNum * divImage.YNum,
		divImage.XNum, divImage.YNum, divImage.XSize, divImage.YSize, divImage.HandleArray);
};