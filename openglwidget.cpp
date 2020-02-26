#include "openglwidget.h"
#include "camera.h"
#include <QtMath>
#include <QVector>
OpenglWidget::OpenglWidget(QOpenGLWidget *parent)
    : QOpenGLWidget(parent), ibo(QOpenGLBuffer::IndexBuffer)
{
    camera = new Camera(QVector3D(0.0f, 0.0f, 4.0f));
    this->setMouseTracking(true);
    this->grabKeyboard();
    lastX = size().width() / 2.0f;
    lastY = size().height() / 2.0f;
    scale = QVector3D(1.0f,1.0f,1.0f);
    makeCurrent();
}

void OpenglWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.0f, 0.05f, 0.05f, 1.0f);

    glEnable(GL_DEPTH_TEST);

    initName(0.0f,0.0f,15.0f,10.0f,5.0f,3.0f);

    initSphere(&vboSphere, &ibo,1.0f,100,100);
    initTor(&vboTor,2.0f,1.0f,60,60);
    initCube(&vboCube,1.0f);
    initCylinder(&vboCylinder,1.0f,3.0f,2.0f,50);
    initCone(&vboCone,1.0f,2.0f,60);
    initTree(5.0f,3.0f);
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
    modelMatrix.translate(QVector3D(x,y,z));
    if(check)
    {

    switch(rotateType)
    {
    case 0:
    {
        q *= QQuaternion::fromAxisAndAngle(QVector3D(1.0f,0.0f,0.0f),0.05f);
        break;
    }
    case 1:
    {
        q *= QQuaternion::fromAxisAndAngle(QVector3D(0.0f,1.0f,0.0f),0.05f);
        break;
    }
    case 2:
    {
        q *= QQuaternion::fromAxisAndAngle(QVector3D(0.0f,0.0f,1.0f),0.05f);
        break;
    }
    }
    }
    if(check || rotState)
        modelMatrix.rotate(q);

    modelMatrix.scale(scale);

    pollEvents();
    shader.bind();
    shader.setUniformValue("viewMatrix", camera->GetViewMatrix());
    shader.setUniformValue("modelMatrix", modelMatrix);
    shader.setUniformValue("projectionMatrix", projectionMatrix);

    switch(type)
    {
    case 0:
    {
        vboSphere.bind();
        int offset = 0;
        int vertLoc = shader.attributeLocation("a_Vertex");
        shader.enableAttributeArray(vertLoc);
        shader.setAttributeBuffer(vertLoc,GL_FLOAT, offset, 3, sizeof(PointData));

        vboSphere.bind();
        ibo.bind();
        if(!filCheck)
            glDrawElements(GL_LINES,ibo.size(),GL_UNSIGNED_INT,0);
        else
            glDrawElements(GL_TRIANGLES,ibo.size(),GL_UNSIGNED_INT,0);
        ibo.release();
        vboSphere.release();
        break;
    }
    case 1:
    {
        vboCylinder.bind();
        int offset = 0;
        int vertLoc = shader.attributeLocation("a_Vertex");
        shader.enableAttributeArray(vertLoc);
        shader.setAttributeBuffer(vertLoc,GL_FLOAT, offset, 3, sizeof(PointData));

        vboCylinder.bind();
        if(!filCheck)
             glDrawArrays(GL_LINES,0,vboCylinder.size());
        else
             glDrawArrays(GL_TRIANGLE_FAN,0,vboCylinder.size());
        vboCylinder.release();
         break;
    }
    case 2:
    {
        vboCube.bind();
        int offset = 0;
        int vertLoc = shader.attributeLocation("a_Vertex");
        shader.enableAttributeArray(vertLoc);
        shader.setAttributeBuffer(vertLoc,GL_FLOAT, offset, 3, sizeof(PointData));

        vboCube.bind();
         if(!filCheck)
             glDrawArrays(GL_LINES,0,vboCone.size());
         else
             glDrawArrays(GL_TRIANGLE_FAN,0,vboCone.size());
        vboCube.release();
         break;
    }
    case 3:
    {
        vboTor.bind();
        int offset = 0;
        int vertLoc = shader.attributeLocation("a_Vertex");
        shader.enableAttributeArray(vertLoc);
        shader.setAttributeBuffer(vertLoc,GL_FLOAT, offset, 3, sizeof(PointData));

        vboTor.bind();
         if(!filCheck)
             glDrawArrays(GL_LINES,0,vboTor.size());
         else
             glDrawArrays(GL_TRIANGLE_STRIP,0,vboTor.size());
        vboTor.release();
         break;
    }
    case 4:
    {
        vboCone.bind();
        int offset = 0;
        int vertLoc = shader.attributeLocation("a_Vertex");
        shader.enableAttributeArray(vertLoc);
        shader.setAttributeBuffer(vertLoc,GL_FLOAT, offset, 3, sizeof(PointData));

        vboCone.bind();
         if(!filCheck)
             glDrawArrays(GL_LINES,0,vboCone.size());
         else
             glDrawArrays(GL_TRIANGLE_FAN,0,vboCone.size());
        vboCone.release();
         break;
    }
    case 5:
    {
        QOpenGLFunctions func;
        func.initializeOpenGLFunctions();
        if(!filCheck)
             treeData.draw(&shader,modelMatrix,&func);
        break;
    }
    }

}

