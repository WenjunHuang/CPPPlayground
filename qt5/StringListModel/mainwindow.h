#pragma once

#include <QWidget>
#include <QStringListModel>
#include <QSortFilterProxyModel>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QWidget {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private slots:

    void on_listView_clicked(const QModelIndex &index);

    void on_matchStringLineEdit_textChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    QStringListModel *_model;
    QSortFilterProxyModel *_proxyModel;
    QStringList _colorList;
};

