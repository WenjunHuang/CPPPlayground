//
// Created by rick on 2022/2/15.
//

#include "person_delegate.h"
#include "person_model.h"
#include <QComboBox>
#include <QPainter>

PersonDelegate::PersonDelegate(QObject *parent) : QItemDelegate{parent} {}

void PersonDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
  if (index.column() == 2) {
    auto combo = qobject_cast<QComboBox *>(editor);
    auto idx = QColor::colorNames().indexOf(index.data(Qt::DisplayRole).toString());
    combo->setCurrentIndex(idx);
  } else
    QItemDelegate::setEditorData(editor, index);
}

void PersonDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
  if (index.column() == 2) {
    auto combo = qobject_cast<QComboBox *>(editor);
    model->setData(index, combo->currentText(), PersonModel::FavoriteColorRole);
//    model->setData(index, combo.currentText(), Qt::EditRole);
  } else
    QItemDelegate::setModelData(editor, model, index);
}

void PersonDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                                          const QModelIndex &index) const {
  editor->setGeometry(option.rect);
}

QWidget *
PersonDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
  if (index.column() == 2) {
    auto editor = new QComboBox{parent};
    auto colors = QColor::colorNames();
    std::for_each(colors.cbegin(), colors.cend(), [editor](const auto &color) {
        QPixmap pix{50, 50};
        pix.fill(QColor{color});
        editor->addItem(QIcon{pix}, color);
    });
    return editor;
  } else {
    return QItemDelegate::createEditor(parent, option, index);
  }
}

QSize PersonDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
  return QItemDelegate::sizeHint(option, index).expandedTo({64, option.fontMetrics.height() + 10});
}

void PersonDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
  if (index.column() == 2) {
    if (option.state & QStyle::State_Selected) {
      painter->fillRect(option.rect, option.palette.highlight());
    }
    auto favColor = index.data(PersonModel::FavoriteColorRole).toString();
    painter->save();
    painter->setBrush(QBrush{QColor{favColor}});

    // Draw color rect
    auto bound = option.rect.adjusted(3, 3, -3, -3);
    painter->drawRect(bound);
    painter->setClipRect(bound);

    // Text size
    auto textSize = option.fontMetrics.size(Qt::TextSingleLine, favColor);
    painter->setBrush(QBrush{QColor{Qt::white}});

    auto widthAdjust = (option.rect.width() - textSize.width()) / 2;
    auto heightAdjust = (option.rect.height() - textSize.height()) / 2;
    painter->drawRect(option.rect.adjusted(widthAdjust,
                                           heightAdjust,
                                           -widthAdjust,
                                           -heightAdjust).marginsAdded({3, 0, 3, 0}));
    painter->drawText(option.rect, favColor, Qt::AlignHCenter | Qt::AlignVCenter);

    painter->restore();
  } else {
    QItemDelegate::paint(painter, option, index);
  }
}
