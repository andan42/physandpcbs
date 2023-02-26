#include "openClInfrastructure.h"

#include <iostream>

int OpenClInfrastructure::PickNvidiaPlatform()
{
    
    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);

    if (platforms.size() == 0)
    {
        std::cout << "No OpenCL platforms found" << std::endl;
        return -1;
    }

    //print out the platform names
    std::cout << "Available platforms:" << std::endl;
    for (int i = 0; i < platforms.size(); i++)
    {
        std::cout << "Platform " << i << ": " << platforms[i].getInfo<CL_PLATFORM_NAME>() << std::endl;
    }
    //select platform with name NVIDIA CUDA, if it exists, else select the first platform
    int platformIndex = 0;
    for (int i = 0; i < platforms.size(); i++)
    {
        if (platforms[i].getInfo<CL_PLATFORM_NAME>() == "NVIDIA CUDA")
        {
            platformIndex = i;
            break;
        }
    }
    platform = platforms[platformIndex];
    std::cout << "Selected platform: " << platform.getInfo<CL_PLATFORM_NAME>() << std::endl;
    return 0;
}
int OpenClInfrastructure::PickGpuDevice()
{
    std::vector<cl::Device> devices;
    platform.getDevices(CL_DEVICE_TYPE_GPU, &devices);

    if (devices.size() == 0)
    {
        std::cout << "No OpenCL devices found" << std::endl;
        return -1;
    }

    //print out the device names, and compute unit values
    std::cout << "Available devices:" << std::endl;
    for (int i = 0; i < devices.size(); i++)
    {
        std::cout << "Device " << i << ": " << devices[i].getInfo<CL_DEVICE_NAME>();
        std::cout << "   Compute units: " << devices[i].getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>() << std::endl;
    }
    //select device with most compute units, if it exists, else select the first device
    int deviceIndex = 0;
    int maxComputeUnits = 0;
    for (int i = 0; i < devices.size(); i++)
    {
        int computeUnits = devices[i].getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>();
        if (computeUnits > maxComputeUnits)
        {
            maxComputeUnits = computeUnits;
            deviceIndex = i;
        }
    }
    device = devices[deviceIndex];
    std::cout << "Selected device: " << device.getInfo<CL_DEVICE_NAME>() << std::endl;
    return 0;
}
int OpenClInfrastructure::CreateContext()
{
    cl_context_properties properties[] =
    {
        CL_CONTEXT_PLATFORM, (cl_context_properties)(platform)(),
        0
    };
    context = cl::Context(device, properties);
    return 0;
}
int OpenClInfrastructure::CreateCommandQueue()
{
    commandQueue = cl::CommandQueue(context, device);
    return 0;
}