//
// Created by rick on 2021/7/29.
//

#pragma once
#include "Hud.h"
#include <QtCore>
#include <memory>
#include <skia/core/SkCanvas.h>

class Scenes {
  public:
    using ScenePtr = std::shared_ptr<Scene>;

    Scenes();
    ~Scenes();
    void draw(SkCanvas* canvas,
              int       width,
              int       height,
              float     scale,
              int       mouseX,
              int       mouseY);

    QMap<QString, ScenePtr>& scenesMap() { return _scenesMap; }
    QVector<QString>&        sceneNames() { return _sceneNames; }
    Scene*                   currentScene() { return _currentScene.get(); }
    QString                  currentSceneName() { return _currentSceneName; }
    void ShowNextScene();
    void ShowPrevScene();

  private:
   std::shared_ptr<Scene> newScene(QString name);
    std::shared_ptr<Scene> setScene(QString scene);

  private:
    std::unique_ptr<HUD>    _hud;
    ScenePtr                _currentScene;
    QString                 _currentSceneName;
    QVector<QString>        _sceneNames;
    QMap<QString, ScenePtr> _scenesMap;
};
