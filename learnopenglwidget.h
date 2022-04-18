#ifndef LEARNOPENGLWIDGET_H
#define LEARNOPENGLWIDGET_H

#include <QTime>
#include <QTimer>
#include <QVector>
#include <QString>
#include <QListWidget>
#include <QThreadPool>
#include <QMatrix4x4>

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLContext>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QImage>
#include <QMouseEvent>
#include <QWheelEvent>

#include "camera.h"
#include "model.h"

class LearnOpenGLWidget : public QOpenGLWidget,QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    enum State{RUN,PAUSE};
    State state;
    LearnOpenGLWidget(QWidget *parent = nullptr);
    ~LearnOpenGLWidget();
    void LoadModel(QString path);
signals:
    void debugMessage(QString message);
protected:
    virtual void initializeGL() Q_DECL_OVERRIDE;
    virtual void resizeGL(int w, int h) Q_DECL_OVERRIDE;
    virtual void paintGL() Q_DECL_OVERRIDE;

    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;

private:
    QMatrix4x4 modelMatrix;
    QMatrix4x4 viewMatrix;
    QMatrix4x4 projectionMatrix;

    Camera camera;
    QTime currentTime;
    QTimer timer;
    QOpenGLShaderProgram shader;
    QOpenGLShaderProgram lightShader;
    Model* model{nullptr};
};

#endif // LEARNOPENGLWIDGET_H
