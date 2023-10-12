#include "writeInfo.hpp"

void WriteHead()
{
    GET_DATE;
    printf("+========================================================================+\n");
    printf("|                                                                        |\n");
    printf("|                            MBIST Partitioner                           |\n");
    printf("|                                                                        |\n");
    printf("|                 Author : @Broyiii,LZX11111111,Sanmu6666                |\n");
    printf("|                    Copyright (C) 2023 M3CPartioners                    |\n");
    printf("|                      Version : 2023-10-12 (V0.01)                      |\n");
    printf("|                      Date    : %s                     |\n", tmp);
    printf("+========================================================================+\n\n");
}


void WrongUsage()
{
    std::cerr << "Usage:\n"
              << "\t./MBISTPart [-command] <parameter>\n"
              << "\nRequired commands:\n"
              << "\t-d <work_directory>                 | input a work directory\n"
              << "\nOptional commands:\n"
              << "\t-help                               | show usage info\n"
              << "\t-info <true/false>                  | print detailed info , defalut is false\n"
              << std::endl;
}


void Parser::PrintMemInfo()
{
    printf("Constraint Infomation:\n");
    printf("    - Memory number:                     %0ld\n", memorys.size());
    printf("    - Area:                              %.4f um2\n", 1500.1);
    printf("    - Power:                             %.4f w\n", 1.5);
    printf("--------------------------------------------------------------------------\n\n");
}

void Parser::PrintResult(double running_time)
{
    printf("**************************************************************************\n");
    printf("*                          Partitioning Result                           *\n");
    printf("**************************************************************************\n");
    printf("    - Running Time = %.4f s\n", running_time);
    printf("--------------------------------------------------------------------------\n\n");
}