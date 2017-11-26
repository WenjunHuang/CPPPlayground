#pragma once
#include <QtWidgets>
#include <qmainwindow.h>
#include "ShowWidget.h"

class ImageProcessor : public QMainWindow{
  Q_OBJECT
public:
  ImageProcessor(QWidget *parent = nullptr);
  ~ImageProcessor();

  void createActions();
  void createMenus();
  void createToolBars();
  void loadFile(QString filename);

  void mergeFormat(QTextCharFormat);

private:
  QMenu *fileMenu;
  QMenu *zoomMenu;
  QMenu *rotateMenu;
  QMenu *mirrorMenu;
  QImage image;
  QString fileName;
  ShowWidget *showWidget;
  QAction *openFileAction;
  QAction *newFileAction;
  QAction *printTextAction;
  QAction *printImageAction;
  QAction *exitAction;
  QAction *copyAction;
  QAction *cutAction;
  QAction *pasteAction;
  QAction *aboutAction;
  QAction *zoomInAction;
  QAction *zoomOutAction;
  QAction *rotate90Action;
  QAction *rotate180Action;
  QAction *rotate270Action;
  QAction *mirrorVerticalAction;

  QAction *mirrorHorizontalAction;
  QAction *undoAction;
  QAction *redoAction;
  QToolBar *fileTool;
  QToolBar *zoomTool;
  QToolBar *rotateTool;
  QToolBar *mirrorTool;
  QToolBar *doToolBar;



};



