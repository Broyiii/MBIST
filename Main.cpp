#include "parser.hpp"
#include "writeInfo.hpp"

// using namespace std;

int main(int argc,char *argv[])
{
    WriteHead();

    if ((argc == 1) || (argc % 2 == 0))
    {
        WrongUsage();
        return 1;
    }

    string f_file;
    string def_file;
    string lib_file;

    int argIndex = 1;
    int requiredArgNum = 0;
    while (argIndex < (argc - 1))
    {
        string arg_str = argv[argIndex++];
        if (arg_str == "-f")
        {
            f_file = argv[argIndex++];
            printf("f_file = %s\n", f_file.c_str());
            ++requiredArgNum;
        }
        else if (arg_str == "-def")
        {
            def_file = argv[argIndex++];
            printf("def_file = %s\n", def_file.c_str());
            ++requiredArgNum;
        }
        else if (arg_str == "-lib")
        {
            lib_file = argv[argIndex++];
            printf("lib_file = %s\n", lib_file.c_str());
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

    if (requiredArgNum != 3)
    {
        WrongUsage();
        return 1;
    }

    printf("Get argv successfully !\n");

    Parser parser(f_file, def_file, lib_file);
    parser.GetInformationFromFile();


    return 0;
}