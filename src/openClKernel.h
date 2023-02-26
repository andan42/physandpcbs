
#pragma once

#define CL_HPP_TARGET_OPENCL_VERSION 200
#include "CL/opencl.hpp"

class OpenClKernel
{
public:
    cl::Context context;
    cl::Device device;
    OpenClKernel(cl::Context context, cl::Device device);
    cl::Program program;
    cl::Kernel kernel;
    int CreateProgramFromSource(const char* source);
    int CreateProgramFromSourceFile(const char* sourceFile);
    int CreateKernel(const char* kernelName);
};