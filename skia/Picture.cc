//
// Created by rick on 2021/9/11.
//
#include <skia/core/SkPictureRecorder.h>
#include <skia/core/SkPicture.h>
#include <skia/core/SkStream.h>
#include <skia/core/SkSurface.h>
#include <skia/core/SkCanvas.h>
int main(){
  SkPictureRecorder recorder;
  auto canvas = recorder.beginRecording(
      SkIntToScalar(200),
      SkIntToScalar(200)
      );
  SkPaint paint;
  paint.setColor(SK_ColorGREEN);
  paint.setAntiAlias(true);
  canvas->drawCircle(100,100,40,paint);
  auto picture = recorder.finishRecordingAsPicture();
  auto picture_data = picture->serialize();

  auto new_picture = SkPicture::MakeFromData(picture_data.get());
  auto surface = SkSurface::MakeRasterN32Premul(200, 200);
  auto surface_canvas = surface->getCanvas();
  new_picture->playback(surface_canvas);

  {
    auto image = surface->makeImageSnapshot();
    auto data = image->encodeToData(SkEncodedImageFormat::kPNG, 80);

    SkFILEWStream out("serialized_picture.png");
    out.write(data->data(), data->size());
    out.flush();
  }
}