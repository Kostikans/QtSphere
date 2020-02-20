#include "openglwidget.h"
#include "camera.h"
#include <QtMath>
#include <QVector>
OpenglWidget::OpenglWidget(QOpenGLWidget *parent)
    : QOpenGLWidget(parent), ibo(QOpenGLBuffer::IndexBuffer)
{
    camera = new Camera(QVector3D(0.0f, 0.0f, 4.0f));
    this->setMouseTracking(true);
    lastX = size().width() / 2.0f;
    lastY = size().height() / 2.0f;

    makeCurrent();
}

void OpenglWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.0f, 0.05f, 0.05f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    //initSphere(1.0f,100,100);
    initName(0.0f,0.0f,15.0f,10.0f,5.0f,3.0f);
    initShaders();
}

void OpenglWidget::resizeGL(int w, int h)
{
    projectionMatrix.setToIdentity();
    projectionMatrix.perspective(45.0f, w / float(h), 0.01f, 100.0f);
}

void OpenglWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 modelMatrix;
    modelMatrix.setToIdentity();
    modelMatrix.translate(m_translate);


    pollEvents();
    shader.bind();
    shader.setUniformValue("viewMatrix", camera->GetViewMatrix());
    shader.setUniformValue("modelMatrix", modelMatrix);
    shader.setUniformValue("projectionMatrix", projectionMatrix);

    vbo.bind();
    int offset = 0;
    int vertLoc = shader.attributeLocation("a_Vertex");
    shader.enableAttributeArray(vertLoc);
    shader.setAttributeBuffer(vertLoc,GL_FLOAT, offset, 3, sizeof(PointData));

    //ibo.bind();
    glLineWidth(5);
    glDrawArrays(GL_LINES,0,vbo.size());
    vbo.release();
}

void OpenglWidget::initShaders()
{
    shader.addShaderFromSourceFile(QOpenGLShader::Vertex,"C:/Users/ea56/Documents/QtSphere/vert.vert");
    shader.addShaderFromSourceFile(QOpenGLShader::Fragment,"C:/Users/ea56/Documents/QtSphere/frag.frag");
    shader.link();
}

void OpenglWidget::initName(int x0,int y0, int h, int w, int d1,int d2)
{
    QVector<PointData> pData;
    //P
    int coeff = x0;
    pData.append(QVector3D(coeff,y0,0.0f));
    pData.append(QVector3D(coeff,y0+h,0.0f));
    pData.append(QVector3D(coeff + w,y0 + h ,0.0f));
    pData.append(QVector3D(coeff + w,y0,0.0f));

    pData.append(QVector3D(coeff + d2,y0 ,0.0f));
    pData.append(QVector3D(coeff + d2,y0 + h - d2 ,0.0f));
    pData.append(QVector3D(coeff + w - d2,y0 + h -d2,0.0f));
    pData.append(QVector3D(coeff + w - d2,y0 ,0.0f));

    pData.append(QVector3D(coeff ,y0 + h ,0.0f));
    pData.append(QVector3D(coeff + w,y0 + h ,0.0f));
    pData.append(QVector3D(coeff + d2,y0 + h  - d2,0.0f));
    pData.append(QVector3D(coeff + w - d2,y0 + h -d2 ,0.0f));

    pData.append(QVector3D(coeff ,y0  ,0.0f));
    pData.append(QVector3D(coeff + d2,y0  ,0.0f));
    pData.append(QVector3D(coeff + w,y0 ,0.0f));
    pData.append(QVector3D(coeff + w - d2,y0  ,0.0f));


    // K
    coeff += (w + d1);
    pData.append(QVector3D(coeff     ,y0,0.0f));
    pData.append(QVector3D(coeff     ,y0+h,0.0f));

    pData.append(QVector3D(coeff + d2,y0 ,0.0f));
    pData.append(QVector3D(coeff + d2,y0 + h/2 - h/5,0.0f));
    pData.append(QVector3D(coeff + d2,y0 + h/2 + h/5,0.0f));
    pData.append(QVector3D(coeff + d2,y0 + h,0.0f));

    pData.append(QVector3D(coeff     ,y0,0.0f));
    pData.append(QVector3D(coeff + d2     ,y0,0.0f));
    pData.append(QVector3D(coeff     ,y0 + h,0.0f));
    pData.append(QVector3D(coeff +d2   ,y0 + h,0.0f));

    pData.append(QVector3D(coeff + d2,y0 + h/2 ,0.0f));
    pData.append(QVector3D(coeff + w , y0 + h ,0.0f));
    pData.append(QVector3D(coeff + d2,y0 + h/2,0.0f));
    pData.append(QVector3D(coeff + w ,y0 ,0.0f));

    pData.append(QVector3D(coeff + d2 ,y0 + h/2 + h/5 ,0.0f));
    pData.append(QVector3D(coeff + w - d2,y0 + h ,0.0f));
    pData.append(QVector3D(coeff + d2,y0 + h/2 - h/5,0.0f));
    pData.append(QVector3D(coeff + w - d2,y0 ,0.0f));

    pData.append(QVector3D(coeff + w ,y0 + h ,0.0f));
    pData.append(QVector3D(coeff + w - d2,y0 + h ,0.0f));
    pData.append(QVector3D(coeff + w,y0,0.0f));
    pData.append(QVector3D(coeff + w - d2,y0 ,0.0f));

    //G
    coeff += (w + d1);
    pData.append(QVector3D(coeff , y0, 0.0f));
    pData.append(QVector3D(coeff , y0  + h ,0.0f));
    pData.append(QVector3D(coeff + w , y0  + h ,0.0f));
    pData.append(QVector3D(coeff + w, y0 + h - h/6 ,0.0f));

    pData.append(QVector3D(coeff  , y0  + h ,0.0f));
    pData.append(QVector3D(coeff + w, y0 + h ,0.0f));
    pData.append(QVector3D(coeff + w, y0 + h - h/6 ,0.0f));
    pData.append(QVector3D(coeff + d2, y0 + h - h/6 ,0.0f));

    pData.append(QVector3D(coeff + d2, y0  ,0.0f));
    pData.append(QVector3D(coeff + d2, y0 + h - h/6 ,0.0f));
    pData.append(QVector3D(coeff , y0  ,0.0f));
    pData.append(QVector3D(coeff + d2, y0  ,0.0f));

    vbo.create();
    vbo.bind();
    vbo.allocate(pData.constData(),pData.size() * sizeof(PointData));
    vbo.release();
}

