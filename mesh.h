#ifndef MESH_H
#define MESH_H

#include <QVector3D>
#include <QVector2D>
#include <QString>
#include <QVector>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLContext>

struct Vertex {
    QVector3D Position;
    QVector3D Normal;
    QVector2D TexCoords;
};

struct Texture {
    QOpenGLTexture* ptr;
    QString type;
    QString path;
};

class Mesh
{
public:
        QVector<Vertex> vertices;
        QVector<unsigned int> indices;
        QVector<Texture> textures;
        Mesh() = default;
        Mesh(QOpenGLFunctions_3_3_Core *_glFunc, QVector<Vertex> _vertices, QVector<unsigned int> _indices, QVector<Texture> _textures);
        void Draw(QOpenGLShaderProgram& _shader);
    private:
        QOpenGLFunctions_3_3_Core *glFunc;
        GLuint VAO,VBO,EBO;
        void setupMesh();
};

#endif // MESH_H
