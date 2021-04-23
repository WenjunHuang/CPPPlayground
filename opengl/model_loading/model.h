//
// Created by rick on 2021/4/23.
//

#pragma once
#include "mesh.h"
#include <QtCore>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

class Model {
public:
    Model(const QString& path,QOpenGLFunctions_3_3_Core* functions);
    void draw(QOpenGLShaderProgram *shader);

private:
    QOpenGLFunctions_3_3_Core* _functions;
    QVector<Mesh> _meshes;
    QDir _directory;
    QVector<Texture> _texturesLoaded;

    void loadModel(const QString& path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    QVector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, const QString& typeName);
};
