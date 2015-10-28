/*
Copyright 2014 Dominic Meiser

This file is part of EasyOpenCL.

mbo is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation, either version 3 of the License, or (at your
option) any later version.

mbo is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License along
with EasyOpenCL.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef EASY_OPENCL_H
#define EASY_OPENCL_H

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#include <EclExport.h>


#ifdef __cplusplus
extern "C" {
#endif


struct ecl_context {
	cl_context context;
	cl_device_id device;
	cl_command_queue queue;
};

ECL_API cl_int eclGetSomeContext(struct ecl_context *context);
ECL_API cl_int eclGetContextInteractively(struct ecl_context *context);

typedef int(*EclChoice)();
ECL_API cl_int eclSetPlatformChoice(EclChoice choice);
ECL_API cl_int eclSetDeviceChoice(EclChoice choice);

#ifdef __cplusplus
}
#endif

#endif

