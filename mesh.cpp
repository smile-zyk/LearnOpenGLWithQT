#include "mesh.h"

Mesh::Mesh(QOpenGLFunctions_3_3_Core *_glFunc, QVector<Vertex> _vertices, QVector<unsigned int> _indices, QVector<Texture> _textures)
    :vertices(_vertices), indices(_indices), textures(_textures),glFunc(_glFunc)
{
    setupMesh();
}


void Mesh::Draw(QOpenGLShaderProgram &shader)
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr   = 1;
    unsigned int heightNr   = 1;
    for(int i = 0; i < textures.size(); i++)
    {
        QString number;
        QString name = textures[i].type;
        if(name == "texture_diffuse")
            number = QString::number(diffuseNr++);
        else if(name == "texture_specular")
            number = QString::number(specularNr++);
        else if(name == "texture_normal")
            number = QString::number(normalNr++); // transfer unsigned int to string
        else if(name == "texture_height")
            number = QString::number(heightNr++); // transfer unsigned int to string
        shader.setUniformValue(("material." + name + number).toStdString().c_str(), i);
        textures[i].ptr->bind(i);
    }
    glFunc->glBindVertexArray(VAO);
    glFunc->glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glFunc->glBindVertexArray(0);
}

void Mesh::setupMesh()
{
    glFunc->glGenVertexArrays(1, &VAO);
    glFunc->glGenBuffers(1, &VBO);
    glFunc->glGenBuffers(1, &EBO);

    glFunc->glBindVertexArray(VAO);
    glFunc->glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glFunc->glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glFunc->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glFunc->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                 &indices[0], GL_STATIC_DRAW);

    glFunc->glEnableVertexAttribArray(0);
    glFunc->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glFunc->glEnableVertexAttribArray(1);
    glFunc->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    glFunc->glEnableVertexAttribArray(2);
    glFunc->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    glFunc->glBindVertexArray(0);
}

//Mesh::Mesh(QOpenGLFunctions_3_3_Core *_glFunc, QVector<Vertex> _vertices, QVector<unsigned int> _indices, QVector<Texture> _textures)
//    :vertices(_vertices), indices(_indices), textures(_textures),glFunc(_glFunc)
//{
//    setupMesh();
//}

//void Mesh::Draw(QOpenGLShaderProgram &shader)
//{
//        unsigned int diffuseNr = 1;
//        unsigned int specularNr = 1;
//        for(int i = 0; i < textures.size(); i++)
//        {
//            QString number;
//            QString name = textures[i].type;
//            if(name == "texture_diffuse")
//                number = QString::number(diffuseNr++);
//            else if(name == "texture_specular")
//                number = QString::number(specularNr++);
//            shader.setUniformValue(("material." + name + number).toStdString().c_str(), i);
//            textures[i].texture->bind(i);
//        }
//        QOpenGLVertexArrayObject::Binder binder(&VAO);
//        glFunc->glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
//}

//void Mesh::setupMesh()
//{
//    VAO.create();
//    VBO.create();
//    EBO.create();
//    VAO.bind();
//    VBO.bind();
//    EBO.bind();
//    VBO.allocate(vertices.data(),vertices.size() * sizeof(Vertex));
//    EBO.allocate(indices.data(),indices.size() * sizeof(unsigned int));
//    // fuck QTOPENGL
//    QOpenGLShaderProgram tmp;
//    tmp.create();
//    tmp.setAttributeBuffer(0,GL_FLOAT,offsetof(Vertex,Position),3,sizeof (Vertex));
//    tmp.setAttributeBuffer(1,GL_FLOAT,offsetof(Vertex,Normal),3,sizeof (Vertex));
//    tmp.setAttributeBuffer(2,GL_FLOAT,offsetof(Vertex,TexCoords),2,sizeof (Vertex));
//    tmp.enableAttributeArray(0);
//    tmp.enableAttributeArray(1);
//    tmp.enableAttributeArray(2);
//    VAO.release();
//    VBO.release();
//    EBO.release();
//}

