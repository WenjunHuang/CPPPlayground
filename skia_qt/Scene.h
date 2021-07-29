//
// Created by rick on 2021/7/29.
//

#pragma once
#include <QByteArray>
#include <skia/core/SkPaint.h>
#include <skia/core/SkFont.h>
#include <skia/core/SkTypeface.h>

class Scene {

public:
    static void initializeResources();

    static QByteArray readResource(const QString& path);
    static sk_sp<SkData> resourceAsData(const QString& path);

    static sk_sp<SkTypeface> inter;
    static sk_sp<SkTypeface> jbMono;
    static sk_sp<SkFont> inter13;
    static sk_sp<SkPaint> blackFill;
};
