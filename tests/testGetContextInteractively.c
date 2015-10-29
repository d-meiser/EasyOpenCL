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
static cl_int err;

static cl_uint choice = 1;

cl_uint mockInteractiveChoice() { return choice; }
cl_uint mockInteractiveChoiceBogus() { return 10000; }

Describe(eclGetContextInteractively)

BeforeEach(eclGetContextInteractively)
{
	choice = 1;
	eclSetPlatformChoice(mockInteractiveChoice);
	eclSetDeviceChoice(mockInteractiveChoice);
	ctx.context = 0;
	ctx.device = 0;
	ctx.queue = 0;
}

AfterEach(eclGetContextInteractively)
{
	if (ctx.context) {
		clReleaseContext(ctx.context);
	}
	if (ctx.queue) {
		clReleaseCommandQueue(ctx.queue);
	}
}

Ensure(eclGetContextInteractively, runs) {
	err = eclGetContextInteractively(&ctx);
}

Ensure(eclGetContextInteractively, returnsAContextIfNoErrorOccured) {
	err = eclGetContextInteractively(&ctx);
	if (err == CL_SUCCESS) {
		assert_that(ctx.context, is_non_null);
	}
}

Ensure(eclGetContextInteractively, returnsAValidContext) {
	eclGetContextInteractively(&ctx);
	if (ctx.context) {
		size_t size;
		err = clGetContextInfo(ctx.context,
				CL_CONTEXT_PROPERTIES, 0, 0, &size);
		assert_that(err, is_equal_to(CL_SUCCESS));
	}
}

Ensure(eclGetContextInteractively, returnsADeviceIfNoErrorOccured) {
	err = eclGetContextInteractively(&ctx);
	if (err == CL_SUCCESS) {
		assert_that(ctx.device, is_non_null);
	}
}

Ensure(eclGetContextInteractively, returnsAValidDevice) {
	eclGetContextInteractively(&ctx);
	if (ctx.device) {
		size_t size;
		err = clGetDeviceInfo(ctx.device,
				CL_DEVICE_ENDIAN_LITTLE, 0, 0, &size);
		assert_that(err, is_equal_to(CL_SUCCESS));
	}
}

Ensure(eclGetContextInteractively, returnsACommandQueueIfNoErrorOccured) {
	err = eclGetContextInteractively(&ctx);
	if (err == CL_SUCCESS) {
		assert_that(ctx.queue, is_non_null);
	}
}

Ensure(eclGetContextInteractively, returnsAValidCommandQueue) {
	eclGetContextInteractively(&ctx);
	if (ctx.queue) {
		size_t size;
		err = clGetCommandQueueInfo(ctx.queue,
				CL_QUEUE_PROPERTIES, 0, 0, &size);
		assert_that(err, is_equal_to(CL_SUCCESS));
	}
}

Ensure(eclGetContextInteractively, commandQueueConsistentWithContext) {
	size_t size;
	cl_context qCtx;
	err = eclGetContextInteractively(&ctx);
	assert_that(err, is_equal_to(CL_SUCCESS));
	clGetCommandQueueInfo(ctx.queue, CL_QUEUE_CONTEXT, 0, 0, &size);
	assert_that(err, is_equal_to(CL_SUCCESS));
	assert_that(size, is_equal_to(sizeof(qCtx)));
	clGetCommandQueueInfo(ctx.queue, CL_QUEUE_CONTEXT, size, &qCtx, 0);

	assert_that(qCtx, is_equal_to(ctx.context));
}

Ensure(eclGetContextInteractively, commandQueueConsistentWithDevice) {
	size_t size;
	cl_device_id qDevice;
	err = eclGetContextInteractively(&ctx);
	assert_that(err, is_equal_to(CL_SUCCESS));
	clGetCommandQueueInfo(ctx.queue, CL_QUEUE_DEVICE, 0, 0, &size);
	assert_that(err, is_equal_to(CL_SUCCESS));
	assert_that(size, is_equal_to(sizeof(qDevice)));
	clGetCommandQueueInfo(ctx.queue, CL_QUEUE_DEVICE, size, &qDevice, 0);

	assert_that(qDevice, is_equal_to(ctx.device));
}

Ensure(eclGetContextInteractively, doesntCrashDueToBogusPlatformChoice) {
	eclSetPlatformChoice(mockInteractiveChoiceBogus);
	err = eclGetContextInteractively(&ctx);
	assert_that(err, is_equal_to(CL_INVALID_VALUE));
}

Ensure(eclGetContextInteractively, recoversFromBogusPlatformChoice) {
	eclSetPlatformChoice(mockInteractiveChoiceBogus);
	eclGetContextInteractively(&ctx);
	eclSetPlatformChoice(mockInteractiveChoice);
	err = eclGetContextInteractively(&ctx);
	assert_that(err, is_equal_to(CL_SUCCESS));
}

Ensure(eclGetContextInteractively, doesntCrashDueToBogusDeviceChoice) {
	eclSetDeviceChoice(mockInteractiveChoiceBogus);
	err = eclGetContextInteractively(&ctx);
	assert_that(err, is_equal_to(CL_INVALID_VALUE));
}

Ensure(eclGetContextInteractively, recoversFromBogusDeviceChoice) {
	eclSetDeviceChoice(mockInteractiveChoiceBogus);
	eclGetContextInteractively(&ctx);
	eclSetDeviceChoice(mockInteractiveChoice);
	err = eclGetContextInteractively(&ctx);
	assert_that(err, is_equal_to(CL_SUCCESS));
}

int main() {
	int err;
	TestSuite *suite = create_test_suite();

	add_test_with_context(suite, eclGetContextInteractively, runs);
	add_test_with_context(suite, eclGetContextInteractively,
			returnsAContextIfNoErrorOccured);
	add_test_with_context(suite, eclGetContextInteractively,
			returnsAValidContext);
	add_test_with_context(suite, eclGetContextInteractively,
			returnsADeviceIfNoErrorOccured);
	add_test_with_context(suite, eclGetContextInteractively,
			returnsAValidDevice);
	add_test_with_context(suite, eclGetContextInteractively,
			returnsACommandQueueIfNoErrorOccured);
	add_test_with_context(suite, eclGetContextInteractively,
			returnsAValidCommandQueue);
	add_test_with_context(suite, eclGetContextInteractively,
			commandQueueConsistentWithContext);
	add_test_with_context(suite, eclGetContextInteractively,
			commandQueueConsistentWithDevice);
	add_test_with_context(suite, eclGetContextInteractively,
			doesntCrashDueToBogusPlatformChoice);
	add_test_with_context(suite, eclGetContextInteractively,
			recoversFromBogusPlatformChoice);
	add_test_with_context(suite, eclGetContextInteractively,
			doesntCrashDueToBogusDeviceChoice);
	add_test_with_context(suite, eclGetContextInteractively,
			recoversFromBogusDeviceChoice);

	err = run_test_suite(suite, create_text_reporter());
	destroy_test_suite(suite);
	return err;
}
