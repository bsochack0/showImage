#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QFileDialog>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "imageprocess.h"
#include "CL\cl.h"
#include "OpenCL.h"
#include "OpenClDeviceInfo.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);
    showMaximized();
    _ui->imageArea->setScene(&_scene);
    _ui->imageArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _ui->imageArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	loadOpenCLDevices();
	
	_ui->comboBox->adjustSize();
}

MainWindow::~MainWindow()
{
    delete _ui;
}

void MainWindow::loadOpenCLDevices()
{
	OpenCL opencl;

	opencl.getOpenCLDeviceList(_devices);

	int devicesCount = _devices.size();

	for (int i = 0; i < devicesCount; i++)
	{
		_ui->comboBox->addItem(_devices[i]._deviceName);
	}
}

void MainWindow::on_openImage_clicked()
{

}

void MainWindow::on_actionOpen_triggered()
{
    QString imagePath = QFileDialog::getOpenFileName(this,
                                             tr("Open File"),
                                             "",
                                             tr("Files (*.bmp)")
                                             );

    QPixmap imageTmp;
	if(!imageTmp.load(imagePath))
	{
		_ui->infoBox->setHtml("Image load failed: " + imagePath);
		return;
	}

	_ui->infoBox->setHtml("<strong>Image loaded</strong>");

    _imagePreview = new QGraphicsPixmapItem(
                imageTmp.scaled(QSize(800,600))
    );

    _scene.addItem(_imagePreview);

    _image = imageTmp.toImage();
}

void MainWindow::processImage()
{
	int deviceNumber = _ui->comboBox->currentIndex();
	ImageProcess imageProcessor(_image, *(_ui->infoBox), _devices[deviceNumber]._deviceId);

	QPixmap imageTmp = QPixmap::fromImage(_image);
	_imagePreview->setPixmap(imageTmp.scaled(800,600));

	 _scene.update();
}
