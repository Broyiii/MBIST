#ifndef _PARSER_HPP_
#define _PARSER_HPP_

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

// define memory type
#define RAM 0




struct Memory
{
    std::string mem_Name = "";
    std::string mem_Path = "";
    std::string timing_pin = "";

    std::map<std::string,int> Algorithms;
    std::vector<std::string> Clock_Siganls;


    int up_bound = 0;
    int low_bound = 0;
    int NumberOfWords = 0;
    float area = 0.0;
    int mem_type = RAM;
    int address_width = 0;
    int word_width = 0;
    float MilliWattsPerMegaHertz = 0.0;
    float total_power = 0.0;
};

class Parser
{
public:
    Parser(std::string wd)
    {
        if (wd.back() == '/')
            this->work_dir = wd;
        else
            this->work_dir = wd + "/";
        
        std::vector<std::string> filenames;
        GetFileNameFromFolder(this->work_dir, filenames);
        for (auto &fn : filenames)
        {
            if (fn.find(".f") != std::string::npos)
                this->memorylist_file = fn;
            else if (fn.find(".def") != std::string::npos)
                this->def_file = fn;
        }
        
        printf("**************************************************************************\n");
        printf("*                          Partitioning Context                          *\n");
        printf("**************************************************************************\n");
        printf("Partition Parameters:\n");
        printf("    - Work directory:                 %s\n", work_dir.c_str());
        printf("    - Memory list file:               %s\n", memorylist_file.c_str());
        printf("    - Memory def file:                %s\n", def_file.c_str());
        printf("--------------------------------------------------------------------------\n");
    }

    void GetInformationFromFile();
    void Print();
    void PrintMemInfo();
    void PrintResult(double running_time);
    

private:
    std::string work_dir;
    std::string memorylist_file;                                // memoory_list inputfile
    std::string def_file;                                       // def path inputfile
    std::string datasheet_file;                                 // datasheet inputfile
    std::string verilog_file;                                   // verilog inputfile
    std::string lib_file;                                       // lib input file
    std::map<std::string,std::map<std::string,Memory>> memorys; // memory_name / memory_path / memory
    
    std::vector<std::string> lib_names;
    
    std::vector<std::string> ds_files; 
    std::vector<std::string> lib_files;
    std::vector<std::string> lvlib_files;
    std::vector<std::string> verilog_files;

    std::map<std::string,std::vector<Memory>> AfterDivByRowCol;

    void ParseMemList();
    void ParseDataSheet(std::string ds);
    void ParseDef();
    void ParseLvlib(std::string lvlib);
    void ParseLib(std::string lib);
    void ParseVerilog();
    void GetFileNameFromFolder(std::string path, std::vector<std::string>& filenames);
    void GetAllFileNames();
    void DivByRowCol();

};


#endif