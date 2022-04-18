#ifndef MODEL_H
#define MODEL_H
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh.h"

class Model
{
    public:
        Model(QOpenGLFunctions_3_3_Core* _glFunc, QString path):glFunc(_glFunc)
        {
            loadModel(path);
        }
        void Draw(QOpenGLShaderProgram &shader)
        {
            for(int i = 0; i < meshes.size(); i++)
                meshes[i].Draw(shader);
        }
    private:
        QOpenGLFunctions_3_3_Core *glFunc;
        QVector<Texture> textures_loaded;
        QVector<Mesh> meshes;
        QString directory;
        void loadModel(QString path);
        void processNode(aiNode *node, const aiScene *scene);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);
        QVector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                             QString typeName);
        QOpenGLTexture *TextureFromFile(QString fileName,QString directory);
};

#endif // MODEL_H
