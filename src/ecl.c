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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_BUFFER_SIZE 1024

static cl_int getAllPlatforms(cl_uint *numPlatforms,
		cl_platform_id **platforms);
static cl_int getAllDevices(cl_platform_id platform, cl_uint *numDevices,
		cl_device_id **devices);
static cl_int printPlatformNames(cl_uint numPlatforms,
		cl_platform_id *platforms);
static cl_int printDeviceNames(cl_uint numDevices, cl_device_id *devices);
static cl_uint OneChooser();
static cl_uint InteractiveChooser();

static EclChoice interactivePlatformChooser = InteractiveChooser;
static EclChoice interactiveDeviceChooser = InteractiveChooser;

cl_int eclGetSomeContext(struct ecl_context *context)
{
	cl_int err;
	cl_uint numPlatforms;
	cl_platform_id *platforms = 0;
	cl_uint numDevices;
	cl_device_id device;
	cl_context_properties props[3] = {0};
	cl_context ctx;
	cl_command_queue queue;
	int chosenPlatform;

	/* First get list of available platforms */
	err = getAllPlatforms(&numPlatforms, &platforms);
	if (err != CL_SUCCESS) goto cleanup;

	/* Then get the list of devices available for the first platform */
	chosenPlatform = OneChooser() - 1;
	err = clGetDeviceIDs(platforms[chosenPlatform], CL_DEVICE_TYPE_ALL, 0,
			0, &numDevices);
	if (err != CL_SUCCESS) goto cleanup;
	if (!numDevices) {
		err = CL_DEVICE_NOT_FOUND;
		goto cleanup;
	}
	err = clGetDeviceIDs(platforms[chosenPlatform], CL_DEVICE_TYPE_ALL, 1,
			&device, 0);
	if (err != CL_SUCCESS) goto cleanup;

	/* Then create a context with that device */
	props[0] = CL_CONTEXT_PLATFORM;
	props[1] = (cl_context_properties)platforms[chosenPlatform];
	ctx = clCreateContext(props, 1, &device, 0, 0, &err);
	if (err != CL_SUCCESS) goto cleanup;

	/* Then create a command queue */
	queue = clCreateCommandQueue(ctx, device, CL_QUEUE_PROFILING_ENABLE,
			&err);
	if (err != CL_SUCCESS) {
		queue = clCreateCommandQueue(ctx, device, 0, &err);
		if (err != CL_SUCCESS) {
			goto cleanup;
		}
	}

	/* Finally store context data */
	context->context = ctx;
	context->device = device;
	context->queue = queue;

cleanup:
	free(platforms);
	return err;
}

cl_int eclGetContextInteractively(struct ecl_context *context)
{
	cl_int err; 
	cl_uint numPlatforms;
	cl_uint chosenPlatform;
	cl_platform_id *platforms = 0;
	cl_uint numDevices;
	cl_device_id *devices = 0;
	cl_uint chosenDevice;
	cl_context_properties props[3] = {0};
	cl_context ctx;
	cl_command_queue queue;

	/* First get list of available platforms */
	err = getAllPlatforms(&numPlatforms, &platforms);
	if (err != CL_SUCCESS) goto cleanup;

	/* choose platform */
	printPlatformNames(numPlatforms, platforms);
	chosenPlatform = interactivePlatformChooser() - 1;
	if (chosenPlatform >= numPlatforms) {
		printf("Invalid value: %d. Legal values are in [1, %d).\n",
				chosenPlatform + 1, numPlatforms + 1);
		err = CL_INVALID_VALUE;
		goto cleanup;
	}

	/* Then get the list of devices available for the chosen platform */
	err = getAllDevices(platforms[chosenPlatform], &numDevices, &devices);
	if (err != CL_SUCCESS) goto cleanup;

	/* choose device */
	err = printDeviceNames(numDevices, devices);
	if (err != CL_SUCCESS) goto cleanup;
	chosenDevice = interactiveDeviceChooser() - 1;
	if (chosenDevice >= numDevices) {
		printf("Invalid value: %d. Legal values are in [1, %d).\n",
				chosenDevice + 1, numDevices + 1);
		err = CL_INVALID_VALUE;
		goto cleanup;
	}

	/* Then create a context with that device */
	props[0] = CL_CONTEXT_PLATFORM;
	props[1] = (cl_context_properties)platforms[chosenPlatform];
	ctx = clCreateContext(props, 1, &devices[chosenDevice], 0, 0, &err);
	if (err != CL_SUCCESS) goto cleanup;

	/* Then create a command queue */
	queue = clCreateCommandQueue(ctx, devices[chosenDevice],
			CL_QUEUE_PROFILING_ENABLE, &err);
	if (err != CL_SUCCESS) goto cleanup;

	context->context = ctx;
	context->device = devices[chosenDevice];
	context->queue = queue;

cleanup:
	free(platforms);
	free(devices);
	return err;
}

cl_int eclGetProgramFromSource(cl_context context, cl_device_id device,
		const char *source, cl_program *program)
{
	cl_int err = CL_SUCCESS;
	size_t len;
	cl_program prog;

	len = strlen(source);
	prog = clCreateProgramWithSource(context, 1, &source, &len, &err);
	if (err != CL_SUCCESS) return err;

	err = clBuildProgram(prog, 1, &device, "", 0, 0);
	if (err != CL_SUCCESS) {
		const char *log;
		clGetProgramBuildInfo(prog, device, CL_PROGRAM_BUILD_LOG, 0, 0,
				&len);
		log = malloc(len);
		clGetProgramBuildInfo(prog, device, CL_PROGRAM_BUILD_LOG, len,
				(void*)log, 0);
		printf("%s\n", log);
		return err;
	}
	*program = prog;

	return err;
}

cl_int eclGetProgramFromFile(cl_context context, cl_device_id device,
		const char *fileName, cl_program *program)
{

	FILE *f;
	char *source;
	int err;
	size_t sourceSize, errS;

	f = fopen(fileName, "r");
	if (!f) return ECL_INVALID_FILE;
	err = fseek(f, 0, SEEK_END);
	if (err) return ECL_INVALID_FILE;
	sourceSize = ftell(f);
	err = fseek(f, 0, SEEK_SET);
	if (err) return ECL_INVALID_FILE;
	source = malloc(sourceSize + 1);
	if (!source) return CL_OUT_OF_HOST_MEMORY;
	errS = fread(source, 1, sourceSize, f);
	if (errS != sourceSize) return ECL_INVALID_FILE;
	source[sourceSize] = 0;
	fclose(f);
	err = eclGetProgramFromSource(context, device, source, program);
	free(source);
	return err;
}

cl_int getAllPlatforms(cl_uint *numPlatforms, cl_platform_id **platforms)
{
	cl_int err;
	err = clGetPlatformIDs(0, 0, numPlatforms);
	if (err != CL_SUCCESS) return err;
	if (!numPlatforms) return CL_INVALID_PLATFORM;
	*platforms = malloc(*numPlatforms * sizeof(**platforms));
	if (!platforms) return CL_OUT_OF_HOST_MEMORY;
	err = clGetPlatformIDs(*numPlatforms, *platforms, 0);
	return err;
}

static cl_int getAllDevices(cl_platform_id platform, cl_uint *numDevices,
		cl_device_id **devices)
{
	cl_int err;
	err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, 0, 0, numDevices);
	if (err != CL_SUCCESS) return err;
	if (!numDevices) return CL_DEVICE_NOT_FOUND;
	*devices = malloc(*numDevices * sizeof(**devices));
	if (!(*devices)) return CL_OUT_OF_HOST_MEMORY;
	err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, *numDevices,
			*devices, 0);
	return err;
}

static cl_uint OneChooser() { return 1; }

static cl_uint InteractiveChooser() {
	int result;
	int charCount;
	char buffer[MAX_BUFFER_SIZE];
	char ch;

	while (1) {
		ch = getchar();
		charCount = 0;
		while ((ch != '\n') && (charCount < MAX_BUFFER_SIZE - 1)) {
			buffer[charCount++] = ch;
			ch = getchar();
		}
		buffer[charCount] = 0x00;
		result = atoi(buffer);
		if (result < 1) {
			printf("Illegal number entered. Please try again.\n");
		} else {
			break;
		}
	}
	return result;
}

cl_int printPlatformNames(cl_uint numPlatforms, cl_platform_id *platforms)
{
	cl_uint i;
	cl_int err;
	char name[MAX_BUFFER_SIZE];

	for (i = 0; i < numPlatforms; ++i) {
		err = clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME,
				sizeof(name), name, 0);
		if (err != CL_SUCCESS) return err;
		printf("[%d] %s\n", i + 1, name);
	}
	return CL_SUCCESS;
}

cl_int printDeviceNames(cl_uint numDevices, cl_device_id *devices)
{
	cl_uint i;
	cl_int err;
	char name[MAX_BUFFER_SIZE];

	for (i = 0; i < numDevices; ++i) {
		err = clGetDeviceInfo(devices[i], CL_DEVICE_NAME,
				sizeof(name), name, 0);
		if (err != CL_SUCCESS) return err;
		printf("[%d] %s\n", i + 1, name);
	}
	return CL_SUCCESS;
}

cl_int eclSetPlatformChoice(EclChoice choice)
{
	interactivePlatformChooser = choice;
	return CL_SUCCESS;
}

cl_int eclSetDeviceChoice(EclChoice choice)
{
	interactiveDeviceChooser = choice;
	return CL_SUCCESS;
}
