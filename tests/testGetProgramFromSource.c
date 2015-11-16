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
static const char src[] =
"__kernel void stream(__global const float *a, __global float *b, int n)\n"
"{\n"
"        b[get_global_id(0)] = a[get_global_id(0)];\n"
"}\n"
;

Describe(eclGetProgramFromSource)

BeforeEach(eclGetProgramFromSource)
{
	err = eclGetSomeContext(&ctx);
	assert_that(err, is_equal_to(CL_SUCCESS));
}

AfterEach(eclGetProgramFromSource)
{
	if (ctx.context) {
		clReleaseContext(ctx.context);
	}
	if (ctx.queue) {
		clReleaseCommandQueue(ctx.queue);
	}
}

Ensure(eclGetProgramFromSource, failsWhenGivenBogusSource) {
	cl_program prg;
	err = eclGetProgramFromSource(ctx.context, ctx.device,
			"Some source code with bugs", &prg);
	assert_that(err, is_not_equal_to(CL_SUCCESS));
}

Ensure(eclGetProgramFromSource, failsWhenGivenBadContext) {
	cl_program prg;
	err = eclGetProgramFromSource(0, ctx.device, src, &prg);
	assert_that(err, is_not_equal_to(CL_SUCCESS));
}

Ensure(eclGetProgramFromSource, worksForReasonableSource) {
	cl_program prg;
	err = eclGetProgramFromSource(ctx.context, ctx.device, src, &prg);
	assert_that(err, is_equal_to(CL_SUCCESS));
}

int main() {
	int err;
	TestSuite *suite = create_test_suite();

	add_test_with_context(suite, eclGetProgramFromSource,
			failsWhenGivenBogusSource);
	add_test_with_context(suite, eclGetProgramFromSource,
			failsWhenGivenBadContext);
	add_test_with_context(suite, eclGetProgramFromSource,
			worksForReasonableSource);

	err = run_test_suite(suite, create_text_reporter());
	destroy_test_suite(suite);
	return err;
}
