#ifndef _PARSER_H_
#define _PARSER_H_

#include <algorithm>
#include <iomanip>
#include <math.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

struct Memory
{
    std::string mem_Name = "";
    std::string mem_Path = "";
    int up_bound = 0;
    int low_bound = 0;
    std::map<std::string,int> Algorithms;
    std::map<std::string,int> Clock_Siganls;
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

    }


    std::string memorylist_file;                                // memoory_list inputfile
    std::string def_file;                                       // def path inputfile
    std::string datasheet_file;                                 // datasheet inputfile
    std::string verilog_file;                                   // verilog inputfile
    std::string lib_file;                                       // lib input file
    std::map<std::string,std::map<std::string,Memory>> memorys; // memory_name / memory_path / memory
    std::vector<Memory> Memorys;
    void GetInformationFromFile();

private:
    void ParserMemList();
    void ParserDataSheet();
    void ParserDef();
    void ParserLib();
    void ParserVerilog();
    void Print();
    void GetAllMemory();

    std::vector<char> buf;
    int ptr_of_buf = 0;
    int size_of_buf = 0;
    

};


#endif