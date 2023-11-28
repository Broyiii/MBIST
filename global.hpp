#ifndef _GLOBAL_HPP_
#define _GLOBAL_HPP_

#include <algorithm>
#include <iomanip>
#include <math.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sys/types.h>
#include <dirent.h>
#include <cstring>
#include <unistd.h>
#include <regex>
#include <unordered_map>
#include <queue>
#include <list>
#include <set>
#include <random>
#include <chrono>
#include <deque>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <atomic>
#include <sys/stat.h>

#include "Logger.hpp"

extern Logger& logger;

class rule{
public:
    bool operator()(const int &v1, const int &v2) const {
        return v1 > v2; // 实现从大到小排序
    }
};

struct AddrMap
{
    std::string name;
    int up;
    int down;
    AddrMap(std::string n, int u, int d) : name(n), up(u), down(d) {}
    ~AddrMap() {}

    std::string GetName(int offset)
    {
        return this->name + "[" + std::to_string(this->down + offset) + "]";
    }
    void Print()
    {
        // printf("%s : [%0d : %0d]\n", name.c_str(), up, down);
        // logger.log(name + " : [" + std::to_string(up) + " : " + std::to_string(down) + "]");
    }
};


struct Memory
{
    std::string mem_Name = "";
    std::string mem_Path = "";
    std::string mem_type = "";
    std::string clk_domain = "";
    std::string OperationSet = "";
    std::string logicalPorts = "";
    std::string ShadowRead = "";
    std::string ShadowWrite = "";
    std::string ShadowWriteOK = "";
    std::string WriteOutOfRange = "";
    std::string direct = "";

    std::vector<std::string> Algorithms;
    std::vector<std::string> Clock_Siganls;
    std::vector<std::string> Block;

    double MilliWattsPerMegaHertz = -1.0;
    double dynamic_power = -1.0;

    long long up_bound = 0;
    long long low_bound = 0;

    double width = 0.0;
    double height = 0.0;
    
    std::vector<std::string> LogicalAddressMap;
    
    bool flag_multi = false;
    int node_id;
    std::set<int> tmp;
    std::deque<int> connectedMems;

    bool operator==(const Memory* other) const {
        return this->mem_Path == other->mem_Path;
    }

    static bool compareMyClass(const Memory* v1, const Memory* v2) {
        return v1->dynamic_power > v2->dynamic_power;
    }

    bool operator()(int v1,int v2) const
    {
        return v1 > v2;
    }

    void GetBlock()
    {
        if (this->mem_Path.empty())
        {
            std::cout << "ERROR ! Such mem has no path !" << std::endl;
            return;
        }
        std::string b = "";
        for (auto &c : this->mem_Path)
        {
            if (c != '/')
            {
                b += c;
            }
            else
            {
                this->Block.push_back(b);
                b.clear();
            }
        }
        if (!b.empty())
        {
            this->Block.push_back(b);
        }
    }

    void AdjustPositionByDirect(int distance_unit)
    {
        if (this->direct == "N")
            return;
        else if (this->direct == "S")
        {
            this->up_bound -= (long long)(this->width * distance_unit);
            this->low_bound -= (long long)(this->height * distance_unit);
        }
        else if (this->direct == "W")
        {
            this->up_bound -= (long long)(this->width * distance_unit);
        }
        else if (this->direct == "E")
        {
            this->low_bound -= (long long)(this->height * distance_unit);
        }
        else if (this->direct == "FN")
        {
            this->up_bound -= (long long)(this->width * distance_unit);
        }
        else if (this->direct == "FS")
        {
            this->low_bound -= (long long)(this->height * distance_unit);
        }
        else if (this->direct == "FW")
        {
            return;
        }
        else if (this->direct == "FE")
        {
            this->up_bound -= (long long)(this->width * distance_unit);
            this->low_bound -= (long long)(this->height * distance_unit);
        }
        else
        {
            std::cout << "ERROR ! No such direct " << this->direct << std::endl;
        }
    }

    std::deque<int> FindBothNeigbor(std::deque<int> P)
    {
        if (P.empty())
            return {};
        std::deque<int> res;
        std::set_intersection(this->connectedMems.begin(), this->connectedMems.end(), 
                              P.begin(), P.end(), std::inserter(res, res.begin()));
        return res;
    }

