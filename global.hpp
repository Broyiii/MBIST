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


    int up_bound = 0;
    int low_bound = 0;
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

    
    bool operator>(const Memory* & other) const 
    {
        return total_power > other->total_power;
    }


};

class PowerCompare
{
    public:
    bool operator()(Memory* v1,Memory* v2) const
    {
        return v1->total_power > v2->total_power;
    }
};

class Group
{
    public:
        std::string algo = "";
        std::vector<std::string> Algos;
        std::string clkDomain = "";
        int memType = 0;
        double total_power = 0.0;
        int max_dis = 0;
        int id;
        std::list<Memory *> Groups;
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

    std::vector<std::string> lib_names;
    
    std::vector<std::string> ds_files; 
    std::vector<std::string> lib_files;
    std::vector<std::string> lvlib_files;
    std::vector<std::string> verilog_files;

    double power_max = 300.0;
    int dis_max = 0;
};



#endif