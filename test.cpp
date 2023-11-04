#include "global.hpp"
#include <iostream>
#include <set>

class compare
{
    public:
    bool operator() (Memory *v1,Memory *v2) const
    {
        return v1->total_power > v1->total_power;
    }
};

int main()
{
    std::set<Memory*, compare> a;
    for (int i = 1; i < 100; ++i)
    {
        Memory* m = new Memory;
        m->total_power = i;
        a.insert(m);
    }

    for (auto x : a)
    {
        std::cout << x->total_power << std::endl;
    }
}