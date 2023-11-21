#ifndef _PARSER_HPP_
#define _PARSER_HPP_

#include "global.hpp"
#include "Logger.hpp"
#include "genetic.hpp"


typedef std::map<std::string,std::set<Memory*>> grouptype;

extern dataBase db;
extern Logger& logger;


class Parser
{
public:
    Parser(){}
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
            else if (fn.find(".spec") != std::string::npos)
                db.spec_file = fn;
        }

        printf("**************************************************************************\n");
        printf("*                          Partitioning Context                          *\n");
        printf("**************************************************************************\n");
        printf("Partition Parameters:\n");
        printf("    - Work directory:                 %s\n", db.work_dir.c_str());
        printf("    - Memory list file:               %s\n", db.memorylist_file.c_str());
        printf("    - Memory def file:                %s\n", db.def_file.c_str());
        printf("    - Memory clk file:                %s\n", db.clk_file.c_str());
        printf("    - Memory spec file:               %s\n", db.spec_file.c_str());
        printf("--------------------------------------------------------------------------\n");
    }

    int groupNum = 0;

    bool GetInformationFromFile();
    void Print();
    void PrintMemInfo();
    void PrintResult(std::chrono::duration<double> duration, bool parseSuccess); 

    std::string Id2Mem(int id)
    {
        return this->memId2memPath[id];
    }
    Memory* Str2Mem(std::string str)
    {
        return this->memorysMappedByPath[str];
    }

    //BK
    void GetMaxClique(std::deque<Memory*> mems);
    void PrintBK();
    // std::vector<Memory*> mems;
    std::vector<GroupedMemList> GetClique()
    {
        return this->maxNodes;
    }

private:
    bool distanceCon = true;
    bool powerCon = true;
    bool clkCon = true;
    int clkDomainNum = 0;

    Population *population;

    std::unordered_map<std::string, Memory*> memorysMappedByPath;            // mem_path / mems
    std::unordered_map<std::string, std::set<Memory*>> memorysMappedByName;  // mem_name / mems

    // std::unordered_map<int,std::string> memId2memPath;
    std::vector<std::string> memId2memPath;

    // std::vector<Group*> AfterGroupBypower;
    std::unordered_map<Group, std::deque<Memory*>, Group::Hash> AfterHardCondition;
    std::unordered_map<Group, std::vector<GroupedMemList>, Group::Hash> AfterGroupByDis;
    // std::vector<std::map<int, std::vector<std::deque<int>>,rule>> AfterGroupByDis;
    std::unordered_map<Group, std::vector<GroupedMemList>, Group::Hash> AfterGroupBypower;

    bool ParseMemList();
    void ParseDataSheet(std::string ds);
    void ParseSUMM(std::string ds);
    void ParseDS_summ(std::string ds, double &power, double &width, double &height);
    void ParseDS_ds02(std::string ds, double &power, double &width, double &height);
    double GetLastDouble(std::string line);
    bool ParseDef();
    void ParseSpec();
    void ParseLvlib(std::string lvlib);
    void ParseLib(std::string lib);
    bool ParseCLK();
    // void ParseVerilog();
    bool GetFileNameFromFolder(std::string path, std::vector<std::string>& filenames);
    bool GetAllFileNames();
    std::string GetInfoFromStr(std::string &line);
    AddrMap GetLogicalAddressMaPInfoUtil(std::string &line, size_t &pos);

    void GroupByHardCondition();
    void GroupMultiAlgoMems(Memory *mem);
    void GroupByDistance();
    bool GroupByPower();
    std::vector<GroupedMemList> GroupOneListByPower(std::vector<GroupedMemList> memsGroup);
    
    void BuildMatric();

    void WriteAnswer();
    void OutCsvFile();


    bool SatisfyPowerCon(std::unordered_map<Group, std::vector<GroupedMemList>, Group::Hash> &groups);

    //BK
    bool CheckLackNodes(std::unordered_map<Group, std::vector<GroupedMemList>, Group::Hash> &groups);
    void BronKerbosh(std::deque<int> R, std::deque<int> P, std::deque<int> S);
    std::vector<GroupedMemList> maxNodes;
    // int num;
    bool* check;
    std::vector<GroupedMemList> RemoveDuplicateMems();
    std::vector<GroupedMemList> RemoveDuplicateMems_t();
    std::vector<GroupedMemList> RemoveDuplicateMems_for_DFS(std::vector<DuplicateMem> &DuplicateMems, std::vector<GroupedMemList> res);

    bool SatisfyDisCon(std::unordered_map<Group, std::vector<GroupedMemList>, Group::Hash> &groups);
    bool CheckSort(std::deque<Memory *> mems);

    std::vector<GroupedMemList> ViolentSearch();
    void DFS(int num, std::vector<GroupedMemList> groups, std::vector<DuplicateMem> &RestMems, int &minGroupNum, std::vector<GroupedMemList> &minGroup);

    void PrintMems()
    {
        for (auto &mem : this->memorysMappedByPath)
        {
            logger.log("[PrintMems] =======================================");
            logger.log("[PrintMems] Name   : " + mem.second->mem_Name);
            logger.log("[PrintMems] Path   : " + mem.second->mem_Path);
            logger.log("[PrintMems] Area   : " + std::to_string(mem.second->width) + " x " + std::to_string(mem.second->height));
            logger.log("[PrintMems] Power  : " + std::to_string(mem.second->dynamic_power));
            logger.log("[PrintMems] Direct : " + mem.second->direct);
            logger.log("[PrintMems] Pos    : " + std::to_string(mem.second->up_bound) + ", " + std::to_string(mem.second->low_bound));
            logger.log("[PrintMems] =======================================");
        }
    }

};

#endif