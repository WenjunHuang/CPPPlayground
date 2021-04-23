//
// Created by rick on 2021/4/23.
//

#include "model.h"
#include <QDebug>
#include <range/v3/all.hpp>
#include <algorithm>

void Model::loadModel(const QString& path)
{
    Assimp::Importer import;
    auto scene = import.ReadFile(path.toLatin1().constData(), aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        qInfo() << "ERROR::ASSIMP::" << import.GetErrorString();
        return;
    }

    _directory = path.left(path.lastIndexOf('/'));

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    // process all the node's meshes (if any)
    for (auto i = 0; i < node->mNumMeshes; i++) {
        auto mesh = scene->mMeshes[node->mMeshes[i]];
        _meshes.append(processMesh(mesh, scene));
    }

    // then do the same for each of its children
    for (auto i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    QVector<Vertex> vertices;
    QVector<uint32_t> indices;
    QVector<Texture> textures;

    for (auto i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        // positions
        vertex.Position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };

        //normals
        if (mesh->HasNormals()) {
            vertex.Normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };
        }

        // texture coordinates
        if (mesh->mTextureCoords[0]) {
            vertex.TexCoords = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
            vertex.Tangent = { mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z };
            vertex.Bitangent = { mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z };
        } else {
            vertex.TexCoords = { 0.0f, 0.0f };
        }

        // process vertex positions, normals and texture coordinates
        vertices.append(vertex);
    }
    // process indices
    for (auto i = 0; i < mesh->mNumFaces; i++) {
        auto face = mesh->mFaces[i];
        for (auto j = 0; j < face.mNumIndices; j++)
            indices.append(face.mIndices[j]);
    }

    // process material
    auto material = scene->mMaterials[mesh->mMaterialIndex];

    // 1. diffuse maps
    auto diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.append(diffuseMaps);
    // 2. specular maps
    auto specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.append(specularMaps);

    // 3. normal maps
    auto normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    textures.append(normalMaps);

    // 4. height maps
    auto heightMaps = loadMaterialTextures(material,aiTextureType_AMBIENT,"texture_height");
    textures.append(heightMaps);

    return Mesh { std::move(vertices), std::move(indices), std::move(textures), _functions };
}

Model::Model(const QString& path, QOpenGLFunctions_3_3_Core* functions)
    : _functions { functions }
{
    loadModel(path);
}

std::shared_ptr<QOpenGLTexture> textureFromFile(const QString& path, const QDir& directory)
{
    QFileInfo fileInfo { directory, path };
    std::shared_ptr<QOpenGLTexture> texture = std::make_shared<QOpenGLTexture>(QOpenGLTexture::Target2D);
    texture->setWrapMode(QOpenGLTexture::Repeat);
    texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);
    texture->setData(QImage(fileInfo.absoluteFilePath()));

    return texture;
}

QVector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, const QString& typeName)
{
    QVector<Texture> textures;
    for (auto i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        // check if texture was loaded
        bool skip = false;
        for (const auto& textureLoaded : _texturesLoaded) {
            if (textureLoaded.textureFileInfo.path() == str.C_Str()) {
                textures.append(textureLoaded);
                skip = true;
                break;
            }
        }
        if (!skip) {
            _texturesLoaded.append({ .texture = textureFromFile(str.C_Str(), _directory), .type = typeName, .textureFileInfo = QFileInfo { _directory, str.C_Str() } });
        }
    }
    return textures;
}
void Model::draw(QOpenGLShaderProgram *shader)
{
    std::for_each(_meshes.begin(),_meshes.end(),[=](auto& mesh){
        mesh.draw(shader);
    });
}
