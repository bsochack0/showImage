#ifndef IMAGEPROCESS_H
#define IMAGEPROCESS_H
#include <QGraphicsItem>
#include <QTextBrowser>
#include "CL\cl.h"
#include "OpenCL.h"

class ImageProcess
{
public:
    ImageProcess(QImage &imageProcess, QTextBrowser &infoBox, cl_device_id deviceId);

private:
    QImage &_image;
	QTextBrowser &_infoBox;
	cl_device_id _deviceId;
};

#endif // IMAGEPROCESS_H
