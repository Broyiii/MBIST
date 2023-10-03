#include "parser.cpp"
#include <algorithm>
#include <iomanip>
#include <math.h>
#include <fstream>
#include <sstream>

using namespace std;

int main(int argc,char *argv[])
{
    string f_file = argv[1];
    string def_file = argv[2];
    string lib_file = argv[3];

    Parser parser(f_file,def_file,lib_file);
    parser.GetInformationFromFile();


    return 0;
}