#include <iostream>
#include <string>
#include <vector>
#include <map>

class Memory
{
    public:
    std::string mem_Name = "";
    std::string mem_Path = "";
    int up_bound = 0;
    int low_bound = 0;

};

class Parser
{
    public:
    void ParserMemList(std::string filename);
    void ParserDataSheet(std::string filename);
    void ParserDef(std::string filename);
    void ParserLib(std::string filename);
    void ParserVerilog(std::string filename);
    void Print();

    private:
    std::map<std::string,std::map<std::string,Memory>> memorys; // memory_name / memory_path / memory
    std::vector<char> buf;
    int ptr_of_buf = 0;
    int size_of_buf = 0;

};