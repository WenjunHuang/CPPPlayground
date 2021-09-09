//
// Created by rick on 2021/7/29.
//

#include "Scenes.h"
#include "scenes/BitmapImageScene.h"
#include "scenes/BitmapScene.h"

void Scenes::draw(SkCanvas* canvas,
                  int width,
                  int height,
                  float scale,
                  int mouseX,
                  int mouseY) {
  canvas->clear(SK_ColorWHITE);
  if (_currentScene) {
    int layer = canvas->save();
    canvas->scale(scale, scale);
    _currentScene->draw(canvas, width, height, scale, mouseX, mouseY);
    canvas->restoreToCount(layer);

    _hud->tick();
    layer = canvas->save();
    canvas->scale(scale, scale);
    _hud->draw(canvas, _currentScene.get(), width, height);
    canvas->restoreToCount(layer);
  }
}

Scenes::Scenes() : _hud{std::make_unique<HUD>(this)} {
  _hud->extras["V"] = "OFF";

  qRegisterMetaType<BitmapScene*>("BitmapScene*");
  qRegisterMetaType<BitmapImageScene*>("BitmapImageScene*");
  _sceneNames = {"Bitmap", "BitmapImage"};
  _currentScene = setScene("BitmapImage");
}

Scenes::~Scenes() = default;

std::shared_ptr<Scene> Scenes::newScene(QString name) {
  auto type = QString("%1Scene*").arg(name.replace(" ", "")).toLatin1();
  auto id = QMetaType::type(type);
  if (id != 0) {
    const QMetaObject* mo = QMetaType::metaObjectForType(id);
    if (!mo)
      return nullptr;
    QObject* instance = mo->newInstance();
    return std::shared_ptr<Scene>(qobject_cast<Scene*>(instance));
  } else
    return nullptr;
}
std::shared_ptr<Scene> Scenes::setScene(QString name) {
  if (!_sceneNames.contains(name))
    return nullptr;
  else {
    _currentSceneName = name;
    if (_scenesMap.contains(name))
      return _scenesMap[name];
    else {
      auto scene = newScene(name);
      if (scene == nullptr)
        return nullptr;
      else {
        std::shared_ptr<Scene> scenePtr(scene);
        _scenesMap.insert(name, scenePtr);
        return scenePtr;
      }
    }
  }
}
void Scenes::ShowNextScene() {
  auto index = _sceneNames.indexOf(_currentSceneName);
  if (index == -1)
    return;

  auto next_scene_name = _sceneNames[(index + 1) % _sceneNames.length()];
  _currentScene = setScene(next_scene_name);
}
void Scenes::ShowPrevScene() {
  auto index = _sceneNames.indexOf(_currentSceneName);
  if (index == -1)
    return;

  auto prev = index - 1;
  if (prev == -1)
    prev = _sceneNames.length() - 1;
  auto next_scene_name = _sceneNames[prev];
  _currentScene = setScene(next_scene_name);
}
