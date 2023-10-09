#ifndef WRITEINFO_HPP
#define WRITEINFO_HPP
#include "parser.hpp"

#define TIME_START              \
    clock_t ____start, ____end; \
    ____start = clock()
#define TIME_OUT \
    ____end = clock();

#define GET_DATE            \
    time_t today = time(0); \
    char tmp[21];           \
    strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S", localtime(&today));

void WriteHead();
void WrongUsage();
#endif