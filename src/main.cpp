//#include <glad/glad.h>
//#include <GLFW/glfw3.h>

#include <iostream>
#include <math.h>

//include stuff to sleep
#include <chrono>
#include <thread>

#include "renderingWrapper.h"

#include "openClInfrastructure.h"

#include <fstream>

int main()
{
    OpenClInfrastructure openClInfrastructure;
    openClInfrastructure.PickNvidiaPlatform();
    openClInfrastructure.PickGpuDevice();
    openClInfrastructure.CreateContext();
    openClInfrastructure.CreateCommandQueue();
    std::ifstream testKernelFile("kernel/test.cl");
    std::string testKernelString((std::istreambuf_iterator<char>(testKernelFile)), (std::istreambuf_iterator<char>()));
    
    cl::Program::Sources testKernelSources;
    testKernelSources.push_back({ testKernelString.c_str(), testKernelString.length() });

    cl::Program testKernelProgram(openClInfrastructure.context, testKernelSources);
    cl_int err = testKernelProgram.build({ openClInfrastructure.device }, "-cl-std=CL2.0");
    if (err != CL_SUCCESS)
    {
        std::cout << "Error building: " << testKernelProgram.getBuildInfo<CL_PROGRAM_BUILD_LOG>(openClInfrastructure.device) << std::endl;
        return -1;
    }
    
    int buffer[1600];
    cl::Buffer testBuffer(openClInfrastructure.context, CL_MEM_READ_WRITE, sizeof(buffer));

    cl::Kernel testKernel(testKernelProgram, "test", &err);
    if (err != CL_SUCCESS)
    {
        std::cout << "Error creating kernel: " << err << std::endl;
        return -1;
    }
    testKernel.setArg(0, testBuffer);
    std::cout << "Running kernel..." << std::endl;
    //measure time elapsed
    auto start = std::chrono::high_resolution_clock::now();
    openClInfrastructure.commandQueue.enqueueWriteBuffer(testBuffer, CL_TRUE, 0, sizeof(buffer), buffer);
    openClInfrastructure.commandQueue.enqueueNDRangeKernel(testKernel, cl::NullRange, cl::NDRange(1600), cl::NullRange);
    openClInfrastructure.commandQueue.enqueueReadBuffer(testBuffer, CL_TRUE, 0, sizeof(buffer), buffer);
    openClInfrastructure.commandQueue.finish();
    //end time elapsed
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    //print 1 of every 100 values to reduce output size
    for (int i = 0; i < 1600; i += 100)
    {
        std::cout << buffer[i] << "  ";
    }
    std::cout << std::endl;
    //print time elapsed
    std::cout << "Elapsed time: " << elapsed.count() << " s " << std::endl;
    return 0;
}

