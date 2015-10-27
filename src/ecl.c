#include <ecl.h>

static cl_int getAllPlatforms(cl_uint *numPlatforms,
			      cl_platform_id **platforms);

cl_int eclGetSomeContext(struct ecl_context *context)
{
	/* First get list of available platforms */
	cl_uint numPlatforms;
	cl_platform_id *platforms = 0;
	cl_int err = getAllPlatforms(&numPlatforms, &platforms);
	if (err != CL_SUCCESS) {
		return err;
	}
	if (!numPlatforms) {
		err = CL_INVALID_PLATFORM;
		goto cleanup;
	}

	/* Then get the list of devices available for the first platform */
	cl_uint numDevices;
	err = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_ALL, 0, 0,
			&numDevices);
	if (err != CL_SUCCESS) {
		goto cleanup;
	}
	if (!numDevices) {
		err = CL_DEVICE_NOT_FOUND;
		goto cleanup;
	}
	cl_device_id device;
	err = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_ALL, 1, &device, 0);
	if (err != CL_SUCCESS) {
		goto cleanup;
	}

	/* Then create a context with that device */
	cl_context_properties props[] = {
	    CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[0], 0};
	cl_context ctx = clCreateContext(props, 1, &device, 0, 0, &err);
	if (err != CL_SUCCESS) {
		goto cleanup;
	}

	/* Store context data */
	context->context = ctx;

	return CL_SUCCESS;

cleanup:
	free(platforms);
	return err;
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
