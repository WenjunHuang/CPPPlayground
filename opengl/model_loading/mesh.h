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
    QVector3D Tangent;
    QVector3D Bitangent;
};

struct Texture {
    std::shared_ptr<QOpenGLTexture> texture;
    QString type;
    QFileInfo textureFileInfo;
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
    Mesh(const Mesh&) = default;
    Mesh(Mesh&&) = default;

    void draw(QOpenGLShaderProgram* shader);

private:
    QOpenGLFunctions_3_3_Core* _functions;
    std::shared_ptr<QOpenGLVertexArrayObject> _vao;
    QOpenGLBuffer _vbo { QOpenGLBuffer::VertexBuffer };
    QOpenGLBuffer _ebo { QOpenGLBuffer::IndexBuffer };

    void setupMesh();
};
