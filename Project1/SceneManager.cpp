#include "SceneManager.h"

#include "Scene.h"
#include "TitleScene.h"
#include "PlayScene.h"
#include "GameOverScene.h"

bool SceneManager::isChanging(const std::string& nextSceneName)
{
    if (currentSceneName != nextSceneName) //シーン名不一致
        return true; // シーン名が変更

    return false; // シーン名に変更無し
}

void SceneManager::NextScene(const std::string& nextSceneName)
{
    // 遷移予定nextシーン名と現在のシーン名を比較、何の変化もなければfalseでシーン移動不必要の判定
    if (isChanging(nextSceneName) == false) return; //特に変更なしでシーン移動の必要なし

    changingSceneName = nextSceneName; // 次に移動予定のシーン名を保管しておく
}

void SceneManager::LoadScene(const std::string& sceneName)
{
    std::string loadSceneName = sceneName;     // sceneNameの指定が空の場合にはNextScene()での事前のシーン変更予約があるかチェック
    if (sceneName == "" && changingSceneName != "")
        loadSceneName = changingSceneName;

    if (loadSceneName == "") return; // シーン指定がないときは終了
    // 現在のシーンの終了処理

    if (currentScene != nullptr)
        currentScene->Finalize(); // 終了処理を呼び出す

    if (loadSceneName == "TitleScene")
    {
        // 次のシーンの生成
        currentScene = std::make_shared<TitleScene>();
    }
    else if (loadSceneName == "PlayScene")
    {
        // 次のシーンの生成
        currentScene = std::make_shared<PlayScene>();
    }
    else if (loadSceneName == "GameOverScene")
    {
        // 次のシーンの生成
        currentScene = std::make_shared<GameOverScene>();
    }
    else
        assert("指定されたシーン名の生成処理が見つからなかった→SceneManager.cppを見直しましょう" == "");

    currentSceneName = loadSceneName; // 現在のシーン名の更新
    changingSceneName = ""; // NextScenceのシーン予約名をリセット

    // 次のシーンの初期化
    currentScene->Initialize(); // 初期化を呼び出す
}

