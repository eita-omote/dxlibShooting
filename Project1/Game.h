#ifndef GAME_H_
#define GAME_H_

#include <vector>// 配列std::vectorを使う
#include <memory>//スマートなポインタを使うのに必要(shared_ptrなど)


#include "Image.h"
#include "Screen.h"

#include "GameManager.h"
#include "SceneManager.h"
#include "Scene.h"

class PlayerBullet;

class Game
{
public:
	/*        ↓{ }を.hに書くことでcppに分けて書くはずの処理ぶぶんを.hに書いてもよい */
	Game() {}; // 初期化コンストラクタ
	~Game() {}; // 破棄処理デストラクタ

	SceneManager& sm = SceneManager::GetInstance(); //唯一のシーンマネージャへの参照(&)を得る
	GameManager& gm = GameManager::GetInstance();
	void Init(); // Init処理(定義だけ)
	void Update(); // 更新処理(定義だけ)
	void Draw();// 描画処理(定義だけ)

	int x = 0; //staticじゃないintはここで=0で初期化できる
	int vx = 0;


};
#endif
