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


void initSphere(QOpenGLBuffer *buff,QOpenGLBuffer *ibo, const float& radius, const int& sectorCount, const int &stackCount);
void initTor(QOpenGLBuffer *buff,float majorRadius, float minorRadius, int numMajor, int numMinor);
void initCube(QOpenGLBuffer *buff,float width);
void initCylinder(QOpenGLBuffer *buff,float radius, float height, float halfLength,int slices);
void initCone(QOpenGLBuffer *buff,float radius, float height,float slices);

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

class Tree{
   QOpenGLBuffer vboCylinder;
   QOpenGLBuffer vboCone1;
   QOpenGLBuffer vboCone2;
   QOpenGLBuffer vboCone3;
public:
   float height;
   float width;
   QVector3D t1;
   QVector3D t2;
   QVector3D t3;
   QVector3D t4;

public:
   void init()
   {
       initCylinder(&vboCylinder,width/7.0f,height/5.0f,2.0f,50);
       initCone(&vboCone1,width/2.0f,height,50);
       initCone(&vboCone2,width/3.0f,height,50);
       initCone(&vboCone3,width/4.0f,height,50);
       t1 = QVector3D(0.0f,0.0f,-0.5f);
       t2 = QVector3D(0.0f,0.0f,0.0f);
       t3 = QVector3D(0.0f,0.0f,1.5f);
       t4 = QVector3D(0.0f,0.0f,3.0f);
   }
   void draw(QOpenGLShaderProgram *shader,QMatrix4x4 model,QOpenGLFunctions *func,bool state)
   {
       vboCylinder.bind();
       shader->bind();
       QMatrix4x4 mod = model;
       int offset = 0;
       int vertLoc = shader->attributeLocation("a_Vertex");
       shader->enableAttributeArray(vertLoc);
       shader->setAttributeBuffer(vertLoc,GL_FLOAT, offset, 3, sizeof(PointData));
       mod.translate(t1);
       shader->setUniformValue("modelMatrix", mod);


       if(!state)
            func->glDrawArrays(GL_LINES,0,vboCylinder.size());
       else
            func->glDrawArrays(GL_TRIANGLE_FAN,0,vboCylinder.size());
       vboCylinder.release();

       mod = model;
       vboCone1.bind();
       shader->bind();
       int vertLoc1 = shader->attributeLocation("a_Vertex");
       shader->enableAttributeArray(vertLoc1);
       shader->setAttributeBuffer(vertLoc1,GL_FLOAT, offset, 3, sizeof(PointData));
       qDebug()<< t3.z();
       mod.translate(t2);
       shader->setUniformValue("modelMatrix", mod);


       if(!state)
           func->glDrawArrays(GL_LINES,0,vboCone1.size());
       else
           func->glDrawArrays(GL_TRIANGLE_FAN,0,vboCone1.size());

       vboCone1.release();

       mod = model;
       vboCone2.bind();
       shader->bind();
       int vertLoc2 = shader->attributeLocation("a_Vertex");
       shader->enableAttributeArray(vertLoc2);
       shader->setAttributeBuffer(vertLoc2,GL_FLOAT, offset, 3, sizeof(PointData));
       mod.translate(t3);
       shader->setUniformValue("modelMatrix", mod);

       if(!state)
           func->glDrawArrays(GL_LINES,0,vboCone2.size());
       else
           func->glDrawArrays(GL_TRIANGLE_FAN,0,vboCone2.size());
       vboCone2.release();

        mod = model;
       vboCone3.bind();
       shader->bind();
       int vertLoc3 = shader->attributeLocation("a_Vertex");
       shader->enableAttributeArray(vertLoc3);
       shader->setAttributeBuffer(vertLoc3,GL_FLOAT, offset, 3, sizeof(PointData));
       mod.translate(t4);
       shader->setUniformValue("modelMatrix",mod);

       if(!state)
           func->glDrawArrays(GL_LINES,0,vboCone3.size());
       else
           func->glDrawArrays(GL_TRIANGLE_FAN,0,vboCone3.size());
       vboCone3.release();
   }
};

