#pragma once

#include <QWidget>

namespace Ui {
    class MainWindow;
}

struct Person{
    QString lastName;
    QString firstName;
    int age;
    QString occupation;
    QString marriageStatus;

};
class MainWindow : public QWidget {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow();


private:
    void newPerson();
    Ui::MainWindow *ui;
    QVector<Person> _table;
};

