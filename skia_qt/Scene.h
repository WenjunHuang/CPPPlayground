//
// Created by rick on 2021/7/29.
//

#pragma once
#include <QtCore>
#include <skia/core/SkCanvas.h>
#include <skia/core/SkFont.h>
#include <skia/core/SkPaint.h>
#include <skia/core/SkTypeface.h>

class Scene : public QObject {
    Q_OBJECT
  public:
    QVector<QString> variants{QStringLiteral("Default")};
    int              variantIdx = 0;

    virtual void draw(SkCanvas* canvas,
                      int       width,
                      int       height,
                      float     scale,
                      int       xpos,
                      int       ypos) = 0;

    [[nodiscard]] QString variantTitle() const { return variants[variantIdx]; }

    void changeVariant(int delta) {
        variantIdx =
            (variantIdx + variants.length() + delta) % variants.length();
    }

  public:
    static void initializeResources();

    static QByteArray    readResource(const QString& path);
    static sk_sp<SkData> resourceAsData(const QString& path);

    static sk_sp<SkTypeface> inter;
    static sk_sp<SkTypeface> jbMono;
    static SkFont            inter13;
    static SkPaint           blackFill;
};
