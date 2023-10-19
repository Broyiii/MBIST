#include "parser.hpp"
#include "writeInfo.hpp"

int main(int argc,char *argv[])
{
    TIME_START;
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
        else
        {
            WrongUsage();
            return 1;
        }
    }

    // if (requiredArgNum != 3)
    // {
    //     WrongUsage();
    //     return 1;
    // }

    Parser parser(work_dir);
    parser.GetInformationFromFile();
    TIME_OUT;
    parser.PrintResult((double)(____end - ____start) / 1000000);
    

    return 0;
}