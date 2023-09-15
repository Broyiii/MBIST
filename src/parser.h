#include <iostream>
#include <string>
#include <vector>
#include <map>

class Memory
{
    public:
    std::string mem_Name = "";
    //std::string mem_Path = "";
    std::vector<std::string> mem_Paths;

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
    std::map<std::string,Memory> memorys; // memory_location / memory
    std::vector<char> buf;
    int pointer_of_buf = 0;
    int size_of_buf = 0;

};