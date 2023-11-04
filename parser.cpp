#include "parser.hpp"
#include "global.hpp"

extern dataBase db;
// using namespace std;

void Parser::GetAllFileNames()
{
    GetFileNameFromFolder(db.work_dir + "ds/", db.ds_files);
    GetFileNameFromFolder(db.work_dir + "lib/", db.lib_files);
    GetFileNameFromFolder(db.work_dir + "lvlib/", db.lvlib_files);
    GetFileNameFromFolder(db.work_dir + "verilog/", db.verilog_files);

    ParseMemList();
    ParseDef();
    ParseCLK();

    for (int i = 0; i < db.lvlib_files.size(); i++)
    {
        for (auto &j : memorysMappedByName)
        {
            if (db.lvlib_files[i].find(j.first) != std::string::npos)
            {
                ParseLvlib(db.lvlib_files[i]);
            }

            if (db.lib_files[i].find(j.first) != std::string::npos)
            {
                ParseLib(db.lib_files[i]);
            }

            if (db.ds_files[i].find(j.first) != std::string::npos)
            {
                ParseDataSheet(db.ds_files[i]);
            }

        }
    }
}

void Parser::GroupByClk(grouptype &origin)
{
    grouptype temp;
    for (auto i : origin)
    {
        for (auto j : i.second)
        {
            std::string CLK = i.first + '_' + j->clk_domain;
            if (temp.find(CLK) != temp.end())
            {
                auto it = temp.find(CLK);
                it->second.insert(j);
            }
            else
            {
                std::set<Memory*> tmp {j};
                temp.insert(std::pair<std::string,std::set<Memory*>>(CLK,std::move(tmp)));
                tmp.clear();
            }
        }
    }
    origin = temp;
}

void Parser::GroupByType(grouptype &origin)
{
    grouptype temp;
    for (auto i : origin)
    {
        for (auto j : i.second)
        {
            std::string type = i.first + '_' + std::to_string(j->mem_type);
            if (temp.find(type) != temp.end())
            {
                auto it = temp.find(type);
                it->second.insert(j);
            }
            else
            {
                std::set<Memory*> tmp {j};
                temp.insert(std::pair<std::string,std::set<Memory*>>(type,std::move(tmp)));
                tmp.clear();
            }
        }
    }
    origin = temp;
}

void Parser::GroupByAlgorithm(grouptype &origin)
{
    grouptype temp;
    for (auto i : origin)
    {
        for (auto j : i.second)
        {
            if (j->Algorithms.size() > 1)
            {
                std::string clk_type_multi = i.first + "_multi";
                if (temp.find(clk_type_multi) != temp.end())
                {
                    auto it = temp.find(clk_type_multi);
                    it->second.insert(j);
                }
                else
                {
                    std::set<Memory*> tmp {j};
                    temp.insert(std::pair<std::string,std::set<Memory*>>(clk_type_multi,std::move(tmp)));
                    tmp.clear();
                }
            }
            else
            {
                std::string clk_type_multi = i.first + '_' + j->Algorithms[0];;
                if (temp.find(clk_type_multi) != temp.end())
                {
                    auto it = temp.find(clk_type_multi);
                    it->second.insert(j);
                }
                else
                {
                    std::set<Memory*> tmp {j};
                    temp.insert(std::pair<std::string,std::set<Memory*>>(clk_type_multi,std::move(tmp)));
                    tmp.clear();
                }
            }

        }
    }
    origin = temp;
}

bool Parser::IsSameGroupDealMultiAlgo(Memory* a,Group group)
{
    if (a->mem_type == group.memType && ((a->total_power + group.total_power) <= db.power_max))
    {
        for (auto i : a->Algorithms)
        {
            if (i == group.algo)
            {
                return true;
                break;
            }
        }
    }
    else
    {
        return false;
    }
}

bool Parser::IsPutInOneGroup(Memory *in, Memory *out, double &power)
{
    power += out->total_power;
    if (power <= db.power_max)
    {
        for (auto i : out->Algorithms)
        {
            for (auto j : in->Algorithms)
            {
                if (i == j)
                {
                    return true;
                    break;
                }
            }
        }
    }
    else
    {
        return false;
    }
}

void Parser::GroupByHardCondition()
{
    GroupByAlgorithm(AfterHardCondition[0]);
    GroupByClk(AfterHardCondition[0]);
    GroupByType(AfterHardCondition[0]);
}

void Parser::GroupByConstraints()
{
    GroupByPower(AfterHardCondition[0],AfterGroupBypower);
}



