#ifndef SCNENEMANAGER_H_
#define SCNENEMANAGER_H_

#include <vector>
#include <memory>
#include <string>
#include <assert.h> // シーン読み込みの失敗表示用

#include "Singleton.h"

class Scene; //クラス宣言だけで★インクルードしないのでこのマネージャファイルで循環は止まる

class SceneManager : public Singleton<SceneManager>//←<～>として継承すると唯一のシングルトン型タイプとなる
{
public:
    friend class Singleton<SceneManager>; // Singleton でのインスタンス作成は許可


    // マネージャを【どこからでもアクセスしやすい「変数の掲示板」として使えばシーンをまたぐ変数も定義できる】
    std::string selectStage = "stage1"; // 選択中のステージ名など
    int scoreMax = -1; // 【シーンをまたぐスコアなど】はマネージャに定義すれば【シーンをまたいだあとも】消えず残る

    std::shared_ptr<Scene> prevScene{ nullptr }; // 一つ前のシーン
    std::shared_ptr<Scene> currentScene{ nullptr }; // 現在のシーン

    std::string currentSceneName = ""; // 現在のシーン名を保管しておく
    std::string changingSceneName = ""; // 予約された次のシーンのクラス名(次に移動予定のシーン名を保管しておく)

    // シーン名を比較、何の変化もなければfalseでシーン移動不必要の判定
    bool isChanging(const std::string& nextSceneName);

    // 次のシーンの予約だけしてLoadScene()を引数なしで呼び出したタイミングで遷移する(ループの途中じゃないキリのいいタイミングを狙って遷移させる)
    void NextScene(const std::string& nextSceneName);

    // シーンをチェンジし遷移する(前のシーンのリセット処理もする)
    void LoadScene(const std::string& sceneName = "");

protected:
    SceneManager() {}; // 外部からのインスタンス作成は禁止
    virtual ~SceneManager() {}; //外部からのインスタンス破棄も禁止
};

#endif