void OpenglWidget::initSphere(const float& radius, const int& sectorCount, const int &stackCount)
{

    QVector<PointData> pData;
    float x, y, z, xy;
    float nx, ny, nz, lengthInv = 1.0f / radius;

    float sectorStep = 2 * M_PI / sectorCount;
    float stackStep = M_PI / stackCount;
    float sectorAngle, stackAngle;

    for(int i = 0; i <= stackCount; ++i)
    {
        stackAngle = M_PI / 2 - i * stackStep;
        xy = radius * cosf(stackAngle);
        z = radius * sinf(stackAngle);

        for(int j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep;

            x = xy * cosf(sectorAngle);
            y = xy * sinf(sectorAngle);

            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            pData.push_back(PointData(QVector3D(x,y,z),QVector3D(nx,ny,nz)));

        }

    }

    QVector<int> indices;
    int k1, k2;
    for(int i = 0; i < stackCount; ++i)
    {
        k1 = i * (sectorCount + 1);
        k2 = k1 + sectorCount + 1;

        for(int j = 0; j < sectorCount; ++j, ++k1, ++k2)
        {
            if(i != 0)
            {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            if(i != (stackCount-1))
            {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }


    vbo.create();
    vbo.bind();
    vbo.allocate(pData.constData(),pData.size() * sizeof(PointData));
    vbo.release();

    ibo.create();
    ibo.bind();
    ibo.allocate(indices.constData(),indices.size() * sizeof(int));
    ibo.release();
}


void OpenglWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (firstMouse)
    {
        lastX = event->localPos().x();
        lastY = event->localPos().y();
        firstMouse = false;
    }

    float xoffset = event->localPos().x() - lastX;
    float yoffset = lastY - event->localPos().y();

    lastX = event->localPos().x();
    lastY = event->localPos().y();

    if (event->buttons() == Qt::LeftButton)
    {
        camera->ProcessMouseMovement(xoffset, yoffset);
        this->update();
    }
}

void OpenglWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->type() == QKeyEvent::KeyPress)
        {
            int ikey = event->key();

            keys[ikey] = true;
        }
        this->update();
}

void OpenglWidget::keyReleaseEvent(QKeyEvent *event)
{
    int ikey = event->key();
    keys[ikey] = false;
    this->update();
}

void OpenglWidget::pollEvents()
{
    if (keys[Qt::Key_T] == true)
    {
        m_translate += QVector3D(0.0f,0.05f,0.0f);
    }
    if (keys[Qt::Key_G] == true)
    {
        m_translate += QVector3D(0.0f,-0.05f,0.0f);
    }
    if (keys[Qt::Key_F] == true)
    {
        m_translate += QVector3D(-0.05f,0.0f,0.0f);
    }
    if (keys[Qt::Key_H] == true)
    {
        m_translate += QVector3D(0.05f,0.0f,0.0f);
    }
    if (keys[Qt::Key_W] == true)
    {
        camera->ProcessKeyboard(FORWARD, deltaTime);
    }
    if (keys[Qt::Key_S] == true)
    {
        camera->ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (keys[Qt::Key_A] == true)
    {
        camera->ProcessKeyboard(LEFT, deltaTime);
    }
    if (keys[Qt::Key_D] == true)
    {
        camera->ProcessKeyboard(RIGHT, deltaTime);

    }
    this->update();
}
