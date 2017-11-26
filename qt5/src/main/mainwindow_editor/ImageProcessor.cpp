//
// Created by xxzyjy on 28/10/2017.
//

#include "ImageProcessor.h"
#include <qmenubar.h>
#include <qtoolbar.h>

ImageProcessor::ImageProcessor(QWidget *parent) : QMainWindow(parent) {
  setWindowTitle(tr("Easy Word"));
  showWidget = new ShowWidget(this);
  setCentralWidget(showWidget);

  createActions();
  createMenus();
  createToolBars();

  if (image.load(":/images/image.png")) {
    showWidget->imageLabel->setPixmap(QPixmap::fromImage(image));
  }

}

void ImageProcessor::createActions() {
  openFileAction = new QAction(QIcon(":/images/open.png"), tr("打开"), this);
  openFileAction->setShortcut(tr("Ctrl+O"));
  openFileAction->setStatusTip(tr("打开一个文件"));

  newFileAction = new QAction(QIcon(":/images/new.png"), tr("新建"), this);
  newFileAction->setShortcut(tr("Ctrl+N"));
  newFileAction->setStatusTip(tr("新建一个文件"));

  exitAction = new QAction(tr("退出"), this);
  exitAction->setShortcut(tr("Ctrl+Q"));
  exitAction->setStatusTip(tr("退出程序"));
  connect(exitAction, &QAction::triggered, this, &QMainWindow::close);

  copyAction = new QAction(QIcon(":/images/copy.png"), tr("复制"), this);
  copyAction->setShortcut(tr("Ctrl+C"));
  copyAction->setStatusTip(tr("复制文件"));
  connect(copyAction, &QAction::triggered, showWidget->text, &QTextEdit::copy);

  cutAction = new QAction(QIcon(":/images/cut.png"), tr("剪切"), this);
  cutAction->setShortcut(tr("Ctrl+X"));
  cutAction->setStatusTip(tr("剪切文件"));
  connect(cutAction, &QAction::triggered, showWidget->text, &QTextEdit::cut);

  pasteAction = new QAction(QIcon(":/images/paste.png"), tr("粘贴"), this);
  pasteAction->setShortcut(tr("Ctrl+V"));
  pasteAction->setStatusTip(tr("粘贴文件"));
  connect(pasteAction, &QAction::triggered, showWidget->text, &QTextEdit::paste);

  aboutAction = new QAction(QIcon(":/images/about.png"),tr("关于"), this);
  connect(aboutAction, &QAction::triggered, this, &QApplication::aboutQt);

  printTextAction = new QAction(QIcon(":/images/printText.png"), tr("打印文本"), this);
  printTextAction->setStatusTip(tr("打印一个文本"));
  printImageAction = new QAction(QIcon(":/images/printImage.png"), tr("打印图像"), this);
  printImageAction->setStatusTip(tr("打印一副图像"));

  zoomInAction = new QAction(QIcon(":/images/zoomin.png"), tr("放大"), this);
  zoomOutAction = new QAction(QIcon(":/images/zoomout.png"), tr("缩小"), this);

  rotate90Action = new QAction(QIcon(":/images/rotate90.png"), tr("旋转90"), this);
  rotate90Action->setStatusTip(tr(" 将一幅图旋转90°"));
  rotate180Action = new QAction(QIcon(":/images/rotate180.png"),tr("旋转180"),this);

}

void ImageProcessor::createMenus() {
  fileMenu = menuBar()->addMenu(tr("文件"));
  fileMenu->addAction(openFileAction);
}

void ImageProcessor::createToolBars() {
  fileTool = addToolBar("File");
  fileTool->addAction(openFileAction);

  zoomTool = addToolBar("Edit");
  zoomTool->addAction(copyAction);
}

void ImageProcessor::loadFile(QString filename) {

}

void ImageProcessor::mergeFormat(QTextCharFormat) {

}

ImageProcessor::~ImageProcessor() {

}
