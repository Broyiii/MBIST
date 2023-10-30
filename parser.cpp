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

void Parser::DivByRowCol()
{
    for (auto i : memorysMappedByName)
    {
        for (auto j : i.second)
        {
            std::string numofwords = std::to_string(j->NumberOfWords);
            std::string widthofwords = std::to_string(j->word_width);
            std::string str = numofwords + '_' + widthofwords;
            if (AfterDivByRowCol.find(str) != AfterDivByRowCol.end())
            {
                auto it = AfterDivByRowCol.find(str);
                it->second.emplace_back(j);
            }
            else
            {
                std::vector<Memory*> tmp {j};
                AfterDivByRowCol.insert(std::pair<std::string,std::vector<Memory*>>(str,std::move(tmp)));
                tmp.clear();
            }
        }
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
                std::cout << k.first << " ";
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
    for (auto i : AfterDivByRowCol)
    {
        std::cout << i.first << ": " << std::endl;
        for (auto j : i.second)
        {
           std::cout << j->mem_Path + '/' + j->mem_Name << std::endl;
        }
        std::cout << std::endl;
    }
    std::cout << "------------------------------------------------------------------------------------------------" << std::endl;
    for (auto &c : clkDomainMap)
    {
        std::cout << c.first << " : " << std::endl;
        for (auto &s : c.second)
        {
            std::cout << "\t" << s << std::endl;
        }
        std::cout << std::endl;
    }
}

void Parser::GetInformationFromFile()
{
    // parser file
    GetAllFileNames();

    PrintMemInfo();

    //grouping
    DivByRowCol();


    // Show Information
    Print();
}
