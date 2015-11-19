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

#ifdef _MSC_VER
#  ifdef BUILDING_ECL
#    define ECL_API __declspec(dllexport)
#  else
#    define ECL_API __declspec(dllimport)
#  endif
#elif __GNUC__ >= 4 || defined(__clang__)
#  define ECL_API __attribute__((visibility ("default")))
#else
#  define ECL_API
#endif

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
ECL_API cl_int eclGetProgramFromSource(cl_context context, cl_device_id device,
		const char *source, cl_program *program);
ECL_API cl_int eclGetProgramFromFile(cl_context context, cl_device_id device,
		const char *fileName, cl_program *program);


/* Error codes */
#define ECL_INVALID_FILE 1


typedef cl_uint(*EclChoice)();
ECL_API cl_int eclSetPlatformChoice(EclChoice choice);
ECL_API cl_int eclSetDeviceChoice(EclChoice choice);

#ifdef __cplusplus
}
#endif

#endif

