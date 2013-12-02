#include "CL\cl.h"
#include "OpenClDeviceInfo.h"
#include <vector>
#include "OpenCL.h"

OpenCL::OpenCL(void)
{
}


OpenCL::~OpenCL(void)
{
}

bool OpenCL::getOpenCLDeviceList(std::vector<OpenClDeviceInfo> &deviceList)
{
	cl_uint platformNum = 0;
	cl_int error = clGetPlatformIDs(0, NULL, &platformNum);
	
	if(platformNum == 0) 
	{
		return false;
	}

	std::vector<cl_platform_id> platformIDs(platformNum);

	error = clGetPlatformIDs(platformNum, &platformIDs[0], NULL);

	//names.resize(platformNum);
	//names.push_back(name);

	for (cl_uint i = 0; i < platformNum; ++i)
	{
        char platformName[1024] = { '\0' };

		cl_platform_id selectedPlatform = platformIDs[i];
        error = clGetPlatformInfo(selectedPlatform, CL_PLATFORM_NAME, 1024, &platformName, NULL);
 
        //error = clGetPlatformInfo(platformIds[i], CL_PLATFORM_VENDOR, 1024, &name, NULL);
 

        // Get device count.
        cl_uint deviceNumber;
 
        error = clGetDeviceIDs(selectedPlatform, CL_DEVICE_TYPE_ALL, 0, NULL, &deviceNumber);
 
        if (0 == deviceNumber)
        {
            continue;
        }
 
        // Get device identifiers.
		std::vector<cl_device_id> deviceIDs(deviceNumber);
        error = clGetDeviceIDs(selectedPlatform, CL_DEVICE_TYPE_ALL, deviceNumber, &deviceIDs[0], &deviceNumber);
 
        // Get device info.
        for (cl_uint i = 0; i < deviceNumber; ++i)
        {
            char deviceName[1024] = { '\0' };
 
            error = clGetDeviceInfo(deviceIDs[i], CL_DEVICE_NAME, 1024, &deviceName, NULL);
 
			QString fullName = QString(platformName) + deviceName;

			OpenClDeviceInfo openClDevice(deviceIDs[i], selectedPlatform, fullName);

			deviceList.push_back(openClDevice);
		}

	}

	return true;
}
