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

    GetNodeID();
    BuildMatric();
}


void Parser::GroupByHardCondition()
{
    for (auto &memorys : memorysMappedByPath)
    {
        auto &mem = memorys.second;
        Group g(0, mem);

        auto iter_group = AfterHardCondition.find(g);
        if (iter_group != AfterHardCondition.end())
        {
            iter_group->second.push_back(mem);
        }
        else
        {
            std::list<Memory*> tmpList({mem});
            AfterHardCondition.insert(std::pair<Group, std::list<Memory*>>(std::move(g), std::move(tmpList)));
        }
    }
}


bool Parser::GroupByPower()
{
    for (auto &maps : AfterHardCondition)
    {
        auto &memorys = maps.second;
        memorys.sort(Memory::compareMyClass);
        std::vector<GroupedMemList> groups;
        groups.resize(1, GroupedMemList(db.power_max));
        if (!groups[0].AddMem(memorys.front()))
        {
            printf("ERR CODE 4, Single mem's power is bigger than max power !\n");
            return false;
        }
        memorys.pop_front();
        NEW_LOOP_FOR_MEMS:
        for (auto &mem : memorys)
        {
            for (auto& memList : groups)
            {
                if (memList.AddMem(memorys.front()))
                {
                    memorys.pop_front();
                    goto NEW_LOOP_FOR_MEMS;
                }
            }
            groups.emplace_back(GroupedMemList(db.power_max, mem));
            memorys.pop_front();
            goto NEW_LOOP_FOR_MEMS;
        }

        AfterGroupBypower.insert(std::pair<Group, std::vector<GroupedMemList>>(maps.first, std::move(groups)));
    }
    return true;
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

void Parser::GetNodeID()
{
    int k = 0;
    for (auto &i : memorysMappedByPath)
    {
        i.second->nodes_id = k;
        k++;
    }
}

void Parser::BuildMatric()
{
    for (auto &i : memorysMappedByPath)
    {
        for (auto &j : memorysMappedByPath)
        {
            if (j.second->nodes_id > i.second->nodes_id)
            {
                if (CalculateDis(i.second,j.second))
                {
                    i.second->conncect_nodse.insert(j.second->nodes_id);
                }
                else
                {
                    continue;
                }
            }
            else
            {
                continue;
            }
        }
    }
}

bool Parser::CalculateDis(Memory *a, Memory *b)
{
    long long t = (a->low_bound - b->low_bound)*(a->low_bound - b->low_bound) + (a->up_bound - b->up_bound) * (a->up_bound - b->up_bound);
    double dis = std::sqrt(t );
    //long long dis = (long long)std::sqrt((a->low_bound - b->low_bound)*(a->low_bound - b->low_bound) + (a->up_bound - b->up_bound) * (a->up_bound - b->up_bound) );
    if (dis <= db.dis_max)
    {
        return true;
    }
    else
    {
        return false;
    } 
}

bool Parser::GetInformationFromFile()
{
    // parser file
    GetAllFileNames();

    PrintMemInfo();

    // grouping
    GroupByHardCondition();
    if(!GroupByPower())
        return false;

    // GroupByConstraints();

    // Show Information
    // Print();
    WriteAnswer();
    return true;
}

