#include "OpenClDeviceInfo.h"


OpenClDeviceInfo::OpenClDeviceInfo(cl_device_id deviceId, cl_platform_id platformId, QString deviceName):
_deviceId(deviceId), _platformId(platformId), _deviceName(deviceName)
{

}


OpenClDeviceInfo::~OpenClDeviceInfo(void)
{
}