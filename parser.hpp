#ifndef _PARSER_HPP_
#define _PARSER_HPP_

#include "global.hpp"
#include "Logger.hpp"
#include "genetic.hpp"
#include "thread.hpp"


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
        
        // get output dir
        std::string tmp = "";
        size_t pos = db.work_dir.size() - 2;
        while (db.work_dir[pos] != '/')
            --pos;
        for (++pos; pos < db.work_dir.size() - 2; ++pos)
        {
            db.output_file_name += db.work_dir[pos];
            // db.log_file_name += db.work_dir[pos];
            db.output_csv_name += db.work_dir[pos];
        }
        db.output_file_name += "_memlist.group";
        // db.log_file_name += ".log";
        db.output_csv_name += ".csv";

        if (db.threadNum > 0)
            threadpool = new ThreadPool(db.threadNum);

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
        printf("    - Work Directory:                 %s\n", db.work_dir.c_str());
        printf("    - Memory List File:               %s\n", db.memorylist_file.c_str());
        printf("    - Memory Def File:                %s\n", db.def_file.c_str());
        printf("    - Memory Clk File:                %s\n", db.clk_file.c_str());
        printf("    - Memory Spec File:               %s\n", db.spec_file.c_str());
        printf("--------------------------------------------------------------------------\n");
        printf("Input Parameters:\n");
        printf("    - Fast Mode:                      %0d\n", db.BKfuntion);
        printf("    - Threads:                        %0d\n", db.threadNum + 1);
        if (db.logFlag)
            printf("    - Output Log File:                true\n");
        else
            printf("    - Output Log File:                false\n");
        if (db.pltFlag)
            printf("    - Output Plt File:                true\n");
        else
            printf("    - Output Plt File:                false\n");
        if (db.checkFlag)
            printf("    - Open Check Mode:                true\n");
        else
            printf("    - Open Check Mode:                false\n");
        printf("--------------------------------------------------------------------------\n");
    }

    int groupNum = 0;

    bool GetInformationFromFile();
    void Print();
    void PrintMemInfo();
    void PrintResult(std::chrono::duration<double> duration, bool parseSuccess); 

private:
    bool distanceCon = true;
    bool powerCon = true;
    bool clkCon = true;
    int clkDomainNum = 0;

    ThreadPool *threadpool;

    std::unordered_map<std::string, Memory*> memorysMappedByPath;            // mem_path / mems
    std::unordered_map<std::string, std::set<Memory*>> memorysMappedByName;  // mem_name / mems

    std::vector<std::string> memId2memPath;

    std::unordered_map<Group, std::deque<Memory*>, Group::Hash> AfterHardCondition;
    std::unordered_map<Group, std::vector<GroupedMemList>, Group::Hash> AfterGroupByDis;
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

    static void GroupThread(Parser* thisParser, std::pair<const Group, std::deque<Memory *>> &groupHard);
    void GroupUtil(std::pair<const Group, std::deque<Memory *>> &groupHard);
    
    void BuildMatric();

    void WriteAnswer();
    void OutCsvFile();


    bool SatisfyPowerCon(std::unordered_map<Group, std::vector<GroupedMemList>, Group::Hash> &groups);

    //BK
    std::vector<GroupedMemList> GetMaxClique(std::deque<Memory*> mems);
    void PrintBK();
    bool CheckLackNodes(std::unordered_map<Group, std::vector<GroupedMemList>, Group::Hash> &groups);
    void BronKerbosh(std::deque<int> R, std::deque<int> P, std::deque<int> S, std::vector<GroupedMemList> &maxNodes);
    // std::vector<GroupedMemList> maxNodes;
    // int num;
    bool* check;
    std::vector<GroupedMemList> RemoveDuplicateMems(std::vector<GroupedMemList> &maxNodes);
    std::vector<GroupedMemList> RemoveDuplicateMems_t(std::vector<GroupedMemList> &maxNodes);
    std::vector<GroupedMemList> RemoveDuplicateMems_for_DFS(std::vector<DuplicateMem> &DuplicateMems, std::vector<GroupedMemList> res);

    bool SatisfyDisCon(std::unordered_map<Group, std::vector<GroupedMemList>, Group::Hash> &groups);
    bool CheckSort(std::deque<Memory *> mems);

    std::vector<GroupedMemList> ViolentSearch(std::vector<GroupedMemList> &maxNodes);
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