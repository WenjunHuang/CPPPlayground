//
// Created by rick on 2021/7/29.
//

#include "Hud.h"
#include "Scenes.h"
#include <QtCore>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/range.hpp>
#include <range/v3/view/filter.hpp>
#include <skia/core/SkFontMetrics.h>
#include <skia/core/SkRRect.h>

using namespace ranges;

void drawStringCentered(SkCanvas*      canvas,
                        const QString& text,
                        float          x,
                        float          y,
                        const SkFont&  font,
                        const SkPaint& paint) {
    auto   bytes = text.toUtf8();
    SkRect bounds{};
    font.measureText(bytes.constData(),
                     bytes.size(),
                     SkTextEncoding::kUTF8,
                     &bounds,
                     &paint);

    SkFontMetrics fontMetrics{};
    font.getMetrics(&fontMetrics);
    auto lineHeight = fontMetrics.fCapHeight;

    auto size = bytes.size();
    canvas->drawString(SkString(bytes.constData(), size),
                       x - bounds.right() / 2,
                       y + lineHeight / 2,
                       font,
                       paint);
}

void drawStringLeft(SkCanvas*      canvas,
                    const QString& text,
                    const SkRect&  outer,
                    const SkFont&  font,
                    const SkPaint& paint) {
    auto bytes = text.toUtf8();

    SkRect inner{};
    font.measureText(
        bytes.constData(), bytes.size(), SkTextEncoding::kUTF8, &inner, &paint);

    SkFontMetrics fontMetrics{};
    font.getMetrics(&fontMetrics);
    auto innerHeight = fontMetrics.fDescent - fontMetrics.fAscent;

    canvas->drawString(SkString(bytes.constData(), bytes.size()),
                       outer.left(),
                       outer.top() + (outer.height() - innerHeight) / 2.0f -
                           fontMetrics.fAscent,
                       font,
                       paint);
}

void HUD::draw(SkCanvas* canvas, const Scene* scene, int width, int height) {
    SkPaint bg;
    bg.setAntiAlias(true);
    bg.setColor(0x90000000);

    SkPaint fg;
    fg.setColor(0xFFFFFFFF);

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
    canvas->translate(width - 230,
                      height - 160 - variantsHeight - extras.size() * 25);
    canvas->drawRRect(
        SkRRect::MakeRectXY(
            SkRect::MakeLTRB(0, 0, 225, 180 + variantsHeight), 7, 7),
        bg);
    canvas->translate(5, 5);

    // Scene
    auto buttonButtons =
        SkRRect::MakeRectXY(SkRect::MakeLTRB(0, 0, 30, 20), 2, 2);
    auto labelBounds = SkRect::MakeLTRB(35, 0, 225, 20);
    canvas->drawRRect(buttonButtons, bg);
    drawStringCentered(canvas, "←→", 15, 10, Scene::inter13, fg);

    auto sceneIdx =
        _scenes->sceneNames().indexOf(_scenes->currentSceneName());
    drawStringLeft(canvas,
                   QString("%1/%2 %3")
                       .arg(sceneIdx + 1)
                       .arg(_scenes->sceneNames().length())
                       .arg(_scenes->currentSceneName()),
                   labelBounds,
                   Scene::inter13,
                   fg);
    canvas->translate(0, 25);

    // Variants
    if (scene->variants.length() > 1) {
        labelBounds = SkRect::MakeLTRB(35, 0, 225, 20);
        canvas->drawRRect(buttonButtons, bg);
        drawStringCentered(
            canvas, QStringLiteral(u"↑↓"), 15, 10, Scene::inter13, fg);
        drawStringLeft(canvas,
                       QString("%1/%2 %3")
                           .arg(scene->variantIdx + 1)
                           .arg(scene->variants.length())
                           .arg(scene->variantTitle()),
                       labelBounds,
                       Scene::inter13,
                       fg);
        canvas->translate(0, 25);
    }

    // Extras
    std::for_each(
        extras.keyValueBegin(), extras.keyValueEnd(), [&](const auto& item) {
            SkRRect buttonBounds =
                SkRRect::MakeRectXY(SkRect::MakeXYWH(5, 0, 20, 20), 2, 2);
            canvas->drawRRect(buttonBounds, bg);
            drawStringCentered(canvas, item.first, 15, 10, Scene::inter13, fg);
            drawStringLeft(
                canvas, item.second, labelBounds, Scene::inter13, fg);
            canvas->translate(0, 25);
        });

    // FPS
    canvas->drawRRect(
        SkRRect::MakeRectXY(SkRect::MakeLTRB(0, 0, _times.size(), 45), 2, 2),
        bg);
    std::for_each(
        _times.begin(), _times.end(), [&, index = 0](auto time) mutable {
            canvas->drawLine(index,
                             45,
                             index,
                             45.0f - time,
                             index > _timesIdx ? graphPast : graph);
            index++;
        });
    std::array rates{30, 60, 120};
    std::for_each(rates.begin(), rates.end(), [&](auto rate) {
        auto frameTime = 1000.0f / rate;
        canvas->drawLine(
            0, 45 - frameTime, _times.size(), 45 - frameTime, graphLimit);
    });

    auto timesGreaterThenZero =
        _times | views::filter([](const auto& item) {
            return item > 0;
        });
    int  length = 0;
    auto sum =
        accumulate(timesGreaterThenZero, 0.0, [&length](double sum, double item) {
            length++;
            return sum + item;
        });
    auto time = QString("%1ms").arg(length > 0 ? sum / length : 0.0,0,'g',2);
    drawStringLeft(canvas,
                   time,
                   SkRect::MakeLTRB(_times.size() + 5, 0, 225, 20),
                   Scene::inter13,
                   fg);
    auto fps =
        QString("%1 fps").arg(length > 0 ? 1000.0f / sum * length : 0.0,0,'f',2);
    drawStringLeft(canvas,
                   fps,
                   SkRect::MakeLTRB(_times.size() + 5, 25, 225, 40),
                   Scene::inter13,
                   fg);
    canvas->translate(0, 25);
    //             accumulate(0, [](int sum, int item) { return sum + item; });
}

void HUD::tick() {
    auto t1           = system_clock::now();
    _times[_timesIdx] = duration_cast<nanoseconds>(t1 - t0).count() / 1000000.0;
    t0                = t1;
    _timesIdx         = (_timesIdx + 1) % _times.size();
}
