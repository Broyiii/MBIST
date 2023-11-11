#ifndef _GLOBAL_HPP_
#define _GLOBAL_HPP_

#include <algorithm>
#include <iomanip>
#include <math.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sys/types.h>
#include <dirent.h>
#include <cstring>
#include <unistd.h>
#include <regex>
#include <unordered_map>
#include <queue>
#include <list>
#include <set>
#include <random>
#include <chrono>
#include <deque>
#include <mutex>


// define memory type
#define RAM 0
#define ROM 1


class rule{
public:
    bool operator()(const int &v1, const int &v2) const {
        return v1 > v2; // 实现从大到小排序
    }
};


struct Memory
{
    std::string mem_Name = "";
    std::string mem_Path = "";
    std::string timing_pin = "";
    std::string clk_domain = "";

    std::vector<std::string> Algorithms;
    std::vector<std::string> Clock_Siganls;


    long long up_bound = 0;
    long long low_bound = 0;
    int NumberOfWords = 0;
    float area = 0.0;
    int mem_type = RAM;
    int address_width = 0;
    int word_width = 0;
    float MilliWattsPerMegaHertz = 0.0;
    float leakage_power = 0.0;
    float dynamic_power = 0.0;
    float total_power = 0.0;
    bool flag_multi = false;
    int node_id;
    std::set<int> tmp;
    std::deque<int> connectedMems;

    static bool compareMyClass(const Memory* v1, const Memory* v2) {
        return v1->total_power > v2->total_power;
    }

    bool operator()(int v1,int v2) const
    {
        return v1 > v2;
    }

    std::deque<int> FindBothNeigbor(std::deque<int> P)
    {
        if (P.empty())
            return {};
        std::deque<int> res;
        std::set_intersection(this->connectedMems.begin(), this->connectedMems.end(), 
                              P.begin(), P.end(), std::inserter(res, res.begin()));
        return res;
    }

    std::deque<int> FindDifference(std::deque<int> S)
    {
        if (S.empty())
            return {};
        std::deque<int> mid;
        std::set_intersection(this->connectedMems.begin(), this->connectedMems.end(), 
                              S.begin(), S.end(), std::inserter(mid, mid.begin()));


        std::deque<int> res;
        std::set_difference(S.begin(),S.end(),
                            mid.begin(),mid.end(),std::back_inserter(res));

        return res;
    }
};

// bool id_cmp(Memory* a,Memory* b)
// {
//     return a->node_id > b->node_id;
// }

class Group
{
public:
    std::vector<std::string> Algos;
    std::string clkDomain = "";
    int memType = 0;

    Group() {}
    Group(Memory* mem) : memType(mem->mem_type)
    {
        if (mem->Algorithms.size() == 1)
        {
            this->Algos = mem->Algorithms;
            this->clkDomain = mem->clk_domain;
        }
        else
        {
            Algos.resize(2, "Multi");
        }
    }

    ~Group() {}

    static std::string GetInfo(Group g)
    {
        if (g.Algos.size() > 1)
            return "Alg:Multi_clk:" + g.clkDomain + "_type:" + std::to_string(g.memType);
        else
            return "Alg:" + g.Algos[0] + "_clk:" + g.clkDomain + "_type:" + std::to_string(g.memType);
    }

    static void Print(Group g)
    {
        printf("\n");
        printf("==========================================================================\n");
        std::cout << "Hard Condition : " << g.GetInfo(g) << std::endl;
        printf("==========================================================================\n");
    }

    bool operator==(const Group& other) const {
        std::string str1 = this->GetInfo(*this);
        std::string str2 = other.GetInfo(other);
        return str1 == str2;
    }
    bool operator!=(const Group& other) const {
        std::string str1 = this->GetInfo(*this);
        std::string str2 = other.GetInfo(other);
        return str1 != str2;
    }

    struct Hash {
        std::size_t operator()(const Group& obj) const {
            return std::hash<std::string>()(obj.GetInfo(obj));
        }
    };

   
};

struct GroupedMemList
{
    std::deque<Memory*> memList;
    double totalPower = 0.0;
    double maxPower;

    GroupedMemList() {}
    GroupedMemList(double mp) : maxPower(mp) {}
    GroupedMemList(double mp, Memory* mem) : maxPower(mp) 
    {
        AddMemUnsafe(mem);
    }
    GroupedMemList(double mp, std::deque<Memory*> mems) : maxPower(mp), memList(mems) 
    {
        for (auto &mem : this->memList)
        {
            this->totalPower += mem->dynamic_power;
        }
    }

    ~GroupedMemList() {}

    bool AddMem(Memory* mem)
    {
        if ((this->totalPower + mem->dynamic_power) <= this->maxPower)
        {
            this->memList.push_back(mem);
            this->totalPower += mem->dynamic_power;
            return true;
        }
        else
        {
            return false;
        }
        return false;
    }

    void AddMemUnsafe(Memory* mem)
    {
        this->memList.push_back(mem);
        this->totalPower += mem->dynamic_power;
    }

    GroupedMemList RemoveDuplicate(GroupedMemList maxList)
    {
        int indexThis = 0;
        int indexMax  = 0;
        GroupedMemList res(this->maxPower);
        while ((indexThis < this->memList.size()) && (indexMax < maxList.memList.size()))
        {
            int valueThis = this->memList[indexThis]->node_id;
            int valueMax  = maxList.memList[indexMax]->node_id;
            if (valueThis < valueMax)
            {
                res.AddMemUnsafe(this->memList[indexThis]);
                ++indexThis;
            }
            else if (valueThis == valueMax)
            {
                ++indexThis;
                ++indexMax;
            }
            else
            {
                ++indexMax;
            }
        }
        while (indexThis < this->memList.size())
        {
            res.AddMemUnsafe(this->memList[indexThis++]);
        }
        return res;
    }

    static bool Compare(const GroupedMemList v1, const GroupedMemList v2)
    {
        return v1.memList.size() > v2.memList.size();
    }

    static bool ComparePower(const Memory* v1, const Memory* v2)
    {
        return v1->dynamic_power > v2->dynamic_power;
    }

    void Print()
    {
        std::cout << this->memList.size() << " :\t";
        for (auto &mem : this->memList)
        {
            std::cout << mem->node_id << "\t";
        }
        std::cout << std::endl;
    }
};


struct dataBase
{
    std::string work_dir;
    std::string memorylist_file;                                // memoory_list inputfile
    std::string def_file;                                       // def path inputfile
    std::string datasheet_file;                                 // datasheet inputfile
    std::string verilog_file;                                   // verilog inputfile
    std::string lib_file;                                       // lib input file
    std::string clk_file;

    FILE* outputFile;
    std::string output_file_name = "output.txt";
    std::string log_file_name = "logfile.log";

    std::vector<std::string> lib_names;
    
    std::vector<std::string> ds_files; 
    std::vector<std::string> lib_files;
    std::vector<std::string> lvlib_files;
    std::vector<std::string> verilog_files;

    double power_max = 50.0;
    int dis_max = 300000;

    bool CalculateDis(Memory *a, Memory *b)
    {
        long long t = (a->low_bound - b->low_bound)*(a->low_bound - b->low_bound) + (a->up_bound - b->up_bound) * (a->up_bound - b->up_bound);
        double dis = std::sqrt(t);
        return (dis <= dis_max);
    }


};



#endif