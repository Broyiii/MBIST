#ifndef _PARSER_HPP_
#define _PARSER_HPP_

#include "global.hpp"

typedef std::map<std::string,std::set<Memory*>> grouptype;

extern dataBase db;

class Parser
{
public:
    Parser(std::string wd)
    {
        if (wd.back() == '/')
            db.work_dir = wd;
        else
            db.work_dir = wd + "/";
        
        std::vector<std::string> filenames;
        GetFileNameFromFolder(db.work_dir, filenames);
        for (auto &fn : filenames)
        {
            if (fn.find(".f") != std::string::npos)
                db.memorylist_file = fn;
            else if (fn.find(".def") != std::string::npos)
                db.def_file = fn;
            else if ((fn.find("clk") != std::string::npos) && (fn.find(".txt") != std::string::npos))
                db.clk_file = fn;
        }

        AfterHardCondition.resize(1);

        
        printf("**************************************************************************\n");
        printf("*                          Partitioning Context                          *\n");
        printf("**************************************************************************\n");
        printf("Partition Parameters:\n");
        printf("    - Work directory:                 %s\n", db.work_dir.c_str());
        printf("    - Memory list file:               %s\n", db.memorylist_file.c_str());
        printf("    - Memory def file:                %s\n", db.def_file.c_str());
        printf("--------------------------------------------------------------------------\n");
    }

    void GetInformationFromFile();
    void Print();
    void PrintMemInfo();
    void PrintResult(std::chrono::duration<double> duration);
    

private:


    std::unordered_map<std::string, Memory*> memorysMappedByPath;            // mem_path / mems
    std::unordered_map<std::string, std::set<Memory*>> memorysMappedByName;  // mem_name / mems
    std::unordered_map<std::string, std::vector<std::string>> clkDomainMap;  // clk / mem_path

    std::vector<grouptype> AfterHardCondition;
    std::vector<Group> AfterGroupBypower;

    void ParseMemList();
    void ParseDataSheet(std::string ds);
    void ParseDef();
    void ParseLvlib(std::string lvlib);
    void ParseLib(std::string lib);
    void ParseCLK();
    void ParseVerilog();
    void GetFileNameFromFolder(std::string path, std::vector<std::string>& filenames);
    void GetAllFileNames();


    void GroupByClk(grouptype &origin);
    void GroupByType(grouptype &origin);
    void GroupByAlgorithm(grouptype &origin);
    bool IsSameGroupDealMultiAlgo(Memory* a,Group group);
    bool IsPutInOneGroup(Memory* in,Memory* out,double &power);
    void GroupByHardCondition();
    void GroupByConstraints();


    void GroupByPower(grouptype &origin, std::vector<Group> &Group_Power);
    void GroupOneBypower(std::list<Memory*> &oneset, int &id, std::vector<Group> &Group_Power);
};


#endif