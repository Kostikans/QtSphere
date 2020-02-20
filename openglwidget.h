#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QOpenGLBuffer>
#include <QOpenGLShader>
class Camera;

struct PointData
{
    QVector3D vertex;
    QVector3D normal;
    PointData(QVector3D &&vert,QVector3D &&norm)
    {
        vertex = std::move(vert);
        normal = std::move(norm);
    }
    PointData(QVector3D &&vert)
    {
        vertex = std::move(vert);
    }
};

class OpenglWidget : public QOpenGLWidget , public QOpenGLFunctions
{
public:
    explicit OpenglWidget(QOpenGLWidget *parent = nullptr);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void initShaders();
    void initName(int x0,int y0, int h, int w, int d,int d2);
    void initSphere(const float& radius, const int& sectorCount, const int &stackCount);

    void mouseMoveEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

    void pollEvents();
private:
    QMatrix4x4 projectionMatrix;
    QVector3D m_translate;
    QOpenGLShaderProgram shader;
    Camera *camera;
    int lastX;
    int lastY;
    bool firstMouse{true};

    QOpenGLBuffer vbo;
    QOpenGLBuffer ibo;
    bool keys[4024] = { false };
    float deltaTime = 1 / 60.0f;
};

#endif // OPENGLWIDGET_H
