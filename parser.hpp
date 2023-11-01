#ifndef _PARSER_HPP_
#define _PARSER_HPP_

#include "global.hpp"

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

    bool HaveMultiAlgorithms = false;

    std::unordered_map<std::string, Memory*> memorysMappedByPath;            // mem_path / mems
    std::unordered_map<std::string, std::set<Memory*>> memorysMappedByName;  // mem_name / mems
    std::map<std::string,std::vector<Memory*>> AfterDivByRowCol;
    std::unordered_map<std::string, std::vector<std::string>> clkDomainMap;  // clk / mem_path



    std::map<std::string,std::vector<Memory*>> AfterGroupByClk;
    std::map<std::string,std::vector<Memory*>> AfterGroupByType;
    std::map<std::string,std::vector<Memory*>> AfterGroupByAlgorithm;

    void ParseMemList();
    void ParseDataSheet(std::string ds);
    void ParseDef();
    void ParseLvlib(std::string lvlib);
    void ParseLib(std::string lib);
    void ParseCLK();
    void ParseVerilog();
    void GetFileNameFromFolder(std::string path, std::vector<std::string>& filenames);
    void GetAllFileNames();


    void GroupByClk();
    void GroupByType();
    void GroupByAlgorithm();
    void GroupByHardCondition();
};


#endif