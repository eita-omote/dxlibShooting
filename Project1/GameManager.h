#ifndef GAMEMANAGER_H_
#define GAMEMANAGER_H_

#include <memory>
#include <list>

#include "Singleton.h"

class Map;
class Player; // クラス宣言だけで★インクルードしないのでこのマネージャファイルで循環は止まる
class PlayerBullet;
class Enemy;
class EnemyBullet;
class Explosion;
class Item;

class GameManager : public Singleton<GameManager>//←<～>として継承すると唯一のシングルトン型タイプとなる
{
public:
    friend class Singleton<GameManager>; // Singleton でのインスタンス作成は許可

    std::shared_ptr<Map> map{ nullptr }; // マップ
    std::shared_ptr<Player> player{ nullptr }; // 自機の初期化
    std::list<std::shared_ptr<PlayerBullet>> playerBullets; // 自機弾のリスト
    std::list<std::shared_ptr<Enemy>> enemies; // 敵のリスト
    std::list<std::shared_ptr<EnemyBullet>> enemyBullets; // 敵弾のリスト
    std::list<std::shared_ptr<Explosion>> explosions; // 爆発エフェクトのリスト
    std::list<std::shared_ptr<Item>> items; // アイテムのリスト

    // 複数リスト↑vector ↑【shared_ptr】回し読みポインタ:メモリにデータを回し読み状態として確保できる

    template <typename T, class T_if>
    void EraseRemoveIf(std::list<T>& v, T_if if_condition)
    {   //            特定のタイプT↑  ↑配列v   ↑条件式if_condition
        v.erase(
            std::remove_if(v.begin(), v.end(), if_condition),
            v.end() //  ↓remove_ifの位置
        ); //例.[生][生][死][死][死]← v.end()の位置
    };

protected:
    GameManager() {}; // 外部からのインスタンス作成は禁止
    virtual ~GameManager() {}; //外部からのインスタンス破棄も禁止
};

#endif