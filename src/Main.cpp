#include "parser.cpp"
#include <algorithm>
#include <iomanip>
#include <math.h>
#include <fstream>
#include <sstream>

using namespace std;

int main()
{

    Parser parser("test.f","test.def");
    parser.GetInformationFromFile();


    return 0;
}