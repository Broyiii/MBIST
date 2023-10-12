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

struct Memory
{
    std::string mem_Name = "";
    std::string mem_Path = "";
    int up_bound = 0;
    int low_bound = 0;
    std::map<std::string,int> Algorithms;
    std::string Clock_Siganls;
    int NumberOfWords = 0;
    
};

class Parser
{
public:
    Parser(std::string mem_list,std::string def,std::string lib)
    {
        this->memorylist_file = mem_list;
        this->def_file = def;
        this->lib_file = lib;
        printf("**************************************************************************\n");
        printf("*                          Partitioning Context                          *\n");
        printf("**************************************************************************\n");
        printf("Partition Parameters:\n");
        printf("    - f file:                         %s\n", mem_list.c_str());
        printf("    - def file:                       %s\n", def.c_str());
        printf("--------------------------------------------------------------------------\n");
    }


    std::string memorylist_file;                                // memoory_list inputfile
    std::string def_file;                                       // def path inputfile
    std::string datasheet_file;                                 // datasheet inputfile
    std::string verilog_file;                                   // verilog inputfile
    std::string lib_file;                                       // lib input file
    std::map<std::string,std::map<std::string,Memory>> memorys; // memory_name / memory_path / memory
    std::vector<std::string> lib_name;
    void GetInformationFromFile();

    std::vector<std::string> ds_folder;
    std::vector<std::string> lib_folder;
    std::vector<std::string> lvlib_folder;
    std::vector<std::string> verilog_folder;

    

private:
    void ParserMemList();
    void ParserDataSheet();
    void ParserDef();
    void ParserLib(std::string lib);
    void ParserVerilog();
    void GetFileNameFromFolder(std::string path, std::vector<std::string>& filenames);
    void GetAllFileNames();
    void Print();

};


#endif