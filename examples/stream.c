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
#include <ecl.h>
#include <assert.h>
#include <stdio.h>

const char *src =
"__kernel void stream(__global const float *a, __global float *b, int n)\n"
"{\n"
"        b[get_global_id(0)] = a[get_global_id(0)];\n"
"}\n"
;

int main()
{
	struct ecl_context ctx;
	cl_program program;
	cl_kernel kernel;
	cl_int err;
	cl_mem in, out;
	size_t globWorkSize;
	int n = 100000;
	cl_event event;
	cl_ulong start, end;

	err = eclGetSomeContext(&ctx);
	assert(err == CL_SUCCESS);

	err = eclGetProgramFromSource(ctx.context, ctx.device, src, &program);
	assert(err == CL_SUCCESS);

	kernel = clCreateKernel(program, "stream", &err);
	assert(err == CL_SUCCESS);

	in = clCreateBuffer(ctx.context, CL_MEM_READ_ONLY, n * sizeof(float),
			0, &err);
	assert(err == CL_SUCCESS);
	out = clCreateBuffer(ctx.context, CL_MEM_READ_ONLY, n * sizeof(float),
			0, &err);
	assert(err == CL_SUCCESS);

	err = clSetKernelArg(kernel, 0, sizeof(in), &in);
	assert(err == CL_SUCCESS);
	err = clSetKernelArg(kernel, 1, sizeof(out), &out);
	assert(err == CL_SUCCESS);
	err = clSetKernelArg(kernel, 2, sizeof(n), &n);
	assert(err == CL_SUCCESS);

	globWorkSize = n;
	err = clEnqueueNDRangeKernel(ctx.queue, kernel, 1, 0, &globWorkSize, 0,
			0, 0, &event);
	assert(err == CL_SUCCESS);
	clWaitForEvents(1, &event);
	err = clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START,
			sizeof(start), &start, 0);
	if (err == CL_PROFILING_INFO_NOT_AVAILABLE) {
		printf("Profling info not available.\n");
		return err;
	} else if (err) {
		printf("An error occurred getting profiling info.\n");
		return err;
	}
	err = clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END,
			sizeof(end), &end, 0);
	if (err == CL_PROFILING_INFO_NOT_AVAILABLE) {
		printf("Profling info not available.\n");
		return err;
	} else if (err) {
		printf("An error occurred getting profiling info.\n");
		return err;
	}
	printf("T/ms: %lf\n", (double)(end - start) / 1.0e6);
	printf("MB:   %lf\n", (double)n * sizeof(float) / 1.0e6);
	printf("GB/s: %lf\n", (double)n * sizeof(float) / (end - start));

	clReleaseCommandQueue(ctx.queue);
	clReleaseContext(ctx.context);
	clReleaseProgram(program);
	clReleaseKernel(kernel);
	clReleaseMemObject(in);
	clReleaseMemObject(out);
	clReleaseEvent(event);

	return err;
}

