#ifndef ENEMY_H_
#define ENEMY_H_

#include "GameObject.h"

#include "GameManager.h"

#include "Explosion.h"

// 敵の基底クラス。
// 全ての敵は、このクラスを継承して作る。
// ★【勉強】abstract型はC++にない!!【純粋仮想関数】を使って作る
class Enemy : public GameObject
{
public:

    //【仲介者パターン】ゲーム内のもの同士はマネージャを通して一元化してやり取り
    GameManager& gm = GameManager::GetInstance(); //唯一のゲームマネージャへの参照(&)を得る
    float collisionRadius = 32; // 当たり判定の半径
    float life = 1; // 耐久力

    // コンストラクタ
    Enemy(Vector3 pos) : GameObject(pos) // GameObjectをベースとして初期化処理を継承する
    {
        this->typeTag = "Enemy";
    }

    // 更新処理。派生クラスでオーバーライドして使う
    virtual void Update() = 0;//virtual ～ = 0;で【純粋仮想関数に】これで【C#のabstract型に】

    // 描画処理。派生クラスでオーバーライドして使う
    virtual void Draw() = 0; //virtual ～ = 0;で【純粋仮想関数に】【関数 = 0で未定義状態を表現】

    //【勉強】関数 = 0って何?と思った人は【関数もメモリ上では住所アドレスに過ぎない】ことを知ろう
    // ★関数 = 0はつまり関数の住所ポインタの針が【メモリ上の何かを針で参照してない(住所不定無処理)】
    // つまり【関数が未定義ってこと】＝【あいまい部分があるabstractクラスってこと】
    //http://etc2myday.jugem.jp/?eid=204
    //http://wisdom.sakura.ne.jp/programming/c/c54.html
    //逆にいうと【関数を配列】にするっていうトリッキーなこともできる!
    //http://www.ced.is.utsunomiya-u.ac.jp/lecture/2012/prog/p3/kadai3/virtualfunc2.php

    // 自機弾に当たったときの処理
    virtual void OnCollisionPlayerBullet(std::shared_ptr<GameObject> playerBullet)
    {
        life -= 1; // ライフを減らす

        // ライフが無くなったら、死亡
        if (life <= 0)
        {
            isDead = true;
            // 爆発を出す
            gm.explosions.emplace_back(std::make_shared<Explosion>(Vector3(position.x, position.y)));
        }
        DropItem(); // アイテムを落とす処理(落とすかどうかはoverrideされた関数次第)
    }
    // アイテムを落とす処理 継承overrideして各ザコごとに違うドロップを実装できる
    virtual void DropItem()
    {
        // 継承overrideして各ザコごとに違う処理を実装する
    }
};

#endif
