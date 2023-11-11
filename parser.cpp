#include "parser.hpp"
#include "global.hpp"

extern dataBase db;

bool Parser::CheckSort(std::deque<Memory *> mems)
{
    logger.log("[CheckSort] Check Sort ...");
    int tmp = -1;
    for (auto m : mems)
    {
        if (tmp >= m->node_id)
        {
            logger.log("[CheckSort] FAIL !");
            return false;
        }
        tmp = m->node_id;
    }
    logger.log("[CheckSort] PASS !");
    return true;
}

bool Parser::SatisfyPowerCon(std::unordered_map<Group, std::vector<GroupedMemList>, Group::Hash> &groups)
{
    logger.log("[SatisfyPowerCon] Check Power ...");
    for (auto &g : groups)
    {
        for (auto &mems : g.second)
        {
            double power = 0.0;
            for (auto &mem : mems.memList)
            {
                power += mem->dynamic_power;
            }
            if (power > db.power_max)
            {
                logger.log("[SatisfyPowerCon] FAIL !");
                return false;
            }
        }
    }
    logger.log("[SatisfyPowerCon] PASS !");
    return true;
}

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

void Parser::GroupMultiAlgoMems(Memory *mem)
{
    Group g;
    g.clkDomain = mem->clk_domain;
    g.memType = mem->mem_type;

    double avePower = -1.0;
    std::string suitableAlgo;
    for (auto &algo : mem->Algorithms)
    {
        g.Algos.emplace_back(algo);
        auto iter = AfterHardCondition.find(g);
        if (iter != AfterHardCondition.end())
        {
            double power = 0.0;
            for (auto &i : iter->second)
            {
                power += i->dynamic_power;
            }
            power += mem->dynamic_power;
            power = std::fmod(power, db.power_max);
            if (power > avePower)
            {
                avePower = power;
                suitableAlgo = algo;
            }
        }
        else
        {
            logger.log("[GroupMultiAlgoMems] ERROR 5 ! Algo is " + algo);
        }
    }

    if (avePower < 0)
    {
        logger.log("[GroupMultiAlgoMems] ERROR 6 ! Algo is " + mem->Algorithms[0]);
        g.Algos.resize(1, mem->Algorithms[0]);
        mem->Algorithms.resize(1, mem->Algorithms[0]);
        std::deque<Memory *> tmp {mem};
        AfterHardCondition.insert(std::pair<Group, std::deque<Memory *>>(g, std::move(tmp)));
        return;
    }

    g.Algos.resize(1, suitableAlgo);
    mem->Algorithms.resize(1, suitableAlgo);
    auto iter = AfterHardCondition.find(g);
    iter->second.push_back(mem);
}

void Parser::GroupByHardCondition()
{
    for (auto &memorys : memorysMappedByPath)
    {
        auto &mem = memorys.second;

        Group g(mem);

        auto iter_group = AfterHardCondition.find(g);
        if (iter_group != AfterHardCondition.end())
        {
            iter_group->second.push_back(mem);
        }
        else
        {
            std::deque<Memory*> tmpDeque({mem});
            AfterHardCondition.insert(std::pair<Group, std::deque<Memory*>>(std::move(g), std::move(tmpDeque)));
        }
    }

    // Handle multi algo
    Group g_multi;
    g_multi.Algos.resize(2, "Multi");
    auto iter = AfterHardCondition.find(g_multi);
    if (iter != AfterHardCondition.end())
    {
        for (auto mem : iter->second)
        {
            GroupMultiAlgoMems(mem);
        }
        AfterHardCondition.erase(iter);
    }
}

void Parser::GroupByDistance()
{
    BuildMatric();
    for (auto i : AfterHardCondition)
    {
        GetMaxClique(i.second);
        AfterGroupByDis.insert(std::pair<Group, std::vector<GroupedMemList>>(Group(this->maxNodes[0].memList[0]), RemoveDuplicateMems()));
        this->maxNodes.clear();
    }
    // PrintBK();
}

bool Parser::GroupByPower()
{
    for (auto groups : AfterGroupByDis)
    {
        std::vector<GroupedMemList> tmp;
        int index = 0;
        for (auto& memsLi : groups.second)
        {
            auto &mems = memsLi.memList;
            if (!CheckSort(mems))
            {
                return false;
            }
            std::sort(mems.begin(), mems.end(), GroupedMemList::ComparePower);
            if (mems.front()->dynamic_power > db.power_max)
            {
                printf("ERR CODE 4, Single mem's power is bigger than max power !\n");
                return false;
            }
            tmp.emplace_back(GroupedMemList(db.power_max, mems.front()));
            mems.pop_front();
            
            NEW_LOOP_FOR_MEMS:
            while (mems.size())
            {
                for (int i = index; i < tmp.size(); ++i)
                {
                    if (tmp[i].AddMem(mems.front()))
                    {
                        mems.pop_front();
                        goto NEW_LOOP_FOR_MEMS;
                    }
                }
                tmp.emplace_back(GroupedMemList(db.power_max, mems.front()));
                mems.pop_front();
            }
            index = tmp.size();
        }
        AfterGroupBypower.insert(std::pair<Group, std::vector<GroupedMemList>>(groups.first, std::move(tmp)));
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


void Parser::BuildMatric()
{
    int cnt = 0;
    for (auto &i : memorysMappedByPath)
    {
        i.second->node_id = cnt;
        memId2memPath.insert(std::pair<int,std::string>(i.second->node_id,i.second->mem_Path));
        cnt++;
    }

    for (auto i : AfterHardCondition)
    {
        for (auto mem_front = i.second.begin(); mem_front != i.second.end(); ++mem_front)
        {
            for (auto mem_back = std::next(mem_front); mem_back != i.second.end(); ++mem_back)
            {
                if (db.CalculateDis(*mem_front, *mem_back))
                {
                    (*mem_front)->connectedMems.push_back((*mem_back)->node_id);
                    (*mem_back)->connectedMems.push_back((*mem_front)->node_id);
                }
            }
        }
    }
}

bool Parser::GetInformationFromFile()
{
    // parser file
    GetAllFileNames();

    PrintMemInfo();

    // grouping
    GroupByHardCondition();
    
    GroupByDistance();

    if(!GroupByPower())
        return false;

    if(!SatisfyDisCon(AfterGroupBypower))
        return false;
    if (!SatisfyPowerCon(AfterGroupBypower))
        return false;

    WriteAnswer();
    return true;
}