void Parser::GroupByPower(grouptype &origin, std::vector<Group> &Group_Power)
{
    int id = 0;
    std::map<std::string,std::list<Memory*>> multi;
    std::vector<Group> left;

    //deal single algorithm
    for (auto i : origin)
    {
        if (i.first.find("multi") == std::string::npos)
        {
            std::list<Memory *> oneset;
            double cur = 0.0;
            for (auto j : i.second)
            {
                cur += j->total_power;
                oneset.push_back(j);
            }
            GroupOneBypower(oneset,id,Group_Power);
        }
        else
        {
            std::list<Memory*> tmp;
            for (auto j : i.second)
            {
                tmp.push_back(j);
            }
            tmp.sort(std::greater<Memory*>());
            multi.insert(std::pair<std::string,std::list<Memory*>>(i.first,tmp));
        }
    }

    //deal multi Algorithm
    for (auto i : multi)
    {
        Group p;
        auto it = i.second.front();
        while (i.second.size())
        {
            auto it1 = i.second.front();
            for (auto k : AfterGroupBypower)
            {
                if (IsSameGroupDealMultiAlgo(it,k))
                {
                    k.Groups.push_back(it);
                    i.second.pop_front();
                }
            }
            if (it1 == i.second.front())
            {
                p.memType = i.second.front()->mem_type;
                p.clkDomain = i.second.front()->clk_domain;
                p.Groups.push_back(i.second.front());
                i.second.pop_front();
            }
        }
        if (p.clkDomain != "")
        {
            left.push_back(p);
        }
    }
    if (left.size())
    {
        for (auto k : left)
        {
            
            int cnt = 0;
            int size = k.Groups.size();
            while (cnt < size)
            {
                Group P;
                double cur_power = 0.0;
                k.Groups.sort(std::greater<Memory*>());
                if (k.Groups.front()->flag_multi)
                {
                    k.Groups.pop_front();
                }
                else
                {
                    auto it = k.Groups.front();
                    cur_power += it->total_power;
                    P.Groups.push_back(it);
                    k.Groups.pop_front();
                    cnt++;

                    for (auto iter : k.Groups)
                    {
                        if (!iter->flag_multi)
                        {
                            if (IsPutInOneGroup(it,iter,cur_power));
                            {
                                P.Groups.push_back(iter);
                                iter->flag_multi = true;
                                cnt++;
                            }
                        }
                    }

                }
                P.clkDomain = k.clkDomain;
                P.memType = k.memType;
                P.total_power = cur_power;
                P.algo = "multi";
                AfterGroupBypower.push_back(P);
            }
            
        }
    }
    
}

void Parser::GroupOneBypower(std::list<Memory *> &oneset, int &id, std::vector<Group> &Group_Power)
{
    Group group;
    double power_sum = 0.0;
    oneset.sort(std::greater<Memory*>());
    auto it = oneset.begin();
    while (it != oneset.end())
    {
        power_sum += (*it)->total_power;
        it++;
    }

    if (power_sum <= db.power_max && power_sum > 0)
    {
        group.Groups = oneset;
        group.total_power = power_sum;
        group.id = id;
        group.memType = oneset.front()->mem_type;
        group.algo = oneset.front()->Algorithms[0];
        Group_Power.push_back(group);
    }
    else
    {
        std::list<Memory *> temp;
        while (power_sum > db.power_max)
        {
            power_sum -= oneset.back()->total_power;
            temp.push_back(oneset.back());
            oneset.pop_back();
        }
        group.Groups = oneset;
        group.total_power = power_sum;
        group.id = id;
        group.memType = oneset.front()->mem_type;
        group.algo = oneset.front()->Algorithms[0];
        Group_Power.push_back(group);
        id++;
        GroupOneBypower(temp, id, Group_Power);
    }
}


void Parser::Print()
{
    std::cout << "\nmemory size = " << memorysMappedByPath.size() << std::endl;
    for (auto i : memorysMappedByName)
    {
        std::cout << i.first << std::endl;
        for (auto &j : i.second)
        {
            std::cout << "Path: " << j->mem_Path << " Low_limit: " << j->low_bound << " Up_limit: " << j->up_bound << " NumberOfWords: " << j->NumberOfWords << std::endl;
            std::cout << "Algorithms: ";
            for (auto &k : j->Algorithms)
            {
                std::cout << k << " ";
            }
            std::cout << std::endl;
            std::cout << "Clock_Siganls: ";
            for (auto t : j->Clock_Siganls)
            {
                std::cout << t << " ";
            }
            std::cout << std::endl;
            std::cout << "Area: " << j->area << " mem_type: " << j->mem_type << " MilliWattsPerMegaHertz: " << j->MilliWattsPerMegaHertz << std::endl;
            std::cout << "Total power: " << j->total_power << std::endl;
            std::cout << " address_width: " << j->address_width << " word_width: " << j->word_width << "\n" << std::endl;
        }
    }
    std::cout << "------------------------------------------------------------------------------------------------" << std::endl;
    std::cout << "------------------------------------------------------------------------------------------------" << std::endl;
}

void Parser::GetInformationFromFile()
{
    // parser file
    GetAllFileNames();std::set<Memory*> tmp_set;

    for (auto it : memorysMappedByPath)
    {  
        tmp_set.insert(it.second);
    }
    AfterHardCondition[0].insert(std::pair<std::string, std::set<Memory*>>("", tmp_set));

    PrintMemInfo();

    // grouping
    GroupByHardCondition();

    GroupByConstraints();

    // Show Information
    Print();
}

