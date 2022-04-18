#include "model.h"

void Model::loadModel(QString path)
{
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path.toStdString(), aiProcess_Triangulate | aiProcess_FlipUVs);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        qDebug() << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
        return;
    }
    directory = path.mid(0, path.lastIndexOf('/'));
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
    QVector<Vertex> vertices;
    QVector<unsigned int> indices;
    QVector<Texture> textures;
    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        vertex.Position = QVector3D(mesh->mVertices[i].x,mesh->mVertices[i].y,mesh->mVertices[i].z);
        vertex.Normal = QVector3D(mesh->mNormals[i].x,mesh->mNormals[i].y,mesh->mNormals[i].z);
        if(mesh->mTextureCoords[0])//only process one texture
            vertex.TexCoords = QVector2D(mesh->mTextureCoords[0][i].x,mesh->mTextureCoords[0][i].y);
        else
            vertex.TexCoords = QVector2D(0.0f,0.0f);
        vertices.push_back(vertex);
    }
    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    if(mesh->mMaterialIndex >= 0)
    {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        textures.append(loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse"));
        textures.append(loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular"));
        textures.append(loadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal"));
        textures.append(loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_height"));
    }
    return Mesh(glFunc,vertices,indices, textures);
}

QVector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, QString typeName)
{
    QVector<Texture> textures;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        QString fileName(str.C_Str());
        bool skip = false;
        for(int j = 0; j < textures_loaded.size(); j++)
        {
            if(textures_loaded[j].path == fileName)
            {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }
        if(!skip)
        {
            Texture texture;
            texture.ptr = TextureFromFile(fileName, directory);
            texture.type = typeName;
            texture.path = fileName;
            textures.push_back(texture);
            textures_loaded.push_back(texture);
        }
    }
    return textures;
}

QOpenGLTexture *Model::TextureFromFile(QString fileName, QString directory)
{
    QString filePath = directory + '/' + fileName;
    qDebug() << filePath;

    QOpenGLTexture * res = new QOpenGLTexture(QOpenGLTexture::Target2D);
    QImage img(filePath);
    if (!img.isNull())
    {
        res->setData(img);
        res->setWrapMode(QOpenGLTexture::Repeat);
        res->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
        res->setMagnificationFilter(QOpenGLTexture::Linear);
    }
    else
    {
        qDebug() << "Texture failed to load at path: " << filePath;
    }

    return res;
}
