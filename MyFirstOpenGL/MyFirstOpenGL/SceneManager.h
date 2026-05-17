#pragma once

class SceneManager
{
public:

    enum SceneType
    {
        SCENE_GAME,
        SCENE_EMPTY
    };

private:

    SceneType currentScene;

public:

    SceneManager();

    void SetScene(SceneType scene);

    SceneType GetCurrentScene() const;

    bool IsGameScene() const;

    bool IsEmptyScene() const;
};

