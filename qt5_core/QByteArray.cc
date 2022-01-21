#include <QString>
#include <QDebug>

int main() {
  QString greeting = "Hello World";
  QByteArray buffer(greeting.toLatin1());
  buffer.append(":");

  qInfo() << buffer;
  qInfo() << buffer.rightJustified(20, '.');
  qInfo() << buffer.at(buffer.length() - 1);
  QString modified(buffer);
  qInfo() << modified;

}

