#include <QString>
#include <QDebug>
#include <memory>
int main() {
    QString a{"hello world"};
    QString b{std::move(a)};
    qDebug() << a;
    qDebug() << b;

}