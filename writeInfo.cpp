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
              << "\t-p <double>                         | input max power \n"
              << "\t-l <int>                            | input max dis \n"
              << "\t-help                               | show usage info\n"
              << "\t-info <true/false>                  | print detailed info , defalut is false\n"
              
              << std::endl;
}


void Parser::PrintMemInfo()
{
    printf("Constraint Infomation:\n");
    printf("    - Memory number:                     %0ld\n", memorysMappedByPath.size());
    printf("    - Area:                              %.4f um2\n", 1500.1);
    printf("    - Power:                             %.4f w\n", 1.5);
    printf("--------------------------------------------------------------------------\n\n");
}

void Parser::PrintResult(std::chrono::duration<double> duration)
{
    auto running_time = duration.count();
    printf("**************************************************************************\n");
    printf("*                          Partitioning Result                           *\n");
    printf("**************************************************************************\n");
    printf("    - Running Time = %.4f s\n", running_time);
    printf("--------------------------------------------------------------------------\n\n");
    for (auto &k  : AfterHardCondition[0])
    {
        std::cout << k.first << " : " << std::endl;
        for (auto &s : k.second)
        {
            std::cout << "\t" << s->mem_Path + '/' + s->mem_Name << std::endl;
        }
        std::cout << std::endl;
    }


    int cnt = 0;
    for (auto i : AfterGroupBypower)
    {
        auto it = i.Groups.begin();
        std::cout << "Group ID: " << i.id <<  " power_sum: " << i.total_power << " Type: " << i.memType << " Algo: " << i.algo << std::endl;
        std::cout << "group size: " << i.Groups.size() << std::endl;
        while (it != i.Groups.end())
        {
            //std::cout << (*it)->mem_Path + '/' + (*it)->mem_Name << std::endl;
            it++;
            cnt++;
        }
    }
    std::cout << "single mem: " << cnt << std::endl;
}