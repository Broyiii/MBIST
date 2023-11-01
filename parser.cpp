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

void Parser::GroupByClk()
{
    for (auto i : memorysMappedByName)
    {
        for (auto j : i.second)
        {
            if (AfterGroupByClk.find(j->clk_domain) != AfterGroupByClk.end())
            {
                auto it = AfterGroupByClk.find(j->clk_domain);
                it->second.emplace_back(j);
            }
            else
            {
                std::vector<Memory*> tmp {j};
                AfterGroupByClk.insert(std::pair<std::string,std::vector<Memory*>>(j->clk_domain,std::move(tmp)));
                tmp.clear();
            }
        }
    }
}

void Parser::GroupByType()
{
    for (auto i : AfterGroupByClk)
    {
        for (auto j : i.second)
        {
            std::string clk_type = i.first + '_' + std::to_string(j->mem_type);
            if (AfterGroupByType.find(clk_type) != AfterGroupByType.end())
            {
                auto it = AfterGroupByType.find(clk_type);
                it->second.emplace_back(j);
            }
            else
            {
                std::vector<Memory*> tmp {j};
                AfterGroupByType.insert(std::pair<std::string,std::vector<Memory*>>(clk_type,std::move(tmp)));
                tmp.clear();
            }
        }
    }
}

void Parser::GroupByAlgorithm()
{
    for (auto i : AfterGroupByType)
    {
        for (auto j : i.second)
        {
            if (j->Algorithms.size() > 1)
            {
                HaveMultiAlgorithms = true;
                std::string clk_type_multi = i.first + "_multi";
                if (AfterGroupByAlgorithm.find(clk_type_multi) != AfterGroupByAlgorithm.end())
                {
                    auto it = AfterGroupByAlgorithm.find(clk_type_multi);
                    it->second.emplace_back(j);
                }
                else
                {
                    std::vector<Memory*> tmp {j};
                    AfterGroupByAlgorithm.insert(std::pair<std::string,std::vector<Memory*>>(clk_type_multi,std::move(tmp)));
                    tmp.clear();
                }
            }
            else
            {
                std::string clk_type_multi = i.first + '_' + j->Algorithms[0];;
                if (AfterGroupByAlgorithm.find(clk_type_multi) != AfterGroupByAlgorithm.end())
                {
                    auto it = AfterGroupByAlgorithm.find(clk_type_multi);
                    it->second.emplace_back(j);
                }
                else
                {
                    std::vector<Memory*> tmp {j};
                    AfterGroupByAlgorithm.insert(std::pair<std::string,std::vector<Memory*>>(clk_type_multi,std::move(tmp)));
                    tmp.clear();
                }
            }

        }
    }
}

void Parser::GroupByHardCondition()
{
    GroupByClk();
    GroupByType();
    GroupByAlgorithm();
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
    GetAllFileNames();
    PrintMemInfo();

    // grouping
    GroupByHardCondition();

    // Show Information
    Print();
}

