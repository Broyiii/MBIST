#include "parser.hpp"
#include "writeInfo.hpp"

int main(int argc,char *argv[])
{
    TIME_START;

    WriteHead();

    // if ((argc == 1) || (argc % 2 == 0))
    // {
    //     WrongUsage();
    //     return 1;
    // }

    std::string f_file = "/home/lzx/git_mbist/MBIST/MBIST/demo/tc03_memorylist_c.f";
    std::string def_file = "/home/lzx/git_mbist/MBIST/MBIST/demo/tc03_memory_pos_c.def";
    std::string lib_file = "";

    int argIndex = 1;
    int requiredArgNum = 0;
    while (argIndex < (argc - 1))
    {
        std::string arg_str = argv[argIndex++];
        if (arg_str == "-f")
        {
            f_file = argv[argIndex++];
            ++requiredArgNum;
        }
        else if (arg_str == "-def")
        {
            def_file = argv[argIndex++];
            ++requiredArgNum;
        }
        else if (arg_str == "-lib")
        {
            lib_file = argv[argIndex++];
            ++requiredArgNum;
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

    Parser parser(f_file,def_file,lib_file);
    parser.GetInformationFromFile();

    TIME_OUT;
    printf("\n- Running Time = %0f s\n", (double)(____end - ____start) / 1000000);

    return 0;
}