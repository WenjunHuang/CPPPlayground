//
// Created by rick on 2020/10/27.
//
#include "bytearray.h"
QVector<QByteArray>
Utils::ByteArray::splitToViews(const QByteArray& in, const QByteArray& sep,
                               const QString::SplitBehavior behavior) {
    if (sep.isEmpty())
        return {in};

    QVector<QByteArray> ret;
    ret.reserve((behavior == QString::KeepEmptyParts)?(1+(in.size() / sep.size())):(1+(in.size() / (sep.size()+1))));
    int head = 0;
    while (head < in.size()){
        int end = in.indexOf(sep,head);
        if (end < 0)
            end = in.size();
        const QByteArray part = QByteArray::fromRawData((in.constData() + head),(end - head));
        if (!part.isEmpty() || (behavior == QString::KeepEmptyParts))
            ret += part;
        head = end + sep.size();
    }
    return ret;
}
const QByteArray Utils::ByteArray::midView(const QByteArray& in, int pos,
                                           int len) {
    if ((pos < 0)||(pos >= in.size()) || (len == 0))
        return {};
}
