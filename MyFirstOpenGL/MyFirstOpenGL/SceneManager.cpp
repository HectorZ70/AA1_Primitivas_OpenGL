#include "SceneManager.h"

SceneManager::SceneManager()
    : currentScene(SCENE_GAME)
{
}

void SceneManager::SetScene(SceneType scene)
{
    currentScene = scene;
}

SceneManager::SceneType SceneManager::GetCurrentScene() const
{
    return currentScene;
}

bool SceneManager::IsGameScene() const
{
    return currentScene == SCENE_GAME;
}

bool SceneManager::IsEmptyScene() const
{
    return currentScene == SCENE_EMPTY;
}