__kernel void test(__global int* inbuf, __global int* outbuf)
{
    size_t id = get_global_id(1) * get_global_size(0) + get_global_id(0);

    outbuf[id] = get_global_id(0) + get_global_id(1) + inbuf[id];
}