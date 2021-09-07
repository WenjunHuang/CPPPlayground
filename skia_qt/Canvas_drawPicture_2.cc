//
// Created by rick on 2021/9/7.
//

#include <skia/core/SkPictureRecorder.h>
#include "window.h"

int main() {
  Run([](SkCanvas* canvas) {
    canvas->clear(SK_ColorWHITE);

    SkPictureRecorder recorder;
    SkCanvas* recordingCanvas = recorder.beginRecording(50, 50);
    for (auto color : {SK_ColorRED, SK_ColorBLUE, 0xFF007F00}) {
      SkPaint paint;
      paint.setColor(color);
      recordingCanvas->drawRect({10, 10, 30, 40}, paint);
      recordingCanvas->translate(10, 10);
      recordingCanvas->scale(1.2f, 1.4f);
    }
    auto playback = recorder.finishRecordingAsPicture();
    auto playbackPtr = playback.get();

    SkPaint paint;
    SkMatrix matrix;
    matrix.reset();
    for (auto alpha : {70, 140, 210}) {
      paint.setAlpha(alpha);
      canvas->drawPicture(playbackPtr, &matrix, &paint);
      matrix.preTranslate(70, 70);
    }
  });
}