#include <ecl.h>

cl_int eclGetSomeContext(struct ecl_context *context) {
	context->context = (void*)1;
	return CL_SUCCESS;
}
