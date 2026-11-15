#ifndef ITEM_H_
#define ITEM_H_

#include "GameObject.h"

#include "GameManager.h"

// アイテムのベース基底クラス
class Item : public GameObject
{
public:
    //【仲介者パターン】ゲーム内のもの同士はマネージャを通して一元化してやり取り
    GameManager& gm = GameManager::GetInstance(); //唯一のゲームマネージャへの参照(&)を得る

    float collisionRadius; // 当たり判定の半径
    enum class CollisionTo // 当たり判定の対象(ビット32桁に1を立てて表現)
    {
        None = 0,
        Player = 0b0000'0000'0000'0000'0000'0000'0000'0001,
        EnemyBullet = 0b0000'0000'0000'0000'0000'0000'0000'0010,
    };

    CollisionTo collisionTo = CollisionTo::Player; // デフォルトはプレイヤとの当たりを判定する

    // コンストラクタ v:移動速度(方向) radius:当たり判定の半径
    Item(Vector3 pos, Vector3 v, float radius = 8)
        : collisionRadius{ radius }, GameObject(pos) // GameObjectをベースとして初期化処理を継承する
    {
        this->typeTag = "Item"; // オブジェクトの種類判別タグ
        this->v = v; // GameObjectの速度vを利用する
    }

    // 更新処理。派生クラスでオーバーライドして使う
    virtual void Update() = 0;//virtual ～ = 0;で【純粋仮想関数に】これで【C#のabstract型に】

    // 描画処理。派生クラスでオーバーライドして使う
    virtual void Draw() = 0; //virtual ～ = 0;で【純粋仮想関数に】【関数 = 0で未定義状態を表現】

    // 自機に当たったときの処理 派生クラスでオーバーライドして使う
    virtual void OnCollisionPlayer(std::shared_ptr<GameObject> object)
    {
        // アイテムごとに継承でoverrideして個別処理を書く
    }
    // 敵弾とぶつかった時の処理
    virtual void OnCollisionEnemyBullet(std::shared_ptr<GameObject> object)
    {
        // アイテムごとに継承でoverrideして個別処理を書く
    }
};

#endif
