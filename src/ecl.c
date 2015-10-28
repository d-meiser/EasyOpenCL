#include <ecl.h>

#include <stdlib.h>
#include <stdio.h>

#define MAX_BUFFER_SIZE 80

static cl_int getAllPlatforms(cl_uint *numPlatforms,
			      cl_platform_id **platforms);
static int NullChooser();
static int InteractiveChooser();

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
	if (err != CL_SUCCESS) {
		return err;
	}
	if (!numPlatforms) {
		err = CL_INVALID_PLATFORM;
		goto cleanup;
	}

	/* Then get the list of devices available for the first platform */
	chosenPlatform = NullChooser();
	err = clGetDeviceIDs(platforms[chosenPlatform], CL_DEVICE_TYPE_ALL, 0,
			0, &numDevices);
	if (err != CL_SUCCESS) {
		goto cleanup;
	}
	if (!numDevices) {
		err = CL_DEVICE_NOT_FOUND;
		goto cleanup;
	}
	err = clGetDeviceIDs(platforms[chosenPlatform], CL_DEVICE_TYPE_ALL, 1,
			&device, 0);
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
	int i;
	i = interactivePlatformChooser();
	i = interactiveDeviceChooser();
	context->device = 0;
	return i;
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

static int NullChooser() { return 0; }

static int InteractiveChooser() {
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
		printf("charcount == %d\n", charCount);
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
