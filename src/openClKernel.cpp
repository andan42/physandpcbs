#include "openClKernel.h"

#include <iostream>
#include <string.h>
#include <fstream>

OpenClKernel::OpenClKernel(cl::Context context, cl::Device device)
{
    this->context = context;
    this->device = device;
}

int OpenClKernel::CreateProgramFromSource(const char* source)
{
    cl::Program::Sources sources;
    sources.push_back({ source, strlen(source) });
    program = cl::Program(context, sources);
    if (program.build({ device }) != CL_SUCCESS)
    {
        std::cout << "Error building: " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device) << std::endl;
        return -1;
    }
    return 0;
}

int OpenClKernel::CreateProgramFromSourceFile(const char* sourceFile)
{
    std::ifstream sourceFileStream(sourceFile);
    std::string sourceString((std::istreambuf_iterator<char>(sourceFileStream)), (std::istreambuf_iterator<char>()));
    return CreateProgramFromSource(sourceString.c_str());
}

int OpenClKernel::CreateKernel(const char* kernelName)
{
    cl_int err;
    kernel = cl::Kernel(program, kernelName, &err);
    if (err != CL_SUCCESS)
    {
        std::cout << "Error creating kernel: " << err << std::endl;
        return -1;
    }
    return 0;
}