    std::deque<int> FindDifference(std::deque<int> S)
    {
        if (S.empty())
            return {};
        std::deque<int> mid;
        std::set_intersection(this->connectedMems.begin(), this->connectedMems.end(), 
                              S.begin(), S.end(), std::inserter(mid, mid.begin()));

        std::deque<int> res;
        std::set_difference(S.begin(),S.end(),
                            mid.begin(),mid.end(),std::back_inserter(res));

        return res;
    }

    struct Hash {
        std::size_t operator()(const Memory* obj) const {
            return std::hash<std::string>()(obj->mem_Path);
        }
    };
};


class Group
{
public:
    std::vector<std::string> Algos;
    std::string clkDomain = "";
    std::string operationSet = "";
    std::string memType = "";
    std::string logicalPorts = "";
    std::string ShadowRead = "";
    std::string ShadowWrite = "";
    std::string ShadowWriteOK = "";
    std::string WriteOutOfRange = "";
    
    std::vector<std::string> LogicalAddressMap;

    Group() {}
    Group(Memory* mem)
    {
        if (mem->Algorithms.size() == 1)
        {
            this->memType = mem->mem_type;
            this->Algos = mem->Algorithms;
            this->clkDomain = mem->clk_domain;
            this->operationSet = mem->OperationSet;
            this->LogicalAddressMap = mem->LogicalAddressMap;
            this->logicalPorts = mem->logicalPorts;
            this->ShadowRead = mem->ShadowRead;
            this->ShadowWrite = mem->ShadowWrite;
            this->ShadowWriteOK = mem->ShadowWriteOK;
            this->WriteOutOfRange = mem->WriteOutOfRange;
        }
        else
        {
            Algos.resize(2, "Multi");
        }
    }
    Group(Memory* mem, bool GroupMulti)
    {
        if (GroupMulti)
        {
            this->memType = mem->mem_type;
            this->clkDomain = mem->clk_domain;
            this->operationSet = mem->OperationSet;
            this->LogicalAddressMap = mem->LogicalAddressMap;
        }
    }

    ~Group() {}

    static std::string GetInfo(Group g)
    {
        std::string res =   "\nAlg: " + g.Algos[0] +   
                            "\nclk: " + g.clkDomain + 
                            "\ntype: " + g.memType +
                            "\nopeartionSet: " + g.operationSet +
                            "\nlogicalPorts: " + g.logicalPorts +
                            "\nShadowRead: " + g.ShadowRead +
                            "\nShadowWrite: " + g.ShadowWrite +
                            "\nShadowWriteOK: " + g.ShadowWriteOK +
                            "\nWriteOutOfRange: " + g.WriteOutOfRange +
                            "\nLogicalAddressMap: ";
        for (auto str : g.LogicalAddressMap)
        {
            res += (str + ",");
        }
        return res;
    }

    static void Print(Group g)
    {
        printf("\n");
        printf("==========================================================================\n");
        std::cout << "Hard Condition : " << Group::GetInfo(g) << std::endl;
        printf("==========================================================================\n");
    }

    bool operator==(const Group& other) const {
        std::string str1 = this->GetInfo(*this);
        std::string str2 = other.GetInfo(other);
        return str1 == str2;
    }
    bool operator!=(const Group& other) const {
        std::string str1 = this->GetInfo(*this);
        std::string str2 = other.GetInfo(other);
        return str1 != str2;
    }

    struct Hash {
        std::size_t operator()(const Group& obj) const {
            return std::hash<std::string>()(Group::GetInfo(obj));
        }
    };

   
};

struct GroupedMemList
{
    std::deque<Memory*> memList;
    double totalPower = 0.0;
    double maxPower;

    GroupedMemList() {}
    GroupedMemList(double mp) : maxPower(mp) {}
    GroupedMemList(double mp, Memory* mem) : maxPower(mp) 
    {
        AddMemUnsafe(mem);
    }
    GroupedMemList(double mp, std::deque<Memory*> mems) : maxPower(mp), memList(mems) 
    {
        for (auto &mem : this->memList)
        {
            this->totalPower += mem->dynamic_power;
        }
    }

    ~GroupedMemList() {}

    bool AddMem(Memory* mem)
    {
        if ((this->totalPower + mem->dynamic_power) <= this->maxPower)
        {
            this->memList.push_back(mem);
            this->totalPower += mem->dynamic_power;
            return true;
        }
        else
        {
            return false;
        }
        return false;
    }

    void AddMemUnsafe(Memory* mem)
    {
        this->memList.push_back(mem);
        this->totalPower += mem->dynamic_power;
    }

