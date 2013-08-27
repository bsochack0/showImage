#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include "OpenClDeviceInfo.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_openImage_clicked();
    void on_actionOpen_triggered();
	void processImage();

private:
    Ui::MainWindow *_ui;
    QGraphicsScene *_scene;
    QGraphicsPixmapItem *_imagePreview;
    QString _imagePath;
	QImage _image;
	void loadOpenCLDevices();
	std::vector<OpenClDeviceInfo> _devices;
};

#endif // MAINWINDOW_H
