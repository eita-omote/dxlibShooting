#ifndef PLAYSCENE_H_
#define PLAYSCENE_H_

#include "Scene.h"

#include "DxLib.h"
#include "Screen.h"
#include "Input.h"
#include "Image.h"
#include "Sound.h"
#include "Map.h"

#include "GameManager.h"
#include "SceneManager.h"

#include "Player.h"
#include "Boss.h"
#include "PlayerBullet.h"
#include "EnemyBullet.h"
#include "Explosion.h"
#include "Item.h"

class PlayScene : public Scene
{
public:
    //★【仲介者パターン】ゲーム内のもの同士はマネージャを通して一元化してやり取り
    SceneManager& sm = SceneManager::GetInstance(); //唯一のシーンマネージャへの参照(&)を得る
    GameManager& gm = GameManager::GetInstance(); //唯一のゲームマネージャへの参照(&)を得る

    int score = 0; // プレイ中のゲームのスコア:短期的数値でシーン遷移ごとにリセットされる

    float scrollSpeed = 1.0f;//1.5f; // スクロール速度

    // コンストラクタ
    PlayScene() : Scene()
    {
        this->tag = "PlayScene";
    }

    // 初期化処理
    void Initialize() override
    {
        Sound::Load("PlayScene"); //音声の読込みとメモリ開放
        // ここでプレイヤの初期化などのプレイ開始時の処理

        Sound::PlayMusic(Sound::gameplay); //プレイ画面BGMを再生

        // マップの初期化
        gm.map = std::make_shared<Map>("Map/stage1.csv");

        gm.player = std::make_shared<Player>(Vector3((float)100, (float)(Screen::Height / 2))); // 自機の初期化
     
    }
        
    

    // 終了処理(大抵Initializeと同じリセットだがInitと区別して終了時だけやりたいリセットもある)
    void Finalize() override
    {
        // ここにプレイ終了時のメモリのリセット処理(ゲームクリアやゲームオーバーなど)
        gm.map = nullptr; // マップのメモリからの解放
        gm.player = nullptr; // プレイヤのメモリからの解放
        gm.enemies.clear(); // 敵のメモリからの解放
        gm.playerBullets.clear(); // プレイヤの弾のメモリからの解放
        gm.enemyBullets.clear(); // 敵弾のメモリからの解放
        gm.explosions.clear(); // 爆発エフェクトのメモリからの解放
        gm.items.clear(); // アイテムのメモリからの解放
        // ゲーム終了時のスコアがMAXなら記録して終了
        if (score > sm.scoreMax) sm.scoreMax = score;
    }

