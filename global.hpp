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

    double power_max = 0.0;
    double dis_max = 0.0;

    

};


#endif