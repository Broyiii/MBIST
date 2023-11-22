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

bool Parser::GetAllFileNames()
{
    if (!GetFileNameFromFolder(db.work_dir + "ds/", db.ds_files))
        logger.log("WARNING ! No ds directory !");
        // std::cout << "WARNING ! No ds directory !" << std::endl;
        // return false;
    if (!GetFileNameFromFolder(db.work_dir + "summ/", db.summ_files))
        logger.log("WARNING ! No summ directory !");

    if (!GetFileNameFromFolder(db.work_dir + "lib/", db.lib_files))
        logger.log("WARNING ! No lib directory !");

    if (!GetFileNameFromFolder(db.work_dir + "lvlib/", db.lvlib_files))
        logger.log("WARNING ! No lvlib directory !");

    if (!GetFileNameFromFolder(db.work_dir + "verilog/", db.verilog_files))
        logger.log("WARNING ! No verilog directory !");


    if (!ParseMemList())
        return false;

    if (db.inputBlock)
        this->distanceCon = false;
    if (db.dis_max > 0)
        this->distanceCon = true;
    if (!ParseDef())
        this->distanceCon = false;

    if (!ParseCLK())
        this->clkCon = false;
    


    for (int i = 0; i < db.lvlib_files.size(); i++)
    {
        for (auto &j : memorysMappedByName)
        {
            if (db.lvlib_files[i].find(j.first) != std::string::npos)
            {
                ParseLvlib(db.lvlib_files[i]);
            }
        }
    }

    for (int i = 0; i < db.lib_files.size(); i++)
    {
        for (auto &j : memorysMappedByName)
        {
            if (db.lib_files[i].find(j.first) != std::string::npos)
            {
                ParseLib(db.lib_files[i]);
            }
        }
    }

    for (int i = 0; i < db.summ_files.size(); i++)
    {
        for (auto &j : memorysMappedByName)
        {
            if (db.summ_files[i].find(j.first) != std::string::npos)
            {
                ParseSUMM(db.summ_files[i]);
            }
        }
    }

    for (int i = 0; i < db.ds_files.size(); i++)
    {
        for (auto &j : memorysMappedByName)
        {
            if (db.ds_files[i].find(j.first) != std::string::npos)
            {
                ParseDataSheet(db.ds_files[i]);
            }
        }
    }

    ParseSpec();

    this->PrintMems();

    return true;
}

void Parser::GroupMultiAlgoMems(Memory *mem)
{
    Group g(mem, true);

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
        logger.log("[GroupByHardCondition] " + Group::GetInfo(g));

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
            logger.log("[GroupByHardCondition] [Handle multi algo]" + Group::GetInfo(Group(mem)));
            GroupMultiAlgoMems(mem);
        }
        AfterHardCondition.erase(iter);
    }
}

std::vector<GroupedMemList> Parser::ViolentSearch(std::vector<GroupedMemList> &maxNodes)
{
    std::vector<GroupedMemList> res(maxNodes.size());
    std::unordered_map<Memory*, std::vector<int>> DuplicateMems;  // <mem, group id>

    for (int index = 0; index < maxNodes.size(); ++index)
    {
        auto mems = maxNodes[index];
        for (auto mem : mems.memList)
        {
            auto iter = DuplicateMems.find(mem);
            if (iter != DuplicateMems.end())
            {
                iter->second.emplace_back(index);
            }
            else
            {
                DuplicateMems.insert(std::pair<Memory*, std::vector<int>>(mem, std::vector<int>(1, index)));
            }
        }
    }
    
    std::vector<DuplicateMem> RestMems;
    for (auto iter : DuplicateMems)
    {
        if (iter.second.size() == 1)
        {
            res[iter.second[0]].AddMemUnsafe(iter.first);
        }
        else
        {
            RestMems.emplace_back(DuplicateMem{iter.first, iter.second});
        }
    }

    std::sort(RestMems.begin(), RestMems.end(), DuplicateMem::ComparePower);
    std::vector<DuplicateMem> AfterRestMems;
    for (auto iter = RestMems.begin(); iter != RestMems.end(); ++iter)  // from big power mem to small power mem
    {
        bool flagPutIntoVec = false;
        for (auto id : iter->groupIDs)
        {
            std::vector<double> restPower(1, db.power_max);
            for (auto mem : res[id].memList)
            {
                bool flag = true;
                for (auto &p : restPower)
                {
                    if (p >= mem->dynamic_power)
                    {
                        p -= mem->dynamic_power;
                        flag = false;
                        break;
                    }
                }
                if (flag)
                {
                    restPower.emplace_back(db.power_max - mem->dynamic_power);
                }
            }
            if (restPower[restPower.size() - 1] >= iter->mem->dynamic_power)
            {
                res[id].AddMemUnsafe(iter->mem);
                flagPutIntoVec = true;
                break;
            }
        }
        if (!flagPutIntoVec)
        {
            AfterRestMems.emplace_back(*iter);
        }
    }

    if (AfterRestMems.size() > 10)
    {
        population = new Population(AfterRestMems, res);
        auto tmp = population->DoGenetic(20);
        population->~Population();
        return tmp;
        // return RemoveDuplicateMems_for_DFS(AfterRestMems, res);
    }
    int minGroupNum = INT32_MAX;
    std::vector<GroupedMemList> minGroup;
    DFS(0, res, AfterRestMems, minGroupNum, minGroup);
    return minGroup;
}

