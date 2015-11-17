__kernel void stream(__global const float *a, __global float *b, int n)
{
        b[get_global_id(0)] = a[get_global_id(0)];
}

