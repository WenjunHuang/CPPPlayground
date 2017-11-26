//
// Created by xxzyjy on 29/10/2017.
//
#include "catch.hpp"
#include "fixture.h"
#include "ImageProcessor.h"

TEST_CASE_METHOD(TestFixture,"mainwindow_imageprocessor","[integration]"){
  auto mainWindow = new ImageProcessor();
  mainWindow->show();
  application.exec();
}

