//
// Created by rick on 2021/9/27.
//
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_TYPES_H
#include FT_OUTLINE_H
#include FT_RENDER_H
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

class Widget : public QWidget {
  Q_OBJECT
 public:
  Widget(const QString& file_name,
         char32_t utf16_char,
         int point_size,
         bool direct_render,
         QWidget* parent = nullptr)
      : QWidget(parent), direct_render_(direct_render) {
    face_ = nullptr;
    library_ = nullptr;

    FT_Init_FreeType(&library_);
    FT_New_Face(library_, file_name.toLatin1().constData(), 0, &face_);
    FT_Set_Char_Size(face_, 0, point_size * 64, logicalDpiX(), logicalDpiX());
    QChar::requiresSurrogates(utf16_char);
    UChar32 unicode;
    int i = 0;
    U16_NEXT_OR_FFFD(&utf16_char, i, 2, unicode);

    FT_UInt glyph_index = FT_Get_Char_Index(face_, unicode);
    FT_Load_Glyph(face_, glyph_index, FT_LOAD_DEFAULT);

    auto left = face_->glyph->metrics.horiBearingX;
    auto right = left + face_->glyph->metrics.width;
    auto top = face_->glyph->metrics.horiBearingY;
    auto bottom = top - face_->glyph->metrics.height;

    glyph_rect_ =
        QRect(QPoint(left / 64, -(top / 64) + 1),
              QSize((right - left) / 64 + 1, (top - bottom) / 64 + 1));
    setFixedSize(300, 300);
  }

  ~Widget() {
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

      if (direct_render_) {
        painter.setPen(Qt::black);

        FT_Raster_Params params;
        params.target = nullptr;
        params.flags = FT_RASTER_FLAG_DIRECT | FT_RASTER_FLAG_AA;
        params.user = &painter;
        params.gray_spans = &Widget::graySpans;
        params.black_spans = nullptr;
        params.bit_set = nullptr;
        params.bit_test = nullptr;

        auto outline = &face_->glyph->outline;
        FT_Outline_Render(library_, outline, &params);
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
  QRect glyph_rect_;
  bool direct_render_;

  static void graySpans(int y, int count, const FT_Span_* spans, void* user) {
    QPainter* painter = reinterpret_cast<QPainter*>(user);
    painter->save();
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
    painter->restore();
  }
};
int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  Widget w("../assets/fonts/NotoSansSC-Regular.otf", U'国', 40, true);
  w.show();
  return app.exec();
}

#include "DrawWithQt.moc"