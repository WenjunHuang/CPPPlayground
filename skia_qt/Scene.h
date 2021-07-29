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
  QVector<QString> variants;

  virtual void draw(SkCanvas *canvas, int width, int height, int xpos,
                    int ypos) = 0;

public:
  static void initializeResources();

  static QByteArray readResource(const QString &path);
  static sk_sp<SkData> resourceAsData(const QString &path);

  static sk_sp<SkTypeface> inter;
  static sk_sp<SkTypeface> jbMono;
  static SkFont inter13;
  static SkPaint blackFill;
};