    bool DelteMem(Memory* mem)
    {
        auto iter = std::find(this->memList.begin(), this->memList.end(), mem);
        if (iter != this->memList.end())
        {
            this->totalPower -= mem->dynamic_power;
            this->memList.erase(iter);
            return true;
        }
        else
        {
            return false;
        }
    }

    GroupedMemList RemoveDuplicate(GroupedMemList maxList)
    {
        int indexThis = 0;
        int indexMax  = 0;
        GroupedMemList res(this->maxPower);
        while ((indexThis < this->memList.size()) && (indexMax < maxList.memList.size()))
        {
            int valueThis = this->memList[indexThis]->node_id;
            int valueMax  = maxList.memList[indexMax]->node_id;
            if (valueThis < valueMax)
            {
                res.AddMemUnsafe(this->memList[indexThis]);
                ++indexThis;
            }
            else if (valueThis == valueMax)
            {
                ++indexThis;
                ++indexMax;
            }
            else
            {
                ++indexMax;
            }
        }
        while (indexThis < this->memList.size())
        {
            res.AddMemUnsafe(this->memList[indexThis++]);
        }
        return res;
    }

    static bool Compare(const GroupedMemList v1, const GroupedMemList v2)
    {
        return v1.memList.size() > v2.memList.size();
    }

    static bool ComparePower(const Memory* v1, const Memory* v2)
    {
        return v1->dynamic_power > v2->dynamic_power;
    }

    void Print()
    {
        std::cout << this->memList.size() << " :\t";
        for (auto &mem : this->memList)
        {
            std::cout << mem->node_id << "\t";
        }
        std::cout << std::endl;
    }
};

struct DuplicateMem 
{
    Memory* mem; 
    std::vector<int> groupIDs;
    static bool ComparePower(const DuplicateMem v1, const DuplicateMem v2)
    {
        return v1.mem->dynamic_power > v2.mem->dynamic_power;
    }
};


struct dataBase
{
    std::string work_dir;
    std::string memorylist_file;                                // memoory_list inputfile
    std::string def_file;                                       // def path inputfile
    std::string datasheet_file;                                 // datasheet inputfile
    std::string verilog_file;                                   // verilog inputfile
    std::string lib_file;                                       // lib input file
    std::string clk_file;
    std::string spec_file;

    FILE* outputFile;
    std::string output_file_name = "./output/";
    std::string log_file_name = "./log/";
    std::string output_csv_name = "./plt/plt_";
    FILE* outcsvFile;
    
    int distance_unit = -1;
    bool inputBlock = false;
    int threadNum = 0;
    bool ManhattanDis = false;
    bool checkFlag = false;
    bool logFlag = false;
    bool pltFlag = false;

    std::vector<std::string> lib_names;
    
    std::vector<std::string> ds_files; 
    std::vector<std::string> summ_files; 
    std::vector<std::string> lib_files;
    std::vector<std::string> lvlib_files;
    std::vector<std::string> verilog_files;

    int BKfuntion = 0;

    double power_max = 0.0;
    long long dis_max = 0;
    int block_max = 1;

    bool CalculateDis(Memory *a, Memory *b)
    {
        long long t;
        if (!ManhattanDis)
            t = (a->low_bound - b->low_bound)*(a->low_bound - b->low_bound) + (a->up_bound - b->up_bound) * (a->up_bound - b->up_bound);
        else
        {
            long long tmp_x = a->up_bound - b->up_bound;
            long long tmp_y = a->low_bound - b->low_bound;
            if (tmp_x < 0) 
                tmp_x = -tmp_x;
            if (tmp_y < 0) 
                tmp_y = -tmp_y;
            t = tmp_x + tmp_y;
        }
        // double dis = std::sqrt(t);
        if (logFlag) logger.log("[CalculateDis] " + std::to_string(t) + " : " + std::to_string(dis_max) + " : " + std::to_string(t <= dis_max));
        return (t <= dis_max);
    }

    bool CalculateBlockCon(Memory *a, Memory *b)
    {
        int bigSize = a->Block.size();
        int smallSize = b->Block.size();
        if (bigSize < smallSize)
        {
            std::swap(bigSize, smallSize);
        }

        int sameBlockNum = 0;
        for (int index = 0; index < smallSize; ++index)
        {
            if (a->Block[index] == b->Block[index])
            {
                ++sameBlockNum;
            }
            else
            {
                break;
            }
        }

        return ((bigSize - sameBlockNum - 1) <= this->block_max);
    }


};



#endif