class Snegovik{
   QOpenGLBuffer vboSphere1;
   QOpenGLBuffer vboSphere2;
   QOpenGLBuffer vboSphere3;
   QOpenGLBuffer vboCone1;
   QOpenGLBuffer *ibo1;
   QOpenGLBuffer *ibo2;
   QOpenGLBuffer *ibo3;
public:
   float radius;
   QVector3D t1;
   QVector3D t2;
   QVector3D t3;
   QVector3D t4;

public:
   void init()
   {
       ibo1 = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
       ibo2 = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
       ibo3 = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
       initSphere(&vboSphere1,ibo1,radius,100,100);
       initSphere(&vboSphere2,ibo2,radius/1.3f,100,100);
       initSphere(&vboSphere3,ibo3,radius/2.0f,100,100);
       initCone(&vboCone1,radius/10.0f,radius/4.0f,50);

       t1 = QVector3D(0.0f,0.0f,0.0f);
       t2 = QVector3D(0.0f,0.0f,1.5f);
       t3 = QVector3D(0.0f,0.0f,2.6f);
       t4 = QVector3D(0.0f,-radius/2.1f,2.6f);
   }
   void draw(QOpenGLShaderProgram *shader,QMatrix4x4 model,QOpenGLFunctions *func,bool state)
   {
       vboSphere1.bind();
       shader->bind();
       QMatrix4x4 mod = model;
       int offset = 0;
       int vertLoc = shader->attributeLocation("a_Vertex");
       shader->enableAttributeArray(vertLoc);
       shader->setAttributeBuffer(vertLoc,GL_FLOAT, offset, 3, sizeof(PointData));
       mod.translate(t1);
       shader->setUniformValue("modelMatrix", mod);

       vboSphere1.bind();
       ibo1->bind();
       if(!state)
           func->glDrawElements(GL_LINES,ibo1->size(),GL_UNSIGNED_INT,0);
       else
           func->glDrawElements(GL_TRIANGLES,ibo1->size(),GL_UNSIGNED_INT,0);
       ibo1->release();
       vboSphere1.release();

       mod = model;
       vboSphere2.bind();
       shader->bind();
       int vertLoc1 = shader->attributeLocation("a_Vertex");
       shader->enableAttributeArray(vertLoc1);
       shader->setAttributeBuffer(vertLoc1,GL_FLOAT, offset, 3, sizeof(PointData));
       mod.translate(t2);
       shader->setUniformValue("modelMatrix", mod);

       vboSphere2.bind();
       ibo2->bind();
       if(!state)
           func->glDrawElements(GL_LINES,ibo2->size(),GL_UNSIGNED_INT,0);
       else
           func->glDrawElements(GL_TRIANGLES,ibo2->size(),GL_UNSIGNED_INT,0);
       ibo2->release();
       vboSphere2.release();

       mod = model;
       vboSphere3.bind();
       shader->bind();
       int vertLoc2 = shader->attributeLocation("a_Vertex");
       shader->enableAttributeArray(vertLoc2);
       shader->setAttributeBuffer(vertLoc2,GL_FLOAT, offset, 3, sizeof(PointData));
       mod.translate(t3);
       shader->setUniformValue("modelMatrix", mod);

       vboSphere3.bind();
       ibo3->bind();
       if(!state)
           func->glDrawElements(GL_LINES,ibo3->size(),GL_UNSIGNED_INT,0);
       else
           func->glDrawElements(GL_TRIANGLES,ibo3->size(),GL_UNSIGNED_INT,0);
       ibo3->release();
       vboSphere3.release();

        mod = model;
       vboCone1.bind();
       shader->bind();
       int vertLoc3 = shader->attributeLocation("a_Vertex");
       shader->enableAttributeArray(vertLoc3);
       shader->setAttributeBuffer(vertLoc3,GL_FLOAT, offset, 3, sizeof(PointData));
       mod.translate(t4);
       QQuaternion q;
       q = QQuaternion::fromAxisAndAngle(QVector3D(1.0f,0.0f,0.0f),90.0f);
       mod.rotate(q);
       shader->setUniformValue("modelMatrix",mod);

       if(!state)
           func->glDrawArrays(GL_LINES,0,vboCone1.size());
       else
           func->glDrawArrays(GL_TRIANGLE_FAN,0,vboCone1.size());
       vboCone1.release();
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
    void initTree(float width, float height);
    void initSnegovik(float radius);

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
    void upFilled(int ch);

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
    bool filCheck = false;

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
    float radius;

    Tree treeData;
    Snegovik snegovik;


};


#endif // OPENGLWIDGET_H