std::vector<GroupedMemList> Parser::RemoveDuplicateMems_for_DFS(std::vector<DuplicateMem> &DuplicateMems, std::vector<GroupedMemList> res)
{    
    for (auto &i : DuplicateMems)
    {
        auto mem = i.mem;
        double maxPower = -1.0;
        int maxIndex = -1;
        for (int index : i.groupIDs)
        {
            double modPower = std::fmod(res[index].totalPower + mem->dynamic_power, db.power_max);
            if (modPower > maxPower)
            {
                maxPower = modPower;
                maxIndex = index;
            }
        }
        res[maxIndex].AddMemUnsafe(mem);
    }
    return res;
}

void Parser::DFS(int num, std::vector<GroupedMemList> groups, std::vector<DuplicateMem> &RestMems, int &minGroupNum, std::vector<GroupedMemList> &minGroup)
{
    // printf("num = %0d\n", num);
    if (num == RestMems.size())
    {
        auto tmp = GroupOneListByPower(groups);
        int thisGroupSize = tmp.size();
        if (thisGroupSize < minGroupNum)
        {
            minGroupNum = thisGroupSize;
            minGroup = tmp;
        }
        return;
    }

    for (auto id : RestMems[num].groupIDs)
    {
        groups[id].AddMemUnsafe(RestMems[num].mem);
        DFS(num+1, groups, RestMems, minGroupNum, minGroup);
        groups[id].DelteMem(RestMems[num].mem);
    }
}

void Parser::GroupByDistance()
{
    BuildMatric();

    for (auto i : AfterHardCondition)
    {
        auto maxNodes = GetMaxClique(i.second);
        auto iter = AfterGroupBypower.find(i.first);
        if (iter == AfterGroupBypower.end())
        {
            if (db.BKfuntion == 0)
                AfterGroupBypower.insert(std::pair<Group, std::vector<GroupedMemList>>(i.first, ViolentSearch(maxNodes)));
            else if (db.BKfuntion == 1)
            {
                AfterGroupByDis.insert(std::pair<Group, std::vector<GroupedMemList>>(i.first, RemoveDuplicateMems_t(maxNodes)));
                GroupByPower();
            }
            else if (db.BKfuntion == 2)
            {
                // TODO::
            }
        }
        else
            logger.log("[GroupByDistance] ERROR ! This group is existed !");
    }

    // PrintBK();
}

