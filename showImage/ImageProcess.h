#ifndef IMAGEPROCESS_H
#define IMAGEPROCESS_H
#include <QGraphicsItem>
#include "CL\cl.h"
#include "OpenCL.h"

class ImageProcess
{
public:
    ImageProcess(QImage *imageProcess, cl_device_id& deviceId);
	int OpenclDeviceId;
private:
    QImage *_image;
	cl_device_id* _deviceId;
    int _width;
    int _height;
};

#endif // IMAGEPROCESS_H
