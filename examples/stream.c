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
#include <string.h>
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
	cl_int err;
	size_t len;
	const char *log;

	err = eclGetSomeContext(&ctx);
	assert(err == CL_SUCCESS);

	len = strlen(src);
	program = clCreateProgramWithSource(ctx.context, 1, &src, &len, &err);
	assert(err == CL_SUCCESS);

	err = clBuildProgram(program, 1, &ctx.device, "", 0, 0);
	if (err != CL_SUCCESS) {
		err = clGetProgramBuildInfo(program, ctx.device,
				CL_PROGRAM_BUILD_LOG, 0, 0, &len);
		log = malloc(len);
		err = clGetProgramBuildInfo(program, ctx.device,
				CL_PROGRAM_BUILD_LOG, len, (void*)log, 0);
		printf("%s\n", log);
	}
	assert(err == CL_SUCCESS);

	clReleaseCommandQueue(ctx.queue);
	clReleaseContext(ctx.context);
	clReleaseProgram(program);

	return 0;
}