std::vector<GroupedMemList> Parser::GroupOneListByPower(std::vector<GroupedMemList> memsGroup)
{
    std::vector<GroupedMemList> tmp;
    int index = 0;
    for (auto& memsLi : memsGroup)
    {
        auto &mems = memsLi.memList;
        if (!CheckSort(mems))
        {
            std::sort(mems.begin(), mems.end(), GroupedMemList::ComparePower);
            // return false;
        }
        
        if (mems.front()->dynamic_power > db.power_max)
        {
            printf("ERROR ! Memoery [ %s ] power is bigger than max power ! power = %.4f\n\n", mems.front()->mem_Name.c_str(), mems.front()->dynamic_power);
            logger.log("ERROR ! Memoery [ " + mems.front()->mem_Name + " ] power is bigger than max power ! power = " + std::to_string(mems.front()->dynamic_power));
            return {};
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
    return tmp;
}

bool Parser::GroupByPower()
{
    for (auto groups : AfterGroupByDis)
    {
        auto tmp = GroupOneListByPower(groups.second);
        if (tmp.empty())
            return false;
        AfterGroupBypower.insert(std::pair<Group, std::vector<GroupedMemList>>(groups.first, std::move(tmp)));
    }

    return true;
}

void Parser::BuildMatric()
{
    int cnt = 0;
    for (auto &i : memorysMappedByPath)
    {
        i.second->node_id = cnt++;
        memId2memPath.emplace_back(i.second->mem_Path);
    }

    if (this->distanceCon)
    {
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
    else if (db.block_max > 0)
    {
        for (auto i : AfterHardCondition)
        {
            for (auto mem_front = i.second.begin(); mem_front != i.second.end(); ++mem_front)
            {
                for (auto mem_back = std::next(mem_front); mem_back != i.second.end(); ++mem_back)
                {
                    if (db.CalculateBlockCon(*mem_front, *mem_back))
                    {
                        (*mem_front)->connectedMems.push_back((*mem_back)->node_id);
                        (*mem_back)->connectedMems.push_back((*mem_front)->node_id);
                    }
                }
            }
        }
    }
    else
    {
        std::cout << "No dis and block constraint ! " << std::endl;
    }
}

void Parser::OutCsvFile()
{
    this->groupNum = 0;
    int cnt = 0;
    Group p;
    db.outcsvFile = fopen(db.output_csv_name.c_str(),"w");
    fprintf(db.outcsvFile, "UP,Down,contronler,hard\n");
    for (auto &i : AfterGroupBypower)
    {
        if (p.clkDomain != i.first.clkDomain || p.memType != p.memType || p.Algos != i.first.Algos)
        {
            cnt++;
            Group p = i.first;
        }  
        for (auto& mList : i.second)
        {
            ++this->groupNum;
            for (auto k : mList.memList)
            {
                fprintf(db.outcsvFile, "%lld,%lld,contronler_%0d,hard_%0d\n",k->up_bound,k->low_bound,this->groupNum,cnt);
            }
        }
    }
}

bool Parser::GetInformationFromFile()
{
    // parser file
    if (!GetAllFileNames())
    {
        std::cout << "ERROR 1 ! Lack necessary files !" << std::endl;
        logger.log("[GetInformationFromFile] ERROR ! Lack necessary files !");
        return false;
    }

    PrintMemInfo();

    for (auto &i : this->memorysMappedByName)
    {
        if ((*i.second.begin())->dynamic_power > db.power_max)
        {
            printf("ERROR ! Memoery [ %s ] power is bigger than max power ! power = %.4f\n\n", (*i.second.begin())->mem_Name.c_str(), (*i.second.begin())->dynamic_power);
            logger.log("ERROR ! Memoery [ " + (*i.second.begin())->mem_Name + " ] power is bigger than max power ! power = " + std::to_string((*i.second.begin())->dynamic_power));
            return false;
        }
        if (this->distanceCon)
        {
            for (auto &mem : i.second)
            {
                mem->AdjustPositionByDirect(db.distance_unit);
            }
        }
    }

    // grouping
    GroupByHardCondition();
    
    GroupByDistance();

    if(!SatisfyDisCon(AfterGroupBypower))
    {
        std::cout << "ERROR ! Do not satisfy distance constraint !" << std::endl;
        return false;
    }

    if (!SatisfyPowerCon(AfterGroupBypower))
    {
        std::cout << "ERROR ! Do not satisfy power constraint !" << std::endl;
        return false;
    }

    WriteAnswer();
    OutCsvFile();
    return true;
}

