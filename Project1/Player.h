#ifndef PLAYER_H_
#define PLAYER_H_

#include "DxLib.h"
#include "Input.h"
#include "MyMath.h"
#include "Image.h"

#include "GameObject.h"

#include "GameManager.h"
class Player : public GameObject
{
public:

    const int VisibleRadius=16;
    const float MoveSpeed = 6; // 移動速度   
    int MutekiJikan = 120; // 無敵時間

    float collisionRadius = 32; // 当たり判定半径

    int lifeMax = 20; // 最大ライフ
    int life = 3; // ライフ
    int mutekiTimer = 0; // 残り無敵時間。0以下なら無敵じゃないってこと


    GameManager& gm = GameManager::GetInstance(); //唯一のゲームマネージャへの参照(&)を得る

    // コンストラクタ
    // pos : 初期位置
    Player(Vector3 pos) : GameObject{ pos }
    {
      
    }
    virtual ~Player() {};

    // 更新処理
    void Update();

    // 描画処理
    void Draw();
    // ライフをバーで表示する処理
    void DrawLifeBar();

    // 敵にぶつかった時の処理
    void OnCollisionEnemy(std::shared_ptr<GameObject> other);
    // 敵弾とぶつかった時の処理
    void OnCollisionEnemyBullet(std::shared_ptr<GameObject> other);


    // ダメージを受ける処理
    void TakeDamage();

    // ライフが回復する処理
    void RecoverLife(int amount);
};

#endif

