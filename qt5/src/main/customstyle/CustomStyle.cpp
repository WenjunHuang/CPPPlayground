//
// Created by xxzyjy on 07/11/2017.
//

#include <QStyleOption>
#include <QPainter>
#include "CustomStyle.h"

void CustomStyle::drawPrimitive(QStyle::PrimitiveElement element, const QStyleOption *option, QPainter *painter,
                                const QWidget *widget) const {
  if (element == PE_IndicatorSpinUp || element == PE_IndicatorSpinDown){
    QPolygon points(3);
    auto x = option->rect.x();
    auto y = option->rect.y();
    auto w = option->rect.width() / 2;
    auto h = option->rect.height() / 2;
    x += (option->rect.width() - w) / 2;
    y += (option->rect.height() - h) / 2;

    if (element == PE_IndicatorSpinUp){
      points[0] = QPoint(x, y+h);
      points[1] = QPoint(x + w, y + h);
      points[2] = QPoint(x + w / 2,y);
    }else {
      points[0] = QPoint(x, y);
      points[1] = QPoint(x + w,y);
      points[2] = QPoint(x + w / 2, y + h);
    }

    if (option->state & State_Enabled) {
      painter->setPen(option->palette.mid().color());
      painter->setBrush(option->palette.buttonText());
    } else {
      painter->setPen(option->palette.buttonText().color());
      painter->setBrush(option->palette.mid());
    }

    painter->drawPolygon(points);
  }else {
    QProxyStyle::drawPrimitive(element, option, painter, widget);
  }
}
