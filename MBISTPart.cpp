#include "parser.hpp"
#include "writeInfo.hpp"
#include "global.hpp"

dataBase db;


int main(int argc,char *argv[])
{
    auto startTime = std::chrono::high_resolution_clock::now();
    std::ios::sync_with_stdio(0),std::cin.tie(0),std::cout.tie(0);

    WriteHead();

    if ((argc == 1) || (argc % 2 == 0))
    {
        WrongUsage();
        return 1;
    }
    
    std::string work_dir;

    int argIndex = 1;
    int requiredArgNum = 0;
    while (argIndex < (argc - 1))
    {
        std::string arg_str = argv[argIndex++];
        if (arg_str == "-d")
        {
            int pathExist = access(argv[argIndex], F_OK);
            if (pathExist == 0)
            {
                work_dir= argv[argIndex++];
            }
            else
            {
                printf("ERROR ! No such directory [ %s ] !\n\n", argv[argIndex]);
                return 1;
            }
            // ++requiredArgNum;
        }
        else if (arg_str == "-info")
        {
            // print detailed info
        }
        else if (arg_str == "-p")
        {
            std::string tmp = argv[argIndex++];
            db.power_max = std::stod(tmp);
        }
        else if (arg_str == "-l")
        {
            std::string tmp = argv[argIndex++];
            db.dis_max = std::stoi(tmp);
        }
        else
        {
            WrongUsage();
            return 1;
        }
    }

    Parser parser(work_dir);
    bool successFlag = parser.GetInformationFromFile();
    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = endTime - startTime;
    parser.PrintResult(duration, successFlag);
    

    return 0;
}