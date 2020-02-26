// #define _GNU_SOURCE
#include <cstdio>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <cstdint>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <stdexcept>
using namespace std;

constexpr int TRASNFER_DIV = 1024;

char const *unit_transfer_table[] = {
    "Bytes", "KiB", "MiB",
    "GiB",   "TiB", "EiB"
};

int __transfer_unit(double& __data, int __depth)
{
    if (__data < static_cast<double>(TRASNFER_DIV))
        return __depth;
    if (__depth >= 6)
        return __depth;
    __data /= static_cast<double>(TRASNFER_DIV);
    return __transfer_unit(__data, __depth + 1);
}

std::string transfer_unit(double __data)
{
    std::ostringstream ss;
    int idx = __transfer_unit(__data, 0);
    ss << __data << unit_transfer_table[idx];
    return ss.str();
}

double calculate_time(timeval& __start, timeval& __end)
{
    double start_time = __start.tv_sec
        + static_cast<double>(__start.tv_usec) / 1000000.0;
    double end_time = __end.tv_sec
        + static_cast<double>(__end.tv_usec) / 1000000.0;
    return end_time - start_time;
}

extern "C"
{
    int direct_memory_access(size_t __size, timeval *__start, timeval *__end);
}

int main(int argc, char const *argv[])
{
    timeval start, end;
    size_t size = 1024 * 1024 * 1024;
    direct_memory_access(size, &start, &end);
    double time = calculate_time(start, end);
    double speed = size / time;
    // std::cout.setf(std::ios_base::fixed);
    std::cout.precision(2);
    std::cout << transfer_unit(size) << "\'s QWORD data was copied in "
        << time << " seconds (" << transfer_unit(speed) << "/s)" << std::endl;

    return 0;
}
