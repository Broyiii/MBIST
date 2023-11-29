#include "parser.hpp"
#include "global.hpp"

extern dataBase db;
std::atomic<int> runOver(0);
std::mutex mtx;

bool Parser::CheckSort(std::deque<Memory *> mems)
{
    if (db.logFlag) logger.log("[CheckSort] Check Sort ...");
    int tmp = -1;
    for (auto m : mems)
    {
        if (tmp >= m->node_id)
        {
            if (db.logFlag) logger.log("[CheckSort] FAIL !");
            return false;
        }
        tmp = m->node_id;
    }
    if (db.logFlag) logger.log("[CheckSort] PASS !");
    return true;
}

bool Parser::SatisfyPowerCon(std::unordered_map<Group, std::vector<GroupedMemList>, Group::Hash> &groups)
{
    if (db.logFlag) logger.log("[SatisfyPowerCon] Check Power ...");
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
                if (db.logFlag) logger.log("[SatisfyPowerCon] FAIL !");
                return false;
            }
        }
    }
    if (db.logFlag) logger.log("[SatisfyPowerCon] PASS !");
    return true;
}

bool Parser::GetAllFileNames()
{
    if (!GetFileNameFromFolder(db.work_dir + "ds/", db.ds_files))
        if (db.logFlag) logger.log("WARNING ! No ds directory !");
        // std::cout << "WARNING ! No ds directory !" << std::endl;
        // return false;
    if (!GetFileNameFromFolder(db.work_dir + "summ/", db.summ_files))
        if (db.logFlag) logger.log("WARNING ! No summ directory !");

    if (!GetFileNameFromFolder(db.work_dir + "lib/", db.lib_files))
        if (db.logFlag) logger.log("WARNING ! No lib directory !");

    if (!GetFileNameFromFolder(db.work_dir + "lvlib/", db.lvlib_files))
        if (db.logFlag) logger.log("WARNING ! No lvlib directory !");

    if (!GetFileNameFromFolder(db.work_dir + "verilog/", db.verilog_files))
        if (db.logFlag) logger.log("WARNING ! No verilog directory !");


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

    if (db.logFlag) this->PrintMems();

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
            if (db.logFlag) logger.log("[GroupMultiAlgoMems] ERROR 5 ! Algo is " + algo);
        }
    }

    if (avePower < 0)
    {
        if (db.logFlag) logger.log("[GroupMultiAlgoMems] ERROR 6 ! Algo is " + mem->Algorithms[0]);
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
        if (db.logFlag) logger.log("[GroupByHardCondition] " + Group::GetInfo(g));

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
            if (db.logFlag) logger.log("[GroupByHardCondition] [Handle multi algo]" + Group::GetInfo(Group(mem)));
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

    // std::sort(RestMems.begin(), RestMems.end(), DuplicateMem::ComparePower);
    // std::vector<DuplicateMem> AfterRestMems;
    // for (auto iter = RestMems.begin(); iter != RestMems.end(); ++iter)  // from big power mem to small power mem
    // {
    //     bool flagPutIntoVec = false;
    //     for (auto id : iter->groupIDs)
    //     {
    //         std::vector<double> restPower(1, db.power_max);
    //         for (auto mem : res[id].memList)
    //         {
    //             bool flag = true;
    //             for (auto &p : restPower)
    //             {
    //                 if (p >= mem->dynamic_power)
    //                 {
    //                     p -= mem->dynamic_power;
    //                     flag = false;
    //                     break;
    //                 }
    //             }
    //             if (flag)
    //             {
    //                 restPower.emplace_back(db.power_max - mem->dynamic_power);
    //             }
    //         }
    //         if (restPower[restPower.size() - 1] >= iter->mem->dynamic_power)
    //         {
    //             res[id].AddMemUnsafe(iter->mem);
    //             flagPutIntoVec = true;
    //             break;
    //         }
    //     }
    //     if (!flagPutIntoVec)
    //     {
    //         AfterRestMems.emplace_back(*iter);
    //     }
    // }
    // auto AfterRestMems = RestMems;

    // if (AfterRestMems.size() > 10)
    if (RestMems.size() > 10)
    {
        if (db.logFlag) logger.log("Genetic, size = " + std::to_string(RestMems.size()));
        Population *population;
        population = new Population(RestMems, res);
        int geneticTime = RestMems.size() * 50;
        if (db.logFlag) logger.log("geneticTime = " + std::to_string(geneticTime));
        auto tmp = population->DoGenetic(geneticTime);
        population->~Population();
        return tmp;
        // return RemoveDuplicateMems_for_DFS(RestMems, res);
    }
    if (db.logFlag) logger.log("DFS, size = " + std::to_string(RestMems.size()));
    int minGroupNum = INT32_MAX;
    std::vector<GroupedMemList> minGroup;
    DFS(0, res, RestMems, minGroupNum, minGroup);
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

Memory* Parser::GetMaxConnectNumMem(std::deque<int> memsID)
{
    Memory* res = memorysMappedByPath[memId2memPath[memsID.front()]];
    size_t num = res->connectedMems.size();
    for (auto memID : memsID)
    {
        auto mem = memorysMappedByPath[memId2memPath[memID]];
        if (mem->connectedMems.size() > num)
        {
            num = mem->connectedMems.size();
            res = mem;
        }
    }
    return res;
}

std::vector<GroupedMemList> Parser::VeryFastGetMaxClique(std::deque<Memory*> mems)
{
    std::vector<GroupedMemList> res;
    std::deque<int> allMems;
    for (auto mem : mems)
    {
        // printf("%0d, ", mem->node_id);
        allMems.push_back(mem->node_id);
    }
    // printf("\n");
    while (!allMems.empty())
    {
        auto maxMem = GetMaxConnectNumMem(allMems);
        std::deque<int> usedMems;
        GroupedMemList g(db.power_max);
        g.AddMemUnsafe(maxMem);
        for (auto m : maxMem->connectedMems)
        {
            g.AddMemUnsafe(memorysMappedByPath[memId2memPath[m]]);
            usedMems.push_back(m);
        }
        res.emplace_back(std::move(g));
        auto iter = usedMems.begin();
        for (; iter != usedMems.end(); ++iter)
        {
            if ((*iter) > maxMem->node_id)
                break;
        }
        usedMems.insert(iter, maxMem->node_id);
        std::deque<int> tmp;
        std::set_difference(allMems.begin(),allMems.end(),
                            usedMems.begin(),usedMems.end(),std::back_inserter(tmp));
        allMems = tmp;
        for (auto id : allMems)
        {
            auto mem = memorysMappedByPath[memId2memPath[id]];
            mem->connectedMems = mem->FindDifferenceForConnect(usedMems);
        }
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

void Parser::GroupThread(Parser* thisParser, std::pair<const Group, std::deque<Memory *>> &groupHard)
{
    std::vector<GroupedMemList> maxNodes;
    if ((db.BKfuntion != 2) || (!thisParser->distanceCon))
        maxNodes = thisParser->GetMaxClique(groupHard.second);
    else
        maxNodes = thisParser->VeryFastGetMaxClique(groupHard.second);
    auto iter = thisParser->AfterGroupBypower.find(groupHard.first);
    if (iter == thisParser->AfterGroupBypower.end())
    {
        if (db.BKfuntion == 0)
            thisParser->AfterGroupBypower.insert(std::pair<Group, std::vector<GroupedMemList>>(groupHard.first, thisParser->ViolentSearch(maxNodes)));
        else if (db.BKfuntion == 1)
        {
            thisParser->AfterGroupByDis.insert(std::pair<Group, std::vector<GroupedMemList>>(groupHard.first, thisParser->RemoveDuplicateMems_t(maxNodes)));
            thisParser->GroupByPower();
        }
        else if (db.BKfuntion == 2)
        {
            thisParser->AfterGroupByDis.insert(std::pair<Group, std::vector<GroupedMemList>>(groupHard.first, maxNodes));
            thisParser->GroupByPower();
        }
    }
    else
        if (db.logFlag) logger.log("[GroupByDistance] ERROR ! This group is existed !");
    ++runOver;
}

void Parser::GroupUtil(std::pair<const Group, std::deque<Memory *>> &groupHard)
{
    std::vector<GroupedMemList> maxNodes;
    if ((db.BKfuntion != 2) || (!this->distanceCon))
        maxNodes = this->GetMaxClique(groupHard.second);
    else
        maxNodes = this->VeryFastGetMaxClique(groupHard.second);
    auto iter = this->AfterGroupBypower.find(groupHard.first);
    if (iter == this->AfterGroupBypower.end())
    {
        if (db.BKfuntion == 0)
            this->AfterGroupBypower.insert(std::pair<Group, std::vector<GroupedMemList>>(groupHard.first, this->ViolentSearch(maxNodes)));
        else if (db.BKfuntion == 1)
        {
            this->AfterGroupByDis.insert(std::pair<Group, std::vector<GroupedMemList>>(groupHard.first, this->RemoveDuplicateMems_t(maxNodes)));
            this->GroupByPower();
        }
        else if (db.BKfuntion == 2)
        {
            this->AfterGroupByDis.insert(std::pair<Group, std::vector<GroupedMemList>>(groupHard.first, maxNodes));
            this->GroupByPower();
        }
    }
    else
        if (db.logFlag) logger.log("[GroupByDistance] ERROR ! This group is existed !");
}

void Parser::GroupByDistance()
{
    BuildMatric();

    if (db.threadNum == 0)
    {
        for (auto &i : AfterHardCondition)
        {
            GroupUtil(i);
        }
    }
    else
    {
        for (auto &i : AfterHardCondition)
        {
            threadpool->enqueue(GroupThread, this, i);
        }
        while(runOver != AfterHardCondition.size());
    }
}

std::vector<GroupedMemList> Parser::GroupOneListByPower(std::vector<GroupedMemList> memsGroup)
{
    std::vector<GroupedMemList> tmp;
    int index = 0;
    for (auto& memsLi : memsGroup)
    {
        auto &mems = memsLi.memList;
        std::sort(mems.begin(), mems.end(), Memory::compareByPower);
        
        if (mems.front()->dynamic_power > db.power_max)
        {
            printf("ERROR ! Memoery [ %s ] power is bigger than max power ! power = %.4f\n\n", mems.front()->mem_Name.c_str(), mems.front()->dynamic_power);
            if (db.logFlag) logger.log("ERROR ! Memoery [ " + mems.front()->mem_Name + " ] power is bigger than max power ! power = " + std::to_string(mems.front()->dynamic_power));
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
        mtx.lock();
        AfterGroupBypower.insert(std::pair<Group, std::vector<GroupedMemList>>(groups.first, std::move(tmp)));
        mtx.unlock();
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
                    if (db.BKfuntion != 2)  // slow and fast mode
                    {
                        if (db.CalculateDis(*mem_front, *mem_back))
                        {
                            (*mem_front)->connectedMems.push_back((*mem_back)->node_id);
                            (*mem_back)->connectedMems.push_back((*mem_front)->node_id);
                        }
                    }
                    else  // very fast mode
                    {
                        if (db.CalculateDisFunc2(*mem_front, *mem_back))
                        {
                            (*mem_front)->connectedMems.push_back((*mem_back)->node_id);
                            (*mem_back)->connectedMems.push_back((*mem_front)->node_id);
                        }
                    }
                }
            }
        }
    }
    else if (db.block_max >= 0)
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
    std::string dir = "./plt/";
    if (access(dir.c_str(), 0) != F_OK)
    {
        mkdir(dir.c_str(), S_IRWXU);
    }
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
        if (db.logFlag) logger.log("[GetInformationFromFile] ERROR ! Lack necessary files !");
        return false;
    }

    PrintMemInfo();

    for (auto &i : this->memorysMappedByName)
    {
        if ((*i.second.begin())->dynamic_power > db.power_max)
        {
            printf("ERROR ! Memoery [ %s ] power is bigger than max power ! power = %.4f\n\n", (*i.second.begin())->mem_Name.c_str(), (*i.second.begin())->dynamic_power);
            if (db.logFlag) logger.log("ERROR ! Memoery [ " + (*i.second.begin())->mem_Name + " ] power is bigger than max power ! power = " + std::to_string((*i.second.begin())->dynamic_power));
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

    if (db.checkFlag)
    {
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
    }

    WriteAnswer();

    if (db.pltFlag)
        OutCsvFile();
    
    return true;
}

