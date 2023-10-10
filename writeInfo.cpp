#include "writeInfo.hpp"

void WriteHead()
{
    GET_DATE;
    printf("+========================================================================+\n");
    printf("|                                                                        |\n");
    printf("|                            MBIST Partitioner                           |\n");
    printf("|                                                                        |\n");
    printf("|                 Author : @Broyiii,LZX11111111,Sanmu6666                |\n");
    printf("|                     Copyright (C) 2023 MPartioners                     |\n");
    printf("|                      Version : 2023-10-09 (V0.01)                      |\n");
    printf("|                      Date    : %s                     |\n", tmp);
    printf("+========================================================================+\n\n");
}


void WrongUsage()
{
    std::cerr << "Usage:\n"
              << "\t./MBISTPart [-command] <parameter>\n"
              << "\nRequired commands:\n"
              << "\t-f   <f_file_directory>             | input a .f file\n"
              << "\t-def <def_file_directory>           | input a .def file\n"
              << "\t-lib <lib_file_directory>           | input a .lib file\n"
              << "\nOptional commands:\n"
              << "\t-help                               | show usage info\n"
              << "\t-info <true/false>                  | print detailed info , defalut is false\n"
              << std::endl;
}
