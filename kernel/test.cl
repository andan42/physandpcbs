__kernel void test(__global int* data)
{
    data[get_global_id(0)] = get_global_id(0) * get_global_id(0) * get_global_id(0) * get_global_id(0);
}