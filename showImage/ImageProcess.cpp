#include "ImageProcess.h"

#include "CL\cl.h"
#include <vector>
#include <string>
#include <sstream>

class clContext
{
public:
	clContext(): _context(0) { }
	clContext(cl_context context): _context(context) { }
	~clContext() { clReleaseContext(_context); }

	clContext & operator=(const cl_context &rhs) { _context = rhs; return *this; }
	operator cl_context() const { return _context; }
private:
	cl_context _context;
};

class clCommandQueue
{
public:
	clCommandQueue(): _commandQueue(0) { }
	clCommandQueue(cl_command_queue commandQueue): _commandQueue(commandQueue) { }
	~clCommandQueue() { clReleaseCommandQueue(_commandQueue); }

	clCommandQueue & operator=(const cl_command_queue &rhs) { _commandQueue = rhs; return *this; }
	operator cl_command_queue() const { return _commandQueue; }
private:
	cl_command_queue _commandQueue;
};

class clMem
{
public:
	clMem(): _mem(0) { }
	clMem(cl_mem mem): _mem(mem) { }
	~clMem() { clReleaseMemObject(_mem); }

	clMem & operator=(const cl_mem &rhs) { _mem = rhs; return *this; }
	operator cl_mem() const { return _mem; }
private:
	cl_mem _mem;
};

class clProgram
{
public:
	clProgram(): _program(0) { }
	clProgram(cl_program program): _program(program) { }
	~clProgram() { clReleaseProgram(_program); }

	clProgram & operator=(const cl_program &rhs) { _program = rhs; return *this; }
	operator cl_program() const { return _program; }
private:
	cl_program _program;
};

class clKernel
{
public:
	clKernel(): _kernel(0) { }
	clKernel(cl_kernel kernel): _kernel(kernel) { }
	~clKernel() { clReleaseKernel(_kernel); }

	clKernel & operator=(const cl_kernel &rhs) { _kernel = rhs; return *this; }
	operator cl_kernel() const { return _kernel; }
private:
	cl_kernel _kernel;
};

ImageProcess::ImageProcess(QImage &imageProcess, QTextBrowser &infoBox, cl_device_id deviceId):
_image(imageProcess), _infoBox(infoBox), _deviceId(deviceId)
{
	const cl_uint channelInNum = 3;
	const cl_uint channelOutNum = 4;
	const cl_uint width = _image.width();
	const cl_uint height = _image.height(); 
	const cl_uint imageSize = width * height * channelOutNum;

	std::vector<cl_uchar> imageData(imageSize, 0);
	for (size_t h = 0; h < height; ++h) {
		for (size_t w = 0; w < width; ++w) {
			for (size_t c = 0; c < channelInNum; ++c) {
				imageData[channelOutNum * width * h + channelOutNum * w + c] = (_image.bits())[channelInNum * width * h + channelInNum * w + c];
			}
		}
	}

	cl_int error;
	clContext context = clCreateContext(0, 1, &deviceId, 0, 0, &error);
	if(error != CL_SUCCESS) {
		_infoBox.setHtml("OpenCL: clCreateContext failed");
		return;
	}

	clCommandQueue queue = clCreateCommandQueue(context, deviceId, 0, &error);
	if(error != CL_SUCCESS) {
		_infoBox.setHtml("OpenCL: clCreateCommandQueue failed");
		return;
	}

	cl_image_format imageFormat = { 0 };
	imageFormat.image_channel_order = CL_RGBA;
	imageFormat.image_channel_data_type = CL_UNORM_INT8;

	cl_image_desc imageDesc = { 0 };
	imageDesc.image_type = CL_MEM_OBJECT_IMAGE2D;
	imageDesc.image_width = width;
	imageDesc.image_height = height;
	
	clMem imageIn = clCreateImage(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, &imageFormat, &imageDesc, &imageData[0], &error);
	if(error != CL_SUCCESS) {
		_infoBox.setHtml("OpenCL: clCreateImage in failed");
		return;
	}

	clMem imageOut = clCreateImage(context, CL_MEM_WRITE_ONLY, &imageFormat, &imageDesc, 0, &error);
	if(error != CL_SUCCESS) {
		_infoBox.setHtml("OpenCL: clCreateImage out failed");
		return;
	}

	std::string kernelStr = 
		"__kernel void grayFilter(read_only image2d_t input, write_only image2d_t output)\n"
		"{\n"
		"  int x = get_global_id(0), y = get_global_id(1);\n"
		"  sampler_t sampler = CLK_FILTER_NEAREST | CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP;\n"
		"  float4 val = read_imagef(input, sampler, (int2)(x, y));\n"
	    "  float3 grayMatrix = {0.299f, 0.587f, 0.114f};\n"
	    "  float3 mono = dot(val.xyz, grayMatrix);\n"
		"  write_imagef(output, (int2)(x, y), (float4)(mono.x, mono.y, mono.z, 0.0f));\n"
		"}";

	const char *kernelPtr = kernelStr.c_str();
	clProgram program = clCreateProgramWithSource(context, 1, &kernelPtr, 0, &error);
	if(error != CL_SUCCESS) {
		_infoBox.setHtml("OpenCL: clCreateProgramWithSource failed");
		return;
	}

	error = clBuildProgram(program, 0, 0, 0, 0, 0);
	if(error != CL_SUCCESS) {
		_infoBox.setHtml("OpenCL: clBuildProgram failed");
		return;
	}

	clKernel kernel = clCreateKernel(program, "grayFilter", &error);
	if(error != CL_SUCCESS) {
		_infoBox.setHtml("OpenCL: clCreateKernel failed");
		return;
	}

	error = clSetKernelArg(kernel, 0, sizeof(cl_mem), &imageIn);
	if(error != CL_SUCCESS) {
		_infoBox.setHtml("OpenCL: clSetKernelArg 0 failed");
		return;
	}

	error = clSetKernelArg(kernel, 1, sizeof(cl_mem), &imageOut);
	if(error != CL_SUCCESS) {
		_infoBox.setHtml("OpenCL: clSetKernelArg 1 failed");
		return;
	}

	size_t workSize[3] = { width, height, 1 };
	error = clEnqueueNDRangeKernel(queue, kernel, 2, 0, workSize, 0, 0, 0, 0);
	if(error != CL_SUCCESS) {
		_infoBox.setHtml("OpenCL: clEnqueueNDRangeKernel failed");
		return;
	}

	size_t origin[3] = { 0 };
	error = clEnqueueReadImage(queue, imageOut, CL_TRUE, origin, workSize, 0, 0, &imageData[0], 0, 0, 0);
	if(error != CL_SUCCESS) {
		_infoBox.setHtml("OpenCL: clEnqueueReadImage failed");
		return;
	}

	for (size_t h = 0; h < height; ++h) {
		for (size_t w = 0; w < width; ++w) {
			for (size_t c = 0; c < channelInNum; ++c) {
				(_image.bits())[channelInNum * width * h + channelInNum * w + c] = imageData[channelOutNum * width * h + channelOutNum * w + c];
			}
		}
	}

	_infoBox.setHtml("Image processed");
}
