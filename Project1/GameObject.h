#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_

#include "DxLib.h"
#include "Vector3.h"
#include <string> //文字タグのため
#include <memory> //回し読みポインタの定義のため

// ゲーム上に表示される物体の基底クラス。
// プレイヤーや敵、アイテムなどはこのクラスを継承して作る。
class GameObject
{
public:
	Vector3 position{ 0,0,0 }; // xyz座標
	Vector3 v{ 0,0,0 }; //xyz方向の速度

	bool isDead = false; // 死んだ（削除対象）フラグ

	std::string tag = ""; // Zako0やBossやPlayerなど小カテゴリ
	std::string typeTag = ""; // EnemyやPlayerなど大カテゴリ
	std::string statusTag = ""; // 爆発状態やアイテムゲット状態など自由に使えばよい

	// コンストラクタ
	GameObject(Vector3 pos) : position{ pos }
	{

	}
	//★仮想デストラクタ【忘れるとメモリがヤバいダメ絶対】
	//【注意！】ベースの基底では必ず定義しないとstringなどが浪費され極悪なメモリ被害に発展
	virtual ~GameObject()
	{

	}

	//★純粋仮想関数=0は継承したZakoやPlayerなどの必須機能(継承したら絶対override必須縛り)
	// 更新処理
	virtual void Update() = 0; //純粋仮想関数に(継承したらoverride必須)

	// 描画処理
	virtual void Draw() = 0; //純粋仮想関数に(継承したらoverride必須)

	//★virtual仮想関数は共通カスタマイズ機能(継承してもoverride必須ではないご自由にカスタマイズ)
	// 衝突したときの関数(仮のvirtual関数←純粋仮想関数と違い継承してもoverride必須ではない)
	virtual void OnCollision(std::shared_ptr<GameObject> other)
	{

	}

};

#endif