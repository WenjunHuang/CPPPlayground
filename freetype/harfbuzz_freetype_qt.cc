//
// Created by rick on 2021/9/27.
//
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_TYPES_H
#include FT_OUTLINE_H
#include FT_RENDER_H
#include <harfbuzz/hb-ft.h>
#include <harfbuzz/hb-icu.h>
#include <harfbuzz/hb.h>
#include <unicode/utf16.h>
#include <QApplication>
#include <QPainter>
#include <QWidget>
#include <cassert>
#include <memory>
#include <utility>

template <auto* P>
struct FunctionWrapper {
  template <typename... Args>
  auto operator()(Args&&... args) {
    return P(std::forward<Args>(args)...);
  }
};

class HarfbuzzFreeType : public QWidget {
  Q_OBJECT
 public:
  struct RasterUserData {
    QPainter* painter_;
    hb_glyph_info_t* glyph_info_;
    hb_glyph_position_t* glyph_pos_;
    int logical_dpi_x_;
    int logical_dpi_y_;
  };
  HarfbuzzFreeType(const QString& font_file,
                   const char16_t* text,
                   int point_size,
                   bool direct_render,
                   QWidget* parent = nullptr)
      : QWidget(parent), direct_render_(direct_render) {
    face_ = nullptr;
    library_ = nullptr;

    FT_Init_FreeType(&library_);
    FT_New_Face(library_, font_file.toLatin1().constData(), 0, &face_);
    FT_Set_Char_Size(face_, 0, point_size * 64, logicalDpiX(), logicalDpiX());
    hBuffer_ = hb_buffer_create();
    hb_buffer_add_utf16(hBuffer_, (const uint16_t*)text, -1, 0, -1);
    //    hb_buffer_set_unicode_funcs(hBuffer_, hb_icu_get_unicode_funcs());
    hb_buffer_set_direction(hBuffer_, HB_DIRECTION_LTR);

    auto hbFont = hb_ft_font_create(face_, nullptr);
    hb_shape(hbFont, hBuffer_, nullptr, 0);

    //    QChar::requiresSurrogates(utf16_char);
    //    UChar32 unicode;
    //    int i = 0;
    //    U16_NEXT_OR_FFFD(&utf16_char, i, 2, unicode);

    //    auto left = face_->glyph->metrics.horiBearingX;
    //    auto right = left + face_->glyph->metrics.width;
    //    auto top = face_->glyph->metrics.horiBearingY;
    //    auto bottom = top - face_->glyph->metrics.height;
    //
    //    glyph_rect_ =
    //        QRect(QPoint(left / 64, -(top / 64) + 1),
    //              QSize((right - left) / 64 + 1, (top - bottom) / 64 + 1));
    setGeometry(QRect(0, 0, 300, 300));
  }

  ~HarfbuzzFreeType() {
    FT_Done_Face(face_);
    FT_Done_FreeType(library_);
  }

 protected:
  void paintEvent(QPaintEvent* event) override {
    QWidget::paintEvent(event);
    if (library_ && face_) {
      // Direct rendering;
      QPainter painter(this);
      auto s = size() / 2;
      painter.translate(QPoint(s.width(), s.height()));

      {
        // Draw center point
        painter.save();
        painter.setBrush(QColor(255, 0, 0));
        painter.drawRect(0, 0, 2, 2);
        painter.restore();
      }

      auto logical_dpi_x = logicalDpiX();
      auto logical_dpi_y = logicalDpiY();

      if (direct_render_) {
        painter.setPen(Qt::black);
        painter.setRenderHint(QPainter::Antialiasing,true);

        FT_Raster_Params params;
        params.target = nullptr;
        params.flags = FT_RASTER_FLAG_DIRECT | FT_RASTER_FLAG_AA;
        params.gray_spans = &HarfbuzzFreeType::graySpans;
        params.black_spans = nullptr;
        params.bit_set = nullptr;
        params.bit_test = nullptr;

        unsigned int glyphLength;
        auto glyphInfos = hb_buffer_get_glyph_infos(hBuffer_, &glyphLength);
        auto glyphPositions =
            hb_buffer_get_glyph_positions(hBuffer_, &glyphLength);
        for (size_t i = 0; i < glyphLength; i++) {
          RasterUserData userData{&painter, &glyphInfos[i], &glyphPositions[i],
                                  logical_dpi_x, logical_dpi_y};
          params.user = &userData;

          painter.translate(glyphPositions[i].x_offset / logical_dpi_x,
                            glyphPositions[i].y_offset / logical_dpi_y);

          FT_Load_Glyph(face_, glyphInfos[i].codepoint, FT_LOAD_DEFAULT);
          auto outline = &face_->glyph->outline;
          FT_Outline_Render(library_, outline, &params);
          painter.translate(glyphPositions[i].x_advance / logical_dpi_x,
                            glyphPositions[i].y_advance / logical_dpi_y);
        }
      } else {
        FT_Render_Glyph(face_->glyph, FT_RENDER_MODE_NORMAL);
        QImage glyph_image(face_->glyph->bitmap.buffer,
                           face_->glyph->bitmap.width,
                           face_->glyph->bitmap.rows,
                           face_->glyph->bitmap.pitch, QImage::Format_Indexed8);
        QVector<QRgb> color_table;
        for (int i = 0; i < 256; i++) {
          color_table << qRgba(0, 0, 0, i);
        }
        glyph_image.setColorTable(std::move(color_table));
        painter.drawImage(QPoint(0, 0), glyph_image);
      }
    }
  }

 private:
  FT_Library library_;
  FT_Face face_;
  hb_buffer_t* hBuffer_;
  unsigned int upem_;
  bool direct_render_;

  static void graySpans(int y, int count, const FT_Span_* spans, void* user) {
    RasterUserData* userData = reinterpret_cast<RasterUserData*>(user);
    QPainter* painter = userData->painter_;
    y = -y;

    for (int i = 0; i < count; i++) {
      const auto& span = spans[i];
      auto opacity = qreal(span.coverage) / 255.0;
      painter->setOpacity(opacity);
      if (span.len > 1)
        painter->drawLine(span.x, y, span.x + span.len - 1, y);
      else
        painter->drawPoint(span.x, y);
    }
  }
};
int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  HarfbuzzFreeType w("../assets/fonts/NotoSansSC-Regular.otf",
                     u"中华人民共和国", 40, true);
  w.show();
  return app.exec();
}

#include "harfbuzz_freetype_qt.moc"