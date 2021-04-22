//
// Created by rick on 2021/4/22.
//

#pragma once

#include <QOpenGLFunctions_3_3_Core>
#include <QVector2D>
#include <QVector3D>
#include <QtCore>
#include <QtGui>
#include <memory>

struct Vertex {
    QVector3D Position;
    QVector3D Normal;
    QVector2D TexCoords;
};
struct Texture {
    std::unique_ptr<QOpenGLTexture> texture;
    QString type;
};

class Mesh {
public:
    QVector<Vertex> vertices;
    QVector<uint32_t> indices;
    QVector<Texture> textures;

    Mesh(QVector<Vertex>&& vertices,
        QVector<uint32_t>&& indices,
        QVector<Texture>&& textures,
         QOpenGLFunctions_3_3_Core* functions);

    void draw(QOpenGLShaderProgram *shader);
private:
    QOpenGLFunctions_3_3_Core *_functions;
    std::unique_ptr<QOpenGLVertexArrayObject> _vao;
    QOpenGLBuffer _vbo{QOpenGLBuffer::VertexBuffer};
    QOpenGLBuffer _ebo{QOpenGLBuffer::IndexBuffer};

    void setupMesh();
};
