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
#include <iostream>
#include <sys/types.h>
#include <dirent.h>
#include <vector>
#include <string>
#include <algorithm>
#include <cstring>
#include <unistd.h>
#include <regex>
#include <unordered_map>
#include <set>
#include <chrono>
#include <queue>
#include <list>

// define memory type
#define RAM 0
#define ROM 1



struct Memory
{
    std::string mem_Name = "";
    std::string mem_Path = "";
    std::string timing_pin = "";
    std::string clk_domain = "";

    std::vector<std::string> Algorithms;
    std::vector<std::string> Clock_Siganls;
    std::set<int> conncect_nodse;


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
    int nodes_id;

    static bool compareMyClass(const Memory* v1, const Memory* v2) {
        return v1->total_power > v2->total_power;
    }
    
    // bool operator>(const Memory* & other) const 
    // {
    //     return total_power > other->total_power;
    // }


    bool operator()(int v1,int v2) const
    {
        return v1 > v2;
    }



};

class Group
{
public:
    int id;
    std::vector<std::string> Algos;
    std::string clkDomain = "";
    int memType = 0;
    double total_power = 0.0;
    int max_dis = 0;

    Group() {}
    Group(int id) : id(id) {}
    Group(int id, Memory* mem) : id(id)
    {
        this->Algos = mem->Algorithms;
        this->clkDomain = mem->clk_domain;
        this->memType = mem->mem_type;
    }

    ~Group() {}

    static std::string GetInfo(Group g)
    {
        if (g.Algos.size() > 1)
            return "Alg:Multi_clk:" + g.clkDomain + "_type:" + std::to_string(g.memType) + 
                   "_power:" + std::to_string(g.total_power) + "_dis:" + std::to_string(g.max_dis) + "_[" + std::to_string(g.id) + "]";
        else
            return "Alg:" + g.Algos[0] + "_clk:" + g.clkDomain + "_type:" + std::to_string(g.memType) + 
                   "_power:" + std::to_string(g.total_power) + "_dis:" + std::to_string(g.max_dis) + "_[" + std::to_string(g.id) + "]";
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

    struct Hash {
        std::size_t operator()(const Group& obj) const {
            return std::hash<std::string>()(obj.GetInfo(obj));
        }
    };
};

struct GroupedMemList
{
    std::vector<Memory*> memList;
    double totalPower = 0.0;
    double maxPower;

    GroupedMemList(double mp) : maxPower(mp) {}
    GroupedMemList(double mp, Memory* mem) : maxPower(mp) 
    {
        AddMem(mem);
    }

    ~GroupedMemList() {}

    bool AddMem(Memory* mem)
    {
        if ((this->totalPower + mem->dynamic_power) <= this->maxPower)
        {
            this->memList.emplace_back(mem);
            this->totalPower += mem->dynamic_power;
            return true;
        }
        else
        {
            return false;
        }
        return false;
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

    std::vector<std::string> lib_names;
    
    std::vector<std::string> ds_files; 
    std::vector<std::string> lib_files;
    std::vector<std::string> lvlib_files;
    std::vector<std::string> verilog_files;

    double power_max = 50.0;
    int dis_max = 300000;


};



#endif