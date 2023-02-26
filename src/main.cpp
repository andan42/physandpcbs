//#include <glad/glad.h>
//#include <GLFW/glfw3.h>

#include <iostream>
#include <math.h>

//include stuff to sleep
#include <chrono>
#include <thread>

#include "renderingWrapper.h"

#include "openClInfrastructure.h"
#include "openClKernel.h"

#include <fstream>

int main()
{
    OpenClInfrastructure openClInfrastructure;
    openClInfrastructure.PickNvidiaPlatform();
    openClInfrastructure.PickGpuDevice();
    openClInfrastructure.CreateContext();
    openClInfrastructure.CreateCommandQueue();

    OpenClKernel openClKernel(openClInfrastructure.context, openClInfrastructure.device);
    
    openClKernel.CreateProgramFromSourceFile("kernel/test.cl");
    openClKernel.CreateKernel("test");

    //create buffers
    int bufferin[1600];
    cl::Buffer testBufferIn(openClInfrastructure.context, CL_MEM_READ_ONLY, sizeof(bufferin));
    //odd values are 10 and even values are 0
    for (int i = 0; i < 1600; i += 1)
    {
        if (i % 2 == 0)
        {
            bufferin[i] = 0;
        }
        else
        {
            bufferin[i] = 10;
        }
    }

    int bufferout[1600];
    cl::Buffer testBufferOut(openClInfrastructure.context, CL_MEM_WRITE_ONLY, sizeof(bufferout));

    openClKernel.kernel.setArg(0, testBufferIn);
    openClKernel.kernel.setArg(1, testBufferOut);
    std::cout << "Running kernel..." << std::endl;
    //start time elapsed
    auto start = std::chrono::high_resolution_clock::now();

    //make an ndrange

    //run kernel
    openClInfrastructure.commandQueue.enqueueWriteBuffer(testBufferIn, CL_TRUE, 0, sizeof(bufferin), bufferin);
    openClInfrastructure.commandQueue.enqueueNDRangeKernel(openClKernel.kernel, cl::NullRange, cl::NDRange(40, 40), cl::NullRange);
    openClInfrastructure.commandQueue.enqueueReadBuffer(testBufferOut, CL_TRUE, 0, sizeof(bufferout), bufferout);
    openClInfrastructure.commandQueue.finish();

    //end time elapsed
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;

    //print
    for (int i = 0; i < 1600; i += 1)
    {

        std::cout << bufferout[i] << " ";
    }
    std::cout << std::endl;

    //print time elapsed
    std::cout << "Elapsed time: " << elapsed.count() << " s " << std::endl;
    return 0;
}

