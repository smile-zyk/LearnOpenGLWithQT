#include "learnopenglwidget.h"
#include <QDebug>
int lastFrameTime;
int deltaTime;

QPoint lastPos;
QPoint currentPos;
Qt::MouseButtons currentBtns;

QVector3D lightPos(0.0f, 0.0f, 3.0f);
QVector3D lightColor(1.0f, 1.0f, 1.0f);

LearnOpenGLWidget::LearnOpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    QSurfaceFormat format;
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    setFormat(format);
    connect(&timer,&QTimer::timeout,this,[this](){update();});
    timer.start(1);
    currentTime.start();
    state = RUN;
}

LearnOpenGLWidget::~LearnOpenGLWidget()
{
    makeCurrent();
    doneCurrent();
}

void LearnOpenGLWidget::LoadModel(QString path)
{
    if(model != nullptr)
        delete model;
    model = nullptr;
    makeCurrent();
    model = new Model(QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>(), path);
    doneCurrent();
}

void LearnOpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    camera.aspect = (float)width()/height();
    shader.create();
    shader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/vertex.glsl");
    shader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/fragment.glsl");
    shader.link();
    glClearColor(0.2f,0.3f,0.3f,1.0f);
    glEnable(GL_DEPTH_TEST);
}

void LearnOpenGLWidget::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
}

void LearnOpenGLWidget::paintGL()
{
    if(model == nullptr) return ;
    qDebug() << camera.position;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if(state == RUN)
    {
        // calculate fps
        int nowTime = currentTime.elapsed();
        if(lastFrameTime != 0)
        {
            deltaTime = nowTime - lastFrameTime;
            emit debugMessage(QStringLiteral("fps: ")+QString::number(1000.0 / deltaTime));
        }
        lastFrameTime = nowTime;
        {
            // process input
            if((currentBtns & Qt::LeftButton) || (currentBtns & Qt::RightButton))
            {
                if(!lastPos.isNull())
                {
                    QVector2D motion = {(float)(currentPos - lastPos).x(),(float)(currentPos - lastPos).y()};
                    motion.setX(motion.x()/width());
                    motion.setY(motion.y()/height());
                    if(currentBtns & Qt::LeftButton)
                    {
                        camera.RotateAroundTarget(motion);
                    }
                    if(currentBtns & Qt::RightButton)
                    {
                        camera.MoveTarget(motion);
                    }
                }
                lastPos = currentPos;
            }
            // calculate  matrix
//            float rotateDegree = deltaTime / 16.0f;
//            modelMatrix.rotate(rotateDegree, 1, 0, 0);
//            modelMatrix.rotate(rotateDegree, 0, 1, 0);
//            modelMatrix.rotate(rotateDegree, 0, 0, 1);
            viewMatrix = camera.GetViewMatrix();
            projectionMatrix = camera.GetProjectionMatrix();
        }
    }
    else
    {
        lastFrameTime = 0;
    }
    // render display box
    shader.bind();
    shader.setUniformValue("model", modelMatrix);
    shader.setUniformValue("view", viewMatrix);
    shader.setUniformValue("projection", projectionMatrix);
    shader.setUniformValue("lightColor", lightColor);
    shader.setUniformValue("lightPos", lightPos);
    shader.setUniformValue("viewPos", camera.position);
    shader.setUniformValue("material.shininess", 32.0f);
    QVector3D diffuseColor = lightColor * 0.9f;
    QVector3D ambientColor = lightColor * 0.6f;
    shader.setUniformValue("light.ambient",  ambientColor);
    shader.setUniformValue("light.diffuse",  diffuseColor);
    shader.setUniformValue("light.specular", lightColor);
    model->Draw(shader);
    shader.release();
}

void LearnOpenGLWidget::mousePressEvent(QMouseEvent *event)
{
    currentBtns = event->buttons();
    currentPos = event->pos();
    lastPos = {0,0};
}

void LearnOpenGLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    currentBtns = event->buttons();
}

void LearnOpenGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    currentPos = event->pos();
}

void LearnOpenGLWidget::wheelEvent(QWheelEvent *event)
{
    QPoint numPixels = event->pixelDelta();
    QPoint numDegrees = event->angleDelta() / 8;

    QPoint res;
    if (!numPixels.isNull()) {
        res = numPixels;
    } else if (!numDegrees.isNull()) {
        QPoint numSteps = numDegrees / 15;
        res = numSteps;
    }
    camera.CloseToTarget(res.y());
}
