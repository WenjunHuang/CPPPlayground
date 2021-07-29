//
// Created by rick on 2021/7/29.
//

#include "Hud.h"
#include <skia/core/SkFontMetrics.h>
#include <skia/core/SkRRect.h>

void HUD::draw(SkCanvas* canvas, const Scene* scene, int width, int height)
{
    SkPaint bg;
    bg.setColor(0x90000000);

    SkPaint fg;
    fg.setColor(SK_ColorWHITE);

    SkPaint graph;
    graph.setColor(0xFF00FF00);
    graph.setStrokeWidth(1);

    SkPaint graphPast;
    graphPast.setColor(0x9000FF00);
    graphPast.setStrokeWidth(1);

    SkPaint graphLimit;
    graphLimit.setColor(0xFFCC3333);
    graphLimit.setStrokeWidth(1);

    SkFontMetrics metrics = {};
    Scene::inter13.getMetrics(&metrics);

    auto variantsHeight = scene->variants.size() > 1 ? 25 : 0;

    // Background
    canvas->translate(width - 230, height - 160 - variantsHeight - extras.size() * 25);
    canvas->drawRRect(SkRRect::MakeRectXY(SkRect::MakeLTRB(0, 0, 225, 180 + variantsHeight), 7, 7), bg);
}
