//
// Created by rick on 2021/4/22.
//
#include "mesh.h"
#include <range/v3/all.hpp>
#include <optional>
Mesh::Mesh(QVector<Vertex>&& vertices, QVector<uint32_t>&& indices, QVector<Texture>&& textures, QOpenGLFunctions_3_3_Core* functions)
    : vertices { std::move(vertices) }
    , indices { std::move(indices) }
    , textures { std::move(textures) }
    , _functions { functions }
{
    setupMesh();
}

void Mesh::setupMesh()
{
    _vao = std::make_unique<QOpenGLVertexArrayObject>();
    QOpenGLVertexArrayObject::Binder bindGuard { _vao.get() };

    _vbo.create();
    _ebo.create();

    _vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    _vbo.bind();
    _vbo.allocate(vertices.data(), vertices.size() * sizeof(Vertex));

    _ebo.bind();
    _ebo.allocate(indices.data(), sizeof(uint32_t) * indices.size());

    // vertex positions
    _functions->glEnableVertexAttribArray(0);
    _functions->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);

    // vertex normals
    _functions->glEnableVertexAttribArray(1);
    _functions->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

    // vertex texture coords
    _functions->glEnableVertexAttribArray(2);
    _functions->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
}

void Mesh::draw(QOpenGLShaderProgram* shader)
{
    using namespace ranges;
    for (int32_t diffuseNr = 1, specularNr = 1; auto&& [index, value] : textures | views::enumerate) {
        _functions->glActiveTexture(GL_TEXTURE0 + index);
        std::optional<int32_t> number;
        if (value.type == "texture_diffuse") {
            number = diffuseNr++;
        } else if (value.type == "texture_specular") {
            number = specularNr++;
        }
        if(number) {
            shader->setUniformValue(QString("material.%1%2").arg(value.type, number.value()).toLatin1().constData(), (GLint)index);
            value.texture->bind(index);
        }
    }

    // draw mesh
    QOpenGLVertexArrayObject::Binder bindGuard{_vao.get()};
    _functions->glDrawElements(GL_TRIANGLES,indices.size(),GL_UNSIGNED_INT,0);
}
