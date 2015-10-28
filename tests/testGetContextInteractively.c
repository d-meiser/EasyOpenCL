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

static int choice = 0;

int mockInteractiveChoice() { return choice; }

Describe(eclGetContextInteractively)

BeforeEach(eclGetContextInteractively)
{
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

int main() {
	int err;
	TestSuite *suite = create_test_suite();

	add_test_with_context(suite, eclGetContextInteractively, runs);

	err = run_test_suite(suite, create_text_reporter());
	destroy_test_suite(suite);
	return err;
}
