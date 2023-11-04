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
    printf("|                      Version : 2023-11-05 (V0.10)                      |\n");
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
    printf("    - Distance:                          %0d um\n", db.dis_max);
    printf("    - Power:                             %.4f uA/MHz\n", db.power_max);
    printf("--------------------------------------------------------------------------\n\n");
}

void Parser::PrintResult(std::chrono::duration<double> duration, bool parseSuccess)
{
    auto running_time = duration.count();
    printf("**************************************************************************\n");
    printf("*                          Partitioning Result                           *\n");
    printf("**************************************************************************\n");
    printf("    - Running Time = %.4f s\n", running_time);
    printf("--------------------------------------------------------------------------\n\n");
    
    // int num = 0;
    // for (auto &k : AfterHardCondition)
    // {
    //     k.first.Print(k.first);
    //     for (auto &mem : k.second)
    //     {
    //         std::cout << mem->mem_Path << std::endl;
    //         ++num;
    //     }
    // }
    // std::cout << "\nnum = " << num << std::endl;

    if (parseSuccess)
    {
        int cnt = 0;
        for (auto &k : AfterGroupBypower)
        {
            k.first.Print(k.first);
            for (auto& mList : k.second)
            {
                // printf("==========================================================================\n");
                printf("- total power = %.4f\n", mList.totalPower);
                // std::cout << mList.totalPower << "=====" << std::endl;
                for (auto& mem : mList.memList)
                {
                    std::cout << "    - " << mem->mem_Path << " : " << mem->dynamic_power << std::endl;
                    ++cnt;
                }
                printf("\n");
            }
        }
        std::cout << "single mem: " << cnt << std::endl;
    }
    else
    {
        printf("Parse Error !\n");
    }
    
}


void Parser::WriteAnswer()
{
    db.outputFile = fopen(db.output_file_name.c_str(), "w");
    int i = 0;
    for (auto &g : AfterGroupBypower)
    {
        for (auto& mList : g.second)
        {
            fprintf(db.outputFile, "controller_%0d\n", ++i);
            for (auto mem : mList.memList)
            {
                fprintf(db.outputFile, "%s\n", mem->mem_Path.c_str());
            }
            fprintf(db.outputFile, "\n");
        }
    }
}