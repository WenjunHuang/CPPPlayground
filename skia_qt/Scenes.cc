//
// Created by rick on 2021/7/29.
//

#include "Scenes.h"
#include "scenes/BitmapScene.h"

void Scenes::draw(SkCanvas* canvas,
                  int       width,
                  int       height,
                  float     scale,
                  int       mouseX,
                  int       mouseY) {
    canvas->clear(0xFFFFFFFF);
    if (_currentScene) {
        canvas->save();
        canvas->scale(scale,scale);
        _currentScene->draw(canvas,width,height,scale,mouseX,mouseY);
        canvas->restore();

        _hud->tick();
        canvas->save();
        _hud->draw(canvas, _currentScene.get(), width, height);
        canvas->restore();
    }
}

Scenes::Scenes() : _hud{std::make_unique<HUD>(this)} {
    _currentScene     = std::make_shared<BitmapScene>();
    _hud->extras["V"] = "OFF";

    _sceneNames       = {"Bitmap"};
    setScene("Bitmap");
}

Scenes::~Scenes() {}

Scene* Scenes::newScene(QString name) {
    auto id = QMetaType::type(
        QString("%1Scene").arg(name.replace(" ", "")).toLatin1());
    if (id != 0)
        return static_cast<Scene*>(QMetaType::construct(id, nullptr, nullptr));
    else
        return nullptr;
}
Scene* Scenes::setScene(QString name) {
    _currentSceneName = name;
    if (!_sceneNames.contains(name))
        return nullptr;
    else {
        if (_scenesMap.contains(name))
            return _scenesMap[name].get();
        else {
            auto scene = newScene(name);
            if (scene == nullptr)
                return nullptr;
            else {
                _scenesMap.insert(name, std::shared_ptr<Scene>(scene));
                return scene;
            }
        }
    }
}
