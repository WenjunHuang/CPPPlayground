//
// Created by xxzyjy on 29/10/2017.
//
#include "catch.hpp"
#include "fixture.h"
#include <QtWidgets>
#include <QDebug>
#include <QFileSystemModel>
#include <QSplitter>
#include <QDir>
#include <QTreeView>
#include <QListView>

TEST_CASE_METHOD(TestFixture, "simple_file_view", "[Model_View]") {
  auto splitter = new QSplitter;
  auto model = new QFileSystemModel;
  model->setRootPath(QDir::currentPath());

  auto tree = new QTreeView(splitter);
  tree->setModel(model);
  tree->setRootIndex(model->index(QDir::homePath()));

  auto list = new QListView(splitter);
  list->setModel(model);
  list->setRootIndex(model->index(QDir::currentPath()));

  splitter->setWindowTitle("Two views onto the same file system model");
  splitter->show();

  application.exec();
}

TEST_CASE_METHOD(TestFixture, "model_play", "[ModelView]") {
  auto model = new QFileSystemModel;
  auto parentIndex = model->index(QDir::homePath());

  auto numRows = model->rowCount(parentIndex);
  qDebug() << "numRows:" << numRows;
  for (int row = 0; row < numRows; ++row) {
    auto index = model->index(row, 0, parentIndex);
    auto text = model->data(index, Qt::DisplayRole).toString();

    qDebug() << index << " " << text;
  }
}

TEST_CASE_METHOD(TestFixture,"read_model","[ModelView]"){
  auto model = new QFileSystemModel;
  auto parentIndex = model->index(QDir::homePath());
  qDebug() << model->rowCount(parentIndex);
}

TEST_CASE_METHOD(TestFixture,"string_model","[ModelView]"){
//  QStringList numbers;
//  numbers << "One"<<"Two"<<"Three"<<"Four";
//
//  StringListModel model(numbers);
//  QListView view;
//  view.setModel(&model);
//
//  view.show();
//  application.exec();
}
