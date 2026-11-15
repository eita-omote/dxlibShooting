#ifndef ZAKO_3_H_
#define ZAKO_3_H_

#include "Enemy.h"
#include "Image.h"
#include "Screen.h"
#include <cmath> // Sin Cosの計算に使う
#include "MyMath.h"
#include "Vector3.h"
#include "MyRandom.h"
#include "EnemyBullet.h" // make_sharedなど【実際の弾を撃ち生成処理を行うのでインクルード必要】

// ザコ3クラス(自爆型機雷タイプの敵キャラ)
class Zako3 : public Enemy
{
public:
    int bombCounter = -1; // やられたときのisDeadまでのカウントダウン
    int BombTime = 300; // 爆発までのカウント時間
    int life = 10; // 敵ライフ

    // コンストラクタ
    Zako3(Vector3 pos) : Enemy(pos)
    {
        this->tag = "Zako3";
    }

    // 更新処理
    void Update() override
    {
        if (position.x > Screen::Width - 350)
        { // スクリーンの端から350の位置で止まる
            position.x -= 1; // とりあえず左へ移動
        }
        else
        { // スクリーン端から350の位置で自爆モードに入る
            life = 0; // 画面中央付近で勝手にlife=0で自爆モードに入る
        }

        bombCounter--; // 爆発カウンタは常にカウントダウン(-1から始まるときは0を通過しないので爆発しない)

        if (life <= 0 && bombCounter < 0)
            bombCounter = BombTime; // 爆発モードに入る(bombCounterがプラス値になり0になると爆発)

        if (bombCounter == 0) // 初期状態は-1からカウントダウン-1,-2,-3..するからここには入らずに済む
        {
            for (int i = 0; i < 360; i = i + 10) //【勉強】for文は1ずつプラスじゃなくてもよい
            {
                gm.enemyBullets.emplace_back(std::make_shared<EnemyBullet>(position, (180 + i) * MyMath::Deg2Rad, 6));
            }
            isDead = true; // 爆発カウンタ==0で初めてisDead状態に
        }
    }

    // 【勉強】共通処理を上書きオーバーライドで上塗りして共通処理を無効化・乗っ取るテクニック
    // 自機弾との衝突処理を★【上書きオーバーライド】して死亡フラグが立たないように
    virtual void OnCollisionPlayerBullet(std::shared_ptr<GameObject> playerBullet) override
    {
        life -= 1; // ライフを減らす

        // 死亡フラグはこのザコ3だけ特別に衝突ではONにしない
    }

    // 描画処理
    void Draw() override
    {
        //【ド・モルガンの定理を学ぼう】||と&&の逆の関係性の法則
        //【勉強】条件【または||】は条件【かつ&&】の(life <= 0 && bombCounter > 0)のちょうど逆条件！
        if (life > 0 || bombCounter <= 0
            || (bombCounter % 8 < 4)) //[点滅]パラパラ8枚の周期で余り4以下の時だけ描く
            DrawRotaGraphF(position.x, position.y, 1, 0, Image::zako3, TRUE);
    }
};

#endif