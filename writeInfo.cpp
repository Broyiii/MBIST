#include "writeInfo.hpp"
#include "parser.hpp"

extern Logger& logger;

void WriteHead()
{
    GET_DATE;
    printf("+========================================================================+\n");
    printf("|                                                                        |\n");
    printf("|                            MBIST Partitioner                           |\n");
    printf("|                                                                        |\n");
    printf("|                      Author  : eda230702                               |\n");
    printf("|                      Version : 2023-11-28  (V1.00)                     |\n");
    printf("|                      Date    : %s                     |\n", tmp);
    printf("+========================================================================+\n\n");

    logger.log("+========================================================================+");
    logger.log("|                                                                        |");
    logger.log("|                            MBIST Partitioner                           |");
    logger.log("|                                                                        |");
    logger.log("|                      Author  : eda230702                               |");
    logger.log("|                      Version : 2023-11-28  (V1.00)                     |");
    logger.log("|                      Date    : " + (std::string)tmp + "                     |");
    logger.log("+========================================================================+");
}


void WrongUsage()
{
    std::cerr << "Usage:\n"
              << "\t./MBISTPart [-command] <parameter>\n"
              << "\nRequired commands:\n"
              << "\t-d <work_directory>             | input a work directory\n"
              << "\nOptional commands:\n"
              << "\t-p <double>                     | input max power \n"
              << "\t-l <long long>                  | input max distance \n"
              << "\t-b <int>                        | input max design heirarchical distance \n"
              << "\t-f <0/1/2>                      | fast mode , defalut is auto choose by memory number \n"
              << "\t                                | \tbig number means the faster running time and the worse result \n"
              << "\t-t <int>                        | multi thread , threads number = [-t] + 1 , defalut is 0\n"
              << "\t-log <1/0>                      | generate log file , defalut is false\n"
              << "\t-plt <1/0>                      | generate plt file , which is used for MBISTPlot , defalut is false\n"
              << "\t-check <1/0>                    | check if the results are correct , defalut is false\n"
              << "\t-help                           | show usage info\n"
              
              << std::endl;
}


void Parser::PrintMemInfo()
{
    printf("Constraint Infomation:\n");
    printf("    - Memory Number:                  %0ld\n", memorysMappedByPath.size());
    if (this->distanceCon)
    {
        if (db.ManhattanDis)
            printf("    - Calculate Distance Function:    Manhattan\n");
        else
            printf("    - Calculate Distance Function:    Euclidean\n");
        printf("    - Units Distance Microns:         %0d\n", db.distance_unit);
        printf("    - Physical Distance:              %lld\n", db.dis_max);
        printf("    - Heirarchical Distance:          unlimited\n");
    }
    else
    {
        printf("    - Physical Distance:              unlimited\n");
        printf("    - Heirarchical Distance:          %0d\n", db.block_max);
    }
    
    if (this->powerCon)
        printf("    - Power:                          %.4f\n", db.power_max);
    else
        printf("    - Power:                          unlimited\n");

    if (this->clkCon)
        printf("    - CLK:                            %0d different domains\n", this->clkDomainNum);
    else   
        printf("    - CLK:                            unlimited\n");
    
    printf("--------------------------------------------------------------------------\n\n");
    if (!db.ManhattanDis)
        db.dis_max *= db.dis_max;
}

void Parser::PrintResult(std::chrono::duration<double> duration, bool parseSuccess)
{
    int cnt = 0;
    if (parseSuccess)
    {
        for (auto &k : AfterGroupBypower)
        {
            // k.first.Print(k.first);
            if (db.logFlag) ("[PrintResult] ");
            if (db.logFlag) logger.log("[PrintResult] ==========================================================================");
            if (db.logFlag) logger.log("[PrintResult] Hard Condition : " + k.first.GetInfo((k.first)));
            if (db.logFlag) logger.log("[PrintResult] ==========================================================================");
            for (auto& mList : k.second)
            {
                // printf("==========================================================================\n");
                if (db.logFlag) logger.log("[PrintResult] - total power = " + std::to_string(mList.totalPower));
                // printf("- total power = %.4f\n", mList.totalPower);
                // std::cout << mList.totalPower << "=====" << std::endl;
                for (auto& mem : mList.memList)
                {
                    // std::string block = "";
                    // for (auto &b : mem->Block)
                    // {
                    //     block += (b + "/");
                    // }
                    // logger.log("[PrintResult]     - " + block);
                    // std::cout << "    - " << mem->mem_Path << " : " << mem->dynamic_power << std::endl;
                    if (db.logFlag) logger.log("[PrintResult]     - " + mem->mem_Path + " : " + std::to_string(mem->dynamic_power));
                    ++cnt;
                }
                if (db.logFlag) logger.log("[PrintResult] ");
                // printf("\n");
            }
        }
        if (db.logFlag) logger.log("[PrintResult] single mem: " + std::to_string(cnt));
        // printf("Parse Success !\n\n");
        // std::cout << "single mem: " << cnt << std::endl;
    }
    else
    {
        if (db.logFlag) logger.log("[PrintResult] Parse Error !");
        // printf("Parse Error !\n\n");
    }

    auto running_time = duration.count();
    printf("**************************************************************************\n");
    printf("*                          Partitioning Result                           *\n");
    printf("**************************************************************************\n");
    if (parseSuccess)
    {  
        printf("    - Totol Group Number:             %0d\n", this->groupNum);
        printf("    - Totol Memory Number:            %0d\n", cnt);
        printf("    - Output File:                    %s\n", db.output_file_name.c_str());
    }
    else
    {
        printf("    - Totol Group Number:             WRONG\n");
        printf("    - Totol Memory Number:            WRONG\n");
        printf("    - Output File:                    WRONG\n");
    }
    printf("    - Log File:                       ./log/logfile.log\n");
    printf("    - Running Time:                   %.4f s\n", running_time);
    printf("--------------------------------------------------------------------------\n\n");    


    if (parseSuccess)
        printf("----------------------- MBIST Partitioner SUCCESS ! ----------------------\n\n");
    else
        printf("------------------------ MBIST Partitioner WRONG ! -----------------------\n\n");
    
}

void Parser::WriteAnswer() 
{
    db.outputFile = fopen(db.output_file_name.c_str(), "w");
    for (auto &g : AfterGroupBypower)
    {
        for (auto& mList : g.second)
        {
            fprintf(db.outputFile, "controller_%0d\n", ++this->groupNum);
            for (auto mem : mList.memList)
            {
                fprintf(db.outputFile, "%s\n", mem->mem_Path.c_str());
            }
            fprintf(db.outputFile, "\n");
        }
    }
}

