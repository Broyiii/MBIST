#include "parser.cpp"
#include <algorithm>
#include <iomanip>
#include <math.h>
#include <fstream>
#include <sstream>

using namespace std;

int main()
{

    Parser p;
    p.ParserMemList("test.f");
    p.ParserDef("test.def");
    p.Print();

    return 0;
}