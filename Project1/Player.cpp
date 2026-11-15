#include "Player.h"
#include "MyMath.h"
#include "PlayerBullet.h" // ★弾を生成して【使う処理があるのでインクルード必要】



// 更新処理【書き移す際にはPlayer::という形で::所属が必要になります】
void Player::Update()
{
    Vector3 v{ 0,0,0 };

    if (Input::GetButton(PAD_INPUT_LEFT))
    {
        if (position.x + VisibleRadius < 0)
        {
            v.x = MoveSpeed;
        }
        else  {
        v.x = -MoveSpeed; // 左
    }
    }
    else if (Input::GetButton(PAD_INPUT_RIGHT))
    {
        if (position.x - VisibleRadius > Screen::Width)
        {
            v.x = -MoveSpeed;
        }
        else {
            v.x = MoveSpeed; // 右
        }

    }
    if (Input::GetButton(PAD_INPUT_UP))
    {
        v.y = -MoveSpeed; // 下
        if (position.y + VisibleRadius < 0)
        {
            v.y = MoveSpeed;
        }
    }
    else if (Input::GetButton(PAD_INPUT_DOWN))
    {
        v.y = MoveSpeed; // 上
        if (position.y - VisibleRadius > Screen::Height)
        {
            v.y = -MoveSpeed;
        }
    }

    // 斜め移動も同じ速度になるように調整
    if (v.magnitude() > 0)
    {
        v /= MyMath::Sqrt2;
    }

    // 実際に位置を動かす
    position += v;

    // ボタン押下で自機弾を発射
    if (Input::GetButtonDown(PAD_INPUT_1))
    {
        // emplace_backで配列に追加。make_shared↓でプレイヤ弾を新しく生成
        gm.playerBullets.emplace_back(std::make_shared<PlayerBullet>(position,0));
        gm.playerBullets.emplace_back(std::make_shared<PlayerBullet>(position, -15 * MyMath::Deg2Rad));//右上
        gm.playerBullets.emplace_back(std::make_shared<PlayerBullet>(position, +15 * MyMath::Deg2Rad));//右下
       
    }

    mutekiTimer--; // 無敵タイマーは無条件で毎フレームカウントダウン
}

// 描画処理
void Player::Draw()
{
    // 無敵ではない場合は描画
  // 無敵中は2フレームに1回描画（点滅）
    if (mutekiTimer <= 0 || mutekiTimer % 2 == 0)
    {
        DxLib::MV1SetScale(Image::player, Vector3{ 0.25f, 0.25f, 0.25f}); // スケールを設定
        DxLib::MV1SetRotationXYZ(Image::player, VGet(0.0f, 3.1416 * 90 / 180, 0.0f)); // 回転角を指定
        DxLib::MV1SetPosition(Image::player, VGet(position.x, -position.y, position.z)); // 自機を三次元空間に配置
        DxLib::MV1DrawModel(Image::player); // 自機の描画
    }
        //DrawRotaGraphF(position.x, position.y, 1, 0, Image::player, TRUE);
    // ライフをバーで表示する処理
    DrawLifeBar();
}
// ライフをバーで表示する処理
void Player::DrawLifeBar()
{
    // ライフをバー表示する際の色設定
    unsigned int frameColor = GetColor(255, 255, 255); // バーの枠の色
    unsigned int backColor = GetColor(0, 0, 0); // バーの背景の色
    unsigned int lineColor = GetColor(0, 0, 0); // バーの区切り目盛りの色
    unsigned int red = GetColor(255, 0, 0), orange = GetColor(255, 100, 0); // ピンチ時の赤、オレンジ
    unsigned int green = GetColor(0, 255, 0), blue = GetColor(0, 255, 255); // 10までは緑、11以上は水色
    int barMax = (lifeMax > 10) ? 10 : lifeMax; // バーの目盛りの数
    int barWidth = 150, barPart = barWidth / barMax, rectWidth = (barPart + 1) * barMax;
    int barHeight = 10; // バーの表示する高さ(4+1+1) 枠は上+1, 下+1, 目盛ぶん↑+1して×10し直すとバーの四角形幅になる
    Vector3 barPos{ 150.0f - rectWidth / 2, 10.0f + collisionRadius }; // バーの表示位置

    // バーの背景を描画
    DrawBox(barPos.x - 1, barPos.y - 1, barPos.x + 1 + rectWidth, barPos.y + 1 + barHeight, backColor, TRUE);
    // バーの枠を描画
    DrawBox(barPos.x - 1, barPos.y - 1, barPos.x + 1 + rectWidth, barPos.y + 1 + barHeight, frameColor, FALSE);
    auto barColor = (life <= 1) ? red : (life <= 2) ? orange : green; // lifeが 残り1:赤,残り2:橙 10以下:緑
    // バーの中身を10まで描画
    int barNum = (life < 10) ? life : 10;
    for (int i = 0; i < barNum; ++i)
        DrawBox(barPos.x + i * (barPart + 1), barPos.y,
            barPos.x + (i + 1) * (barPart + 1), barPos.y + barHeight, barColor, TRUE);
    // バーの中身(水色)を20まで描画
    int blueNum = (life > 20) ? 10 : (life > 10) ? life - 10 : 0; // lifeが残り10以上のときは水色
    for (int i = 0; i < blueNum; ++i)
        DrawBox(barPos.x + i * (barPart + 1), barPos.y,
            barPos.x + (i + 1) * (barPart + 1), barPos.y + barHeight, blue, TRUE);
    // バーの目盛りを描画
    for (int i = 0; i < 9; ++i)
        DrawLine(barPos.x + (i + 1) * (barPart + 1), barPos.y,
            barPos.x + (i + 1) * (barPart + 1), barPos.y + barHeight, (i + 1 == life) ? barColor : lineColor, TRUE);
}
// 敵にぶつかった時の処理
void Player::OnCollisionEnemy(std::shared_ptr<GameObject> other)
{
    // 無敵じゃなければダメージを受ける
    if (mutekiTimer <= 0)
    {
        TakeDamage();
    }
}

// 敵弾とぶつかった時の処理
void Player::OnCollisionEnemyBullet(std::shared_ptr<GameObject> other)
{
    // 無敵じゃなければダメージを受ける
    if (mutekiTimer <= 0)
    {
        TakeDamage();
    }
}

// ダメージを受ける処理
void Player::TakeDamage()
{
    life -= 1; // ライフ減少

    if (life <= 0)
    {
        // ライフが無くなったら死亡
        isDead = true;
    }
    else
    {
        // 無敵時間発動
        mutekiTimer = MutekiJikan;
    }
}
// ライフが回復する処理
void Player::RecoverLife(int amount)
{
    life += amount; // ライフ回復
}