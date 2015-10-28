#include <ecl.h>

static cl_int getAllPlatforms(cl_uint *numPlatforms,
			      cl_platform_id **platforms);

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

	/* First get list of available platforms */
	err = getAllPlatforms(&numPlatforms, &platforms);
	if (err != CL_SUCCESS) {
		return err;
	}
	if (!numPlatforms) {
		err = CL_INVALID_PLATFORM;
		goto cleanup;
	}

	/* Then get the list of devices available for the first platform */
	err = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_ALL, 0, 0,
			&numDevices);
	if (err != CL_SUCCESS) {
		goto cleanup;
	}
	if (!numDevices) {
		err = CL_DEVICE_NOT_FOUND;
		goto cleanup;
	}
	err = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_ALL, 1, &device, 0);
	if (err != CL_SUCCESS) {
		goto cleanup;
	}

	/* Then create a context with that device */
	props[0] = CL_CONTEXT_PLATFORM;
	props[1] = (cl_context_properties)platforms[0];
	ctx = clCreateContext(props, 1, &device, 0, 0, &err);
	if (err != CL_SUCCESS) {
		goto cleanup;
	}

	/* Then create a command queue */
	queue = clCreateCommandQueue(ctx, device, 0, &err);
	if (err != CL_SUCCESS) {
		goto cleanup;
	}

	/* Finally store context data */
	context->context = ctx;
	context->device = device;
	context->queue = queue;

	return CL_SUCCESS;

cleanup:
	free(platforms);
	return err;
}

ECL_API cl_int eclGetContextInteractively(struct ecl_context *context)
{
	context->device = 0;
	return CL_SUCCESS;
}

cl_int getAllPlatforms(cl_uint *numPlatforms, cl_platform_id **platforms)
{
	cl_int err = clGetPlatformIDs(0, 0, numPlatforms);
	if (err != CL_SUCCESS) return err;
	*platforms = malloc(*numPlatforms * sizeof(**platforms));
	if (!platforms) return CL_OUT_OF_HOST_MEMORY;
	err = clGetPlatformIDs(*numPlatforms, *platforms, 0);
	if (err != CL_SUCCESS) {
		free(*platforms);
		*platforms = 0;
	}
	return CL_SUCCESS;
}
