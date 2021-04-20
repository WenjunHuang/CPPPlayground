#include <QGuiApplication>
#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLShaderProgram>
#include <QSurfaceFormat>
#include <QWindow>
//
// Created by rick on 2021/4/17.
//

class MainWindow : public QWindow, private QOpenGLFunctions_4_5_Core {
    Q_OBJECT
  public:
    MainWindow(){
        setSurfaceType(SurfaceType::OpenGLSurface);

        QSurfaceFormat format;
        format.setSamples(16);
        format.setVersion(4, 6);
        format.setProfile(QSurfaceFormat::CoreProfile);
        setFormat(format);
    }
  private:
    QOpenGLContext* _context;
    QOpenGLShaderProgram* _program;
    unsigned int _vao;
}

int main(int argc,char* argv[]){
    QGuiApplication app{argc,argv};


    return app.exec();
}