
#include "CL\cl.h"
#include <Qt\qstring.h>

#ifndef OPENCL_DEVICE_INFO_H
#define OPENCL_DEVICE_INFO_H

class OpenClDeviceInfo
{
public:
	OpenClDeviceInfo(cl_device_id deviceId, cl_platform_id platformId, QString deviceName);
	~OpenClDeviceInfo(void);

	cl_device_id _deviceId;
	cl_platform_id _platformId;
	QString _deviceName;
};

#endif