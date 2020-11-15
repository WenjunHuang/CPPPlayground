//
// Created by rick on 2020/10/27.
//

#pragma once
#include <QString>
#include <QVector>

class QByteArray;

namespace Utils::ByteArray {
    QVector<QByteArray> splitToViews(const QByteArray& in,
                                     const QByteArray& sep,
                                     const QString::SplitBehavior behavior = QString::KeepEmptyParts);
    const QByteArray midView(const QByteArray& in,int pos,int len=-1);
}
