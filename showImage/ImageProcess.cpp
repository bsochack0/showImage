#include "ImageProcess.h"

ImageProcess::ImageProcess(QImage *imageProcess, cl_device_id& deviceId)
{
    _image = imageProcess;
    _width = _image->width();
    _height = _image->height();
}
