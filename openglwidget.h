#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QOpenGLBuffer>
#include <QOpenGLShader>
#include <QQuaternion>

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
    Q_OBJECT
public:
    explicit OpenglWidget(QOpenGLWidget *parent = nullptr);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void initShaders();
    void initName(int x0,int y0, int h, int w, int d,int d2);
    void initSphere(const float& radius, const int& sectorCount, const int &stackCount);
    void initTor(float majorRadius, float minorRadius, int numMajor, int numMinor);
    void initCube(float width);
    void initCylinder(float radius, float height, float halfLength,int slices);
    void initCone(float radius, float height,float slices);

    void mouseMoveEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

    void pollEvents();
public slots:
    void indexChange(int index);
    void moveX(int _x);
    void moveY(int _y);
    void moveZ(int _z);

    void zoom(int z);
    void angle(int ang);

    void RotIndexChange(int index);

    void upCheck(int ch);

private:

    QMatrix4x4 projectionMatrix;
    QQuaternion q;
    float x{0},y{0},z{0};
    QVector3D scale;
    QOpenGLShaderProgram shader;
    Camera *camera;
    int lastX;
    int lastY;
    bool firstMouse{true};

    bool check = false;
    bool rotState = false;

    QOpenGLBuffer vboSphere;
    QOpenGLBuffer vboTor;
    QOpenGLBuffer vboCube;
    QOpenGLBuffer vboCylinder;
    QOpenGLBuffer vboCone;


    QOpenGLBuffer ibo;
    bool keys[4024] = { false };
    float deltaTime = 1 / 60.0f;

    int type = 0;
    int rotateType = 0;

};


#endif // OPENGLWIDGET_H
