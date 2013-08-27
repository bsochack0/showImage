#include "OpenClDeviceInfo.h"

#ifndef OPENCL_CLASS_H
#define OPENCL_CLASS_H

class OpenCL
{
public:
	OpenCL(void);
	~OpenCL(void);
	bool getOpenCLDeviceList(std::vector<OpenClDeviceInfo> &deviceList);
};

#endif