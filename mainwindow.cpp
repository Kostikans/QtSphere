#include "mainwindow.h"
#include <QObject>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    horizonalLayout = new QHBoxLayout();
    verticalLayout = new QVBoxLayout();
    hlaybelSliders = new QHBoxLayout();
    hlaySliders = new QHBoxLayout();
    openglWidget = new OpenglWidget();

    xSlider = new QSlider(Qt::Vertical);
    ySlider = new QSlider(Qt::Vertical);
    zSlider = new QSlider(Qt::Vertical);

    angleSlider = new QSlider(Qt::Vertical);
    zoomSlider = new QSlider(Qt::Vertical);

    x = new QLabel("x");
    y = new QLabel("y");
    z = new QLabel("z");
    zoom = new QLabel("zoom");
    angle = new QLabel("angle");

    checkBox.setTristate(false);

    horizonalLayout->addWidget(openglWidget);
    this->setLayout(horizonalLayout);
    openglWidget->show();
    this->resize(1000,800);

    RotBox.addItem("x");
    RotBox.addItem("y");
    RotBox.addItem("z");

    box.addItem("Sphere");
    box.addItem("Cylinder");
    box.addItem("Cube");
    box.addItem("Tor");
    box.addItem("Cone");
    box.addItem("Tree");

    connect(&box, SIGNAL( currentIndexChanged(int)), openglWidget, SLOT(indexChange(int)));
    connect(&RotBox, SIGNAL( currentIndexChanged(int)), openglWidget, SLOT(RotIndexChange(int)));
    connect(&checkBox, SIGNAL( stateChanged(int)), openglWidget, SLOT(upCheck(int)));
    connect(&checkFilled, SIGNAL( stateChanged(int)), openglWidget, SLOT(upFilled(int)));

    verticalLayout->addWidget(&RotBox);
    verticalLayout->addWidget(&box);
    verticalLayout->addWidget(&checkBox);
    verticalLayout->addWidget(&checkFilled);


    xSlider->setValue(0);
    xSlider->setRange(-MAX_VALUE,MAX_VALUE);

    ySlider->setValue(0);
    ySlider->setRange(-MAX_VALUE,MAX_VALUE);

    zSlider->setValue(0);
    zSlider->setRange(-MAX_VALUE,MAX_VALUE);

    zoomSlider->setValue(100);
    zoomSlider->setRange(0,200);
    angleSlider->setValue(0);
    angleSlider->setRange(-360,360);

    hlaySliders->addWidget(xSlider);
    hlaySliders->addWidget(ySlider);
    hlaySliders->addWidget(zSlider);

    hlaySliders->addWidget(angleSlider);
    hlaySliders->addWidget(zoomSlider);

    hlaybelSliders->addWidget(x);
    hlaybelSliders->addWidget(y);
    hlaybelSliders->addWidget(z);
    hlaybelSliders->addWidget(angle);
    hlaybelSliders->addWidget(zoom);

    verticalLayout->addLayout(hlaySliders);
    verticalLayout->addLayout(hlaybelSliders);

    horizonalLayout->addLayout(verticalLayout);

    connectSliders();
    connect(&box, SIGNAL( currentIndexChanged(int)), this, SLOT(reroll(int)));

}

void MainWindow::connectSliders()
{
    connect(xSlider,SIGNAL(valueChanged(int)),openglWidget,SLOT(moveX(int)));
    connect(ySlider,SIGNAL(valueChanged(int)),openglWidget,SLOT(moveY(int)));
    connect(zSlider,SIGNAL(valueChanged(int)),openglWidget,SLOT(moveZ(int)));

    connect(angleSlider,SIGNAL(valueChanged(int)),openglWidget,SLOT(angle(int)));
    connect(zoomSlider,SIGNAL(valueChanged(int)),openglWidget,SLOT(zoom(int)));
}

void MainWindow::reroll(int a)
{
     xSlider->setValue(0);
     ySlider->setValue(0);
     zSlider->setValue(0);
     zoomSlider->setValue(100);
     angleSlider->setValue(0);
}
