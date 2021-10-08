//
// Created by rick on 2021/9/11.
//
#include <skia/core/SkCanvas.h>
#include <skia/core/SkSurface.h>
#include <fstream>

void raster(int width, int height, void (*draw)(SkCanvas*), const char* path) {
  sk_sp<SkSurface> rasterSurface =
      SkSurface::MakeRasterN32Premul(width, height);
  SkCanvas* rasterCanvas = rasterSurface->getCanvas();
  draw(rasterCanvas);
  sk_sp<SkImage> img(rasterSurface->makeImageSnapshot());
  if (!img) {
    return;
  }
  sk_sp<SkData> png(img->encode());
  if (!png) {
    return;
  }
  SkFILEWStream out(path);
  (void)out.write(png->data(), png->size());
}

int main() {
  auto surface = SkSurface::MakeRasterN32Premul(200, 200);
  auto canvas = surface->getCanvas();
  SkPaint paint;
  paint.setColor(SkColors::kGreen);
  canvas->clear(SK_ColorWHITE);
  canvas->drawCircle(100, 100, 40, paint);
  canvas->flush();

  auto image = surface->makeImageSnapshot();
  auto data = image->encodeToData(SkEncodedImageFormat::kPNG, 80);

  //  SkFILEWStream out("output1.jpg");
  //  out.write(data->data(),data->size());
  //  out.flush();

  std::ofstream of("output.png", std::ios_base::binary | std::ios_base::out |
                                     std::ios_base::trunc);
  of.write((const char*)data->data(), data->size());
  of.flush();
}