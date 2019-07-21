#pragma once

#include <QWindow>
#include <QOpenGLFunctions>
#include <QOpenGLPaintDevice>

class OpenGLWindow : public QWindow, protected QOpenGLFunctions {
Q_OBJECT
public:
  explicit OpenGLWindow(QWindow *parent = nullptr);

  ~OpenGLWindow() override;

  virtual void render(QPainter *painter);

  virtual void render();

  virtual void initialize();

  void setAnimating(bool animating);

public slots:
  void renderLater();
  void renderNow();

protected:
  bool event(QEvent* event) override;
  void exposeEvent(QExposeEvent* event) override;

private:
  bool _animating;
  QOpenGLContext *_context;
  QOpenGLPaintDevice *_device;


};



