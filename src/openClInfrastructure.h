
#pragma once

#define CL_HPP_TARGET_OPENCL_VERSION 200
#include "CL/opencl.hpp"

class OpenClInfrastructure
{
public:
    cl::Platform platform;
    cl::Device device;
    cl::Context context;
    cl::CommandQueue commandQueue;
    int PickNvidiaPlatform();
    int PickGpuDevice();
    int CreateContext();
    int CreateCommandQueue();
};