void OpenglWidget::initShaders()
{
    shader.addShaderFromSourceFile(QOpenGLShader::Vertex,"/home/kostikan/Документы/GitHub/QtSphere/vert.vert");
    shader.addShaderFromSourceFile(QOpenGLShader::Fragment,"/home/kostikan/Документы/GitHub/QtSphere/frag.frag");
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


}

void initSphere(QOpenGLBuffer *buff,QOpenGLBuffer *ibo, const float& radius, const int& sectorCount, const int &stackCount)
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


    buff->create();
    buff->bind();
    buff->allocate(pData.constData(),pData.size() * sizeof(PointData));
    buff->release();

    ibo->create();
    ibo->bind();
    ibo->allocate(indices.constData(),indices.size() * sizeof(int));
    ibo->release();
}

void initTor(QOpenGLBuffer *buff,float majorRadius, float minorRadius, int numMajor, int numMinor)
{
    QVector<PointData> pData;
    double majorStep = 2.0f * 3.14159265358979323846 / numMajor;
    double minorStep = 2.0f * 3.14159265358979323846 / numMinor;
    int i, j;

    for (i = 0; i < numMajor; ++i)
    {
            double a0 = i * majorStep;
            double a1 = a0 + majorStep;
            GLfloat x0 = (GLfloat) cos(a0);
            GLfloat y0 = (GLfloat) sin(a0);
            GLfloat x1 = (GLfloat) cos(a1);
            GLfloat y1 = (GLfloat) sin(a1);

        for (j=0; j<=numMinor; ++j)
        {
            double b = j * minorStep;
            GLfloat c = (GLfloat) cos(b);
            GLfloat r = minorRadius * c + majorRadius;
            GLfloat z = minorRadius * (GLfloat) sin(b);

            // Первая точка
            pData.push_back(QVector3D(x0*r, y0*r, z));
            pData.push_back(QVector3D(x1*r, y1*r, z));
        }
    }
    buff->create();
    buff->bind();
    buff->allocate(pData.constData(),pData.size() * sizeof(PointData));
    buff->release();

}

void initCube(QOpenGLBuffer *buff,float width)
{
     QVector<PointData> pData;

     pData.push_back(QVector3D(-width / 2, -width / 2, -width / 2));
     pData.push_back(QVector3D(-width / 2,  width / 2, -width / 2));
     pData.push_back(QVector3D(-width / 2,  width / 2,  width / 2));
     pData.push_back(QVector3D(-width / 2, -width / 2,  width / 2));

     pData.push_back(QVector3D( width / 2, -width / 2, -width / 2));
     pData.push_back(QVector3D( width / 2, -width / 2,  width / 2));
     pData.push_back(QVector3D( width / 2,  width / 2,  width / 2));
     pData.push_back(QVector3D( width / 2,  width / 2, -width / 2));

     pData.push_back(QVector3D(-width / 2, -width / 2, -width / 2));
     pData.push_back(QVector3D(-width / 2, -width / 2,  width / 2));
     pData.push_back(QVector3D( width / 2, -width / 2,  width / 2));
     pData.push_back(QVector3D( width / 2, -width / 2, -width / 2));

     pData.push_back(QVector3D(-width / 2, width/ 2, -width/ 2));
     pData.push_back(QVector3D(-width / 2, width/ 2,  width/ 2));
     pData.push_back(QVector3D( width / 2, width/ 2,  width/ 2));
     pData.push_back(QVector3D( width / 2, width/ 2, -width/ 2));

     pData.push_back(QVector3D(-width / 2, -width / 2, -width / 2));
     pData.push_back(QVector3D( width / 2, -width / 2, -width / 2));
     pData.push_back(QVector3D( width / 2,  width / 2, -width / 2));
     pData.push_back(QVector3D(-width / 2,  width / 2, -width / 2));

     pData.push_back(QVector3D(-width / 2, -width / 2,  width / 2));
     pData.push_back(QVector3D( width / 2, -width / 2,  width / 2));
     pData.push_back(QVector3D( width / 2,  width / 2,  width / 2));
     pData.push_back(QVector3D(-width / 2,  width / 2,  width / 2));

     pData.push_back(QVector3D( width / 2,  -width / 2,  width / 2));
     pData.push_back(QVector3D( width / 2,  width / 2,  width / 2));
     pData.push_back(QVector3D( width / 2,  -width / 2,  -width / 2));
     pData.push_back(QVector3D(width / 2,  width / 2, - width / 2));


     buff->create();
     buff->bind();
     buff->allocate(pData.constData(),pData.size() * sizeof(PointData));
     buff->release();
}

void initCylinder(QOpenGLBuffer *buff,float radius, float height, float halfLength,int slices)
{
    QVector<PointData> pData;
    for(int i=0; i<slices; i++)
    {
        float seta = i*360.0/slices;
        float vx = sin(qDegreesToRadians(seta))*radius;
        float vy = cos(qDegreesToRadians(seta))*radius;

        pData.push_back(QVector3D(vx, vy,  -height));
        pData.push_back(QVector3D(vx, vy,   height));

    }
    for(int i=0; i<slices; i++)
    {
        float seta = i*360.0/slices;
        float seta1 = (i +1)*360.0/slices;
        float vx = sin(qDegreesToRadians(seta))*radius;
        float vy = cos(qDegreesToRadians(seta))*radius;

        float vx1 = sin(qDegreesToRadians(seta1))*radius;
        float vy1 = cos(qDegreesToRadians(seta1))*radius;

        pData.push_back(QVector3D(vx, vy,  height));
        pData.push_back(QVector3D(vx1, vy1,   height));

        pData.push_back(QVector3D(vx, vy,  -height));
        pData.push_back(QVector3D(vx1, vy1,   -height));

    }
    buff->create();
    buff->bind();
    buff->allocate(pData.constData(),pData.size() * sizeof(PointData));
    buff->release();
}

void initCone(QOpenGLBuffer *buff,float radius, float height,float slices)
{
    QVector<PointData> pData;

    for (int i = 0; i < slices; i++)
    {
        float seta = i*360.0/slices;
         pData.push_back(QVector3D(sin((qDegreesToRadians(seta))) * radius, cos((qDegreesToRadians(seta))) * radius,0.0f ));
         pData.push_back(QVector3D(0, 0, height));
    }

    for (int i = 0; i < slices; i++)
    {
        float seta = i*360.0/slices;
        float seta1 = (i +1)*360.0/slices;
        pData.push_back(QVector3D(sin((qDegreesToRadians(seta))) * radius, cos((qDegreesToRadians(seta))) * radius,0.0f ));
        pData.push_back(QVector3D(sin((qDegreesToRadians(seta1))) * radius, cos((qDegreesToRadians(seta1))) * radius,0.0f ));
    }

    buff->create();
    buff->bind();
    buff->allocate(pData.constData(),pData.size() * sizeof(PointData));
    buff->release();

}

void OpenglWidget::initTree(float width, float height)
{
    treeData.width = width;
    treeData.height = height;
    treeData.init();
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
         if(ikey > 1024)
             return;
         keys[ikey] = true;
     }
     this->update();
}

void OpenglWidget::keyReleaseEvent(QKeyEvent *event)
{
    int ikey = event->key();
    if(ikey > 1024)
        return;
    keys[ikey] = false;
    this->update();
}

void OpenglWidget::pollEvents()
{
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

void OpenglWidget::indexChange(int index)
{
    type = index;
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
    q = QQuaternion();
    rotState = false;
    scale = QVector3D(1.0f,1.0f,1.0f);
}

void OpenglWidget::moveX(int _x)
{

    x = _x/float(1000);
}

void OpenglWidget::moveY(int _y)
{
    y = _y/float(1000);
}

void OpenglWidget::moveZ(int _z)
{
    z = _z/float(1000);
}

void OpenglWidget::zoom(int z)
{
   float t = z/float(100);
   scale = QVector3D(t,t,t);
}

void OpenglWidget::angle(int ang)
{
    rotState = true;
    switch(rotateType)
    {
    case 0:
    {
        q = QQuaternion::fromAxisAndAngle(QVector3D(1.0f,0.0f,0.0f),float(ang));
        break;
    }
    case 1:
    {
        q = QQuaternion::fromAxisAndAngle(QVector3D(0.0f,1.0f,0.0f),float(ang));
        break;
    }
    case 2:
    {
        q = QQuaternion::fromAxisAndAngle(QVector3D(0.0f,0.0f,1.0f),float(ang));
        break;
    }
    }

}

void OpenglWidget::RotIndexChange(int index)
{
    rotateType = index;
}

void OpenglWidget::upCheck(int ch)
{
    check = ch;
    q = QQuaternion();
}

void OpenglWidget::upFilled(int ch)
{
    filCheck = ch;
}
