//
// Created by rick on 2021/9/11.
//
#include <skia/core/SkDocument.h>
#include <skia/core/SkStream.h>
#include <skia/docs/SkPDFDocument.h>
#include <skia/core/SkCanvas.h>

int main() {
  SkFILEWStream stream("output.pdf");
  auto document = SkPDF::MakeDocument(&stream);
  auto canvas = document->beginPage(200,200);
  SkPaint paint;
  paint.setColor(SK_ColorGREEN);
  canvas->drawCircle(100,100,40,paint);
  document->endPage();
  document->close();
}