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
    p.parsermemlist("test.f");

    p.print();

    return 0;
}