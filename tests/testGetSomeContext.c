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
#include <cgreen/cgreen.h>
#include <ecl.h>

static struct ecl_context ctx;
cl_int err;

Describe(eclGetSomeContext)

BeforeEach(eclGetSomeContext)
{
	ctx.context = 0;
	ctx.device = 0;
	ctx.queue = 0;
}

AfterEach(eclGetSomeContext)
{
	if (ctx.context) {
		clReleaseContext(ctx.context);
	}
	if (ctx.queue) {
		clReleaseCommandQueue(ctx.queue);
	}
}

Ensure(eclGetSomeContext, runs) {
	eclGetSomeContext(&ctx);
}

Ensure(eclGetSomeContext, returnsAContextIfNoErrorOccured) {
	err = eclGetSomeContext(&ctx);
	if (err == CL_SUCCESS) {
		assert_that(ctx.context, is_non_null);
	}
}

Ensure(eclGetSomeContext, returnsAValidContext) {
	eclGetSomeContext(&ctx);
	if (ctx.context) {
		size_t size;
		err = clGetContextInfo(ctx.context,
				CL_CONTEXT_PROPERTIES, 0, 0, &size);
		assert_that(err, is_equal_to(CL_SUCCESS));
	}
}

Ensure(eclGetSomeContext, returnsADeviceIfNoErrorOccured) {
	err = eclGetSomeContext(&ctx);
	if (err == CL_SUCCESS) {
		assert_that(ctx.device, is_non_null);
	}
}

Ensure(eclGetSomeContext, returnsAValidDevice) {
	eclGetSomeContext(&ctx);
	if (ctx.device) {
		size_t size;
		err = clGetDeviceInfo(ctx.device,
				CL_DEVICE_ENDIAN_LITTLE, 0, 0, &size);
		assert_that(err, is_equal_to(CL_SUCCESS));
	}
}

Ensure(eclGetSomeContext, returnsACommandQueueIfNoErrorOccured) {
	err = eclGetSomeContext(&ctx);
	if (err == CL_SUCCESS) {
		assert_that(ctx.queue, is_non_null);
	}
}

Ensure(eclGetSomeContext, returnsAValidCommandQueue) {
	eclGetSomeContext(&ctx);
	if (ctx.queue) {
		size_t size;
		err = clGetCommandQueueInfo(ctx.queue,
				CL_QUEUE_PROPERTIES, 0, 0, &size);
		assert_that(err, is_equal_to(CL_SUCCESS));
	}
}

Ensure(eclGetSomeContext, commandQueueConsistentWithContext) {
	size_t size;
	cl_context qCtx;
	err = eclGetSomeContext(&ctx);
	assert_that(err, is_equal_to(CL_SUCCESS));
	clGetCommandQueueInfo(ctx.queue, CL_QUEUE_CONTEXT, 0, 0, &size);
	assert_that(err, is_equal_to(CL_SUCCESS));
	assert_that(size, is_equal_to(sizeof(qCtx)));
	clGetCommandQueueInfo(ctx.queue, CL_QUEUE_CONTEXT, size, &qCtx, 0);

	assert_that(qCtx, is_equal_to(ctx.context));
}

Ensure(eclGetSomeContext, commandQueueConsistentWithDevice) {
	size_t size;
	cl_device_id qDevice;
	err = eclGetSomeContext(&ctx);
	assert_that(err, is_equal_to(CL_SUCCESS));
	clGetCommandQueueInfo(ctx.queue, CL_QUEUE_DEVICE, 0, 0, &size);
	assert_that(err, is_equal_to(CL_SUCCESS));
	assert_that(size, is_equal_to(sizeof(qDevice)));
	clGetCommandQueueInfo(ctx.queue, CL_QUEUE_DEVICE, size, &qDevice, 0);

	assert_that(qDevice, is_equal_to(ctx.device));
}

int main() {
	int err;
	TestSuite *suite = create_test_suite();

	add_test_with_context(suite, eclGetSomeContext, runs);
	add_test_with_context(suite, eclGetSomeContext,
			returnsAContextIfNoErrorOccured);
	add_test_with_context(suite, eclGetSomeContext,
			returnsAValidContext);
	add_test_with_context(suite, eclGetSomeContext,
			returnsADeviceIfNoErrorOccured);
	add_test_with_context(suite, eclGetSomeContext,
			returnsAValidDevice);
	add_test_with_context(suite, eclGetSomeContext,
			returnsACommandQueueIfNoErrorOccured);
	add_test_with_context(suite, eclGetSomeContext,
			returnsAValidCommandQueue);
	add_test_with_context(suite, eclGetSomeContext,
			commandQueueConsistentWithContext);
	add_test_with_context(suite, eclGetSomeContext,
			commandQueueConsistentWithDevice);

	err = run_test_suite(suite, create_text_reporter());
	destroy_test_suite(suite);
	return err;
}
