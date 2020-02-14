#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "gl.h"

#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/Sparse>

#include <iostream>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void resizeEvent(QResizeEvent *event);
    void keyPressEvent(QKeyEvent *ke);



private:
    Ui::MainWindow *ui;
    OpenglWidget * GL_w;
    QTimer *tim;

private slots:
    //void resize_window(const QSize& size);


};

#endif // MAINWINDOW_H
