#pragma once

#include <QProxyStyle>

class CustomStyle : public QProxyStyle {
Q_OBJECT
public:
  CustomStyle() {}

  ~CustomStyle() {}

  void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter,
                     const QWidget *widget) const override;

  void drawSubControl(SubControl sc, const QStyleOption *option, QPainter *painter,
                      const QWidget *widget) const override;
};



