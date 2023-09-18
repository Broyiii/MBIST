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
    void parsermemlist(std::string filename);
    void parserdatasheet(std::string filename);
    void parserdef(std::string filename);
    void parserlib(std::string filename);
    void parserverilog(std::string filename);
    void print();

    private:
    std::map<std::string,std::map<std::string,Memory>> memorys; // memory_name / memory_path / memory
    std::vector<char> buf;
    int pointer_of_buf = 0;
    int size_of_buf = 0;

};