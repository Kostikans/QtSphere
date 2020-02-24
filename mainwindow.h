#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QComboBox>
#include "openglwidget.h"
#include <QSlider>
#include <QLabel>
#include <QCheckBox>


class MainWindow :  public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    const int MAX_VALUE{10000};
    QHBoxLayout *horizonalLayout;
    QVBoxLayout *verticalLayout;
    QOpenGLWidget *openglWidget;
    QComboBox box;
    QComboBox RotBox;
    QCheckBox checkBox;
    QCheckBox checkFilled;

    QLabel  *x;
    QLabel  *y;
    QLabel  *z;
    QLabel  *zoom;
    QLabel  *angle;

    QSlider *xSlider;
    QSlider *ySlider;
    QSlider *zSlider;

    QSlider *zoomSlider;
    QSlider *angleSlider;

    QHBoxLayout *hlaySliders;
    QHBoxLayout *hlaybelSliders;

    void connectSliders();
public slots:
    void reroll(int a);
};

#endif // MAINWINDOW_H