    // 更新処理
    void Update() override
    {
        // カメラを正射影に変更
        SetupCamera_Ortho(Screen::Width/2);
        SetCameraPositionAndTarget_UpVecY(Vector3{Screen::Width/2,-Screen::Height/2,-1000}, Vector3{ Screen::Width / 2,-Screen::Height / 2,0 });
        // ここにプレイ中の更新処理を持ってくる
        // マップデータの読出し位置のスクロール
        gm.map->Scroll(scrollSpeed);

        if (!gm.player->isDead) // 自機が死んでいなければ
            gm.player->Update(); // プレイヤの更新【忘れるとプレイヤが動かない】

        // 自機弾の更新処理
        // for文で全自機弾をループで回してUpdateで更新する
        for (const auto& b : gm.playerBullets)
        {  // autoキーワードはビルドのコンパイル機械語翻訳の時に型を自動推論してくれる
            b->Update();
        }

        // 敵弾の更新処理
        for (const auto& b : gm.enemyBullets)
        {
            b->Update();
        }

        // 自機と敵弾の衝突判定
        for (const auto& enemyBullet : gm.enemyBullets)
        {
            // 自機が死んでたらこれ以上判定しない
            if (gm.player->isDead)
                break;

            // 敵弾が死んでたらスキップ
            if (enemyBullet->isDead)
                continue;

            // 円同士の衝突判定で調べる
            if (MyMath::CircleCircleIntersection(
                gm.player->position, gm.player->collisionRadius,
                enemyBullet->position, enemyBullet->collisionRadius))
            {
                gm.player->OnCollisionEnemyBullet(enemyBullet);
            }
        }

        // 敵の更新処理
        for (const auto& b : gm.enemies)
        {
            b->Update();
        }

        // 自機弾と敵の衝突判定
        for (const auto& playerBullet : gm.playerBullets)
        {
            // 自機弾が死んでたらスキップする
            if (playerBullet->isDead)
                continue;

            for (const auto& enemy : gm.enemies)
            {
                // 敵が死んでたらスキップする
                if (enemy->isDead)
                    continue;

                // 自機弾と敵が重なっているか？
                if (MyMath::CircleCircleIntersection(
                    playerBullet->position, playerBullet->collisionRadius,
                    enemy->position, enemy->collisionRadius))
                {
                    // 重なっていたら、それぞれのぶつかったときの処理を呼び出す
                    enemy->OnCollisionPlayerBullet(playerBullet);
                    playerBullet->OnCollisionEnemy(enemy);
                    Sound::Play(Sound::bomb); // 効果音の再生
                    // 衝突の結果、自機弾が死んだら、この弾のループはおしまい
                    if (playerBullet->isDead)
                        break;
                }
            }
        }

        // 自機と敵の衝突判定
        for (const auto& enemy : gm.enemies)
        {
            // 自機が死んでたらこれ以上判定しない
            if (gm.player->isDead)
                break;

            // 敵が死んでたらスキップ
            if (enemy->isDead)
                continue;

            // 円同士の衝突判定で調べる
            if (MyMath::CircleCircleIntersection(
                gm.player->position, gm.player->collisionRadius,
                enemy->position, enemy->collisionRadius))
            {
                gm.player->OnCollisionEnemy(enemy);
            }
        }

        // 爆発エフェクトの更新処理
        for (const auto& e : gm.explosions)
        {
            e->Update();
        }

        // アイテムの更新処理
        for (const auto& itm : gm.items)
        {
            itm->Update();
        }

        // 自機とアイテム、敵弾とアイテムの衝突判定
        for (const auto& itm : gm.items)
        {
            // 自機が死んでたらこれ以上判定しない
            if (gm.player->isDead)
                break;

            // アイテムが死んでたらスキップ
            if (itm->isDead)
                continue;

            // 円同士の衝突判定で調べる
            if (MyMath::CircleCircleIntersection(
                gm.player->position, gm.player->collisionRadius,
                itm->position, itm->collisionRadius))
            {
                itm->OnCollisionPlayer(gm.player);
            }

            for (const auto& enemyBullet : gm.enemyBullets)
            {
                // 敵弾が死んでたらスキップ
                if (enemyBullet->isDead)
                    continue;

                // 円同士の衝突判定で調べる
                if (MyMath::CircleCircleIntersection(
                    itm->position, itm->collisionRadius,
                    enemyBullet->position, enemyBullet->collisionRadius))
                {
                    enemyBullet->OnCollisionItem(itm);
                    itm->OnCollisionEnemyBullet(enemyBullet);
                }
            }
        }

        //return; //【★実験】ここでreturnで処理を終わると弾のメモリを解放しない！下のerase前でそこまで処理が行かないからメモリ解放せずに終わる実験

        // 自機弾の削除処理
        gm.EraseRemoveIf(gm.playerBullets,
            [](std::shared_ptr<PlayerBullet>& ptr) { return ptr->isDead; });//isDead==trueの条件のものを削除

        // 敵のリストから死んでるものを除去する
        gm.EraseRemoveIf(gm.enemies,
            [](std::shared_ptr<Enemy>& ptr) { return ptr->isDead; });//isDead==trueの条件のものを削除

        // 敵弾のリストから死んでるものを除去する
        gm.EraseRemoveIf(gm.enemyBullets,
            [](std::shared_ptr<EnemyBullet>& ptr) { return ptr->isDead; });//isDead==trueの条件のものを削除

        //爆発エフェクトのリストから死んでるものを除去する
        gm.EraseRemoveIf(gm.explosions,
            [](std::shared_ptr<Explosion>& ptr) { return ptr->isDead; });//isDead==trueの条件のものを削除

        //アイテムのリストから死んでるものを除去する
        gm.EraseRemoveIf(gm.items,
            [](std::shared_ptr<Item>& ptr) { return ptr->isDead; });//isDead==trueの条件のものを削除

        // ゲームオーバー判定と画面チェンジ処理
        bool isGameover = (gm.player == nullptr);
        if (!isGameover && gm.player->isDead) isGameover = true;
        if (isGameover)
        {
            Sound::PlayMusic(Sound::ending);
            sm.LoadScene("GameOverScene"); //シーン遷移
            return; // シーンをロードしたらUpdateを即終了しないとUpdateの他の処理が走っちゃう
        }
    }

    // 描画処理
    void Draw() override
    {
        // ここにプレイ画面の描画処理を持ってくる
        // 敵の描画処理
        for (const auto& b : gm.enemies)
        {
            b->Draw();
        }

        // 爆発エフェクトの描画処理
        for (const auto& e : gm.explosions)
        {
            e->Draw();
        }

        if (!gm.player->isDead) // 自機が死んでいなければ
            gm.player->Draw(); // プレイヤの更新【忘れるとプレイヤ表示されない】

        // 自機弾の描画処理
        // for文で全自機弾をループで回してUpdateで更新する
        for (const auto& b : gm.playerBullets)
        {  // autoキーワードはビルドのコンパイル機械語翻訳の時に型を自動推論してくれる
            b->Draw();
        }

        // 敵弾の描画処理
        for (const auto& b : gm.enemyBullets)
        {
            b->Draw();
        }

        // アイテムの描画処理
        for (const auto& itm : gm.items)
        {
            itm->Draw();
        }

    }
};

#endif