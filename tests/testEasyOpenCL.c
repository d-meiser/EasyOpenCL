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

Describe(getSomeOpenCLEnvironment)

BeforeEach(getSomeOpenCLEnvironment) {}
AfterEach(getSomeOpenCLEnvironment) {}

Ensure(getSomeOpenCLEnvironment, returnsAValidEnvironment) {}

int main() {
	TestSuite *suite = create_test_suite();
	add_test_with_context(suite, getSomeOpenCLEnvironment,
			returnsAValidEnvironment);
	destroy_test_suite(suite);
	return 0;
}
