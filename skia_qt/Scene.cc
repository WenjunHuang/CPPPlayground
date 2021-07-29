//
// Created by rick on 2021/7/29.
//

#include "Scene.h"
#include <QFile>
#include <skia/core/SkData.h>

sk_sp<SkTypeface> Scene::inter = nullptr;
sk_sp<SkTypeface> Scene::jbMono = nullptr;
SkFont Scene::inter13 ;
SkPaint Scene::blackFill;

QByteArray Scene::readResource(const QString& resource)
{
    QFile resourceFile { QString(":%1").arg(resource) };
    resourceFile.open(QFile::ReadOnly);
    return resourceFile.readAll();
}

void ReleaseData(const void* ptr, void* context)
{
    QByteArray* bytes = (QByteArray*)context;
    delete bytes;
}

sk_sp<SkData> Scene::resourceAsData(const QString& path)
{
    auto bytes = new QByteArray { readResource(path) };
    auto data = SkData::MakeWithProc((const void*)bytes->constData(), bytes->size(), &ReleaseData, bytes);
    return data;
}

void Scene::initializeResources()
{
    inter = SkTypeface::MakeFromData(resourceAsData("/assets/fonts/InterHinted-Regular.ttf"));
    jbMono = SkTypeface::MakeFromData(resourceAsData("/assets/fonts/JetBrainsMono-Regular.ttf"));

    inter13.setTypeface(inter);
    inter13.setSize(13);
    inter13.setSubpixel(true);

    blackFill.setColor(0xFF000000);
}
