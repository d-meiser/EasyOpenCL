[![Build Status](https://travis-ci.org/d-meiser/EasyOpenCL.svg?branch=master)](https://travis-ci.org/d-meiser/EasyOpenCL)
[![Coverage Status](https://coveralls.io/repos/d-meiser/EasyOpenCL/badge.svg?branch=master&service=github)](https://coveralls.io/github/d-meiser/EasyOpenCL?branch=master)

# EasyOpenCL

This library eliminates much of the boilerplate needed to get started
with OpenCL.  It allows you to get started more easily and more quickly.
For example to get an OpenCL context, a command queue, and a device all
you'd have to do is
```
struct ecl_context ctx;
eclGetSomeContext(&ctx);
```
EasyOpenCL tries to do this without adding any layers of abstraction.
The utility functions that make up the library use and return standard
OpenCL types.  This way it's super easy to mix and match EasyOpenCL
functions with your own code.


# How to use EasyOpenCL

By far the easiest way to get going with EasyOpenCL is to just grab the
files `ecl.h` and `ecl.c` from the `src` directory and add them to your
project.


# Documentation

EasyOpenCL consists of four functions, see `ecl.h`.  Two of them deal
with setting up OpenCL contexts, command queues, and devices, and the
other two create `cl_program`s.  Context, queue, and device are
collected in `struct ecl_context`.


## `eclGetSomeContext`

Returns some context.  Use this if you really don't care about the
specifics of the device and platform and just want to running OpenCL
code ASAP.


## `eclGetContextInteractively`

Let's the user choose platform and device.  This is similar to
PyOpenCL's `create_some_context(interactive=True)`.


## `eclGetProgramFromSource`

This produces a `cl_program` from a source string.  Use
`eclGetSomeContext` or `eclGetContextInteractively` to obtain the
`cl_context` and `cl_device_id` needed by this function.  The main
simplification that this function offers is that it handles errors that
occur during the compilation.  If a compilation error occurs the
function obtains the compiler log and prints it to the screen.


## `eclGetProgramFromFile`

Is the same as `eclGetProgramFromSource` except that the kernel source
is read from a file.

