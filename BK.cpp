#include "parser.hpp"
#include "Logger.hpp"

extern Logger& logger;

bool Parser::SatisfyDisCon(std::unordered_map<Group, std::vector<GroupedMemList>, Group::Hash> &groups)
{
    logger.log("[SatisfyDisCon] Check Distance Constraints ...");
    if (!CheckLackNodes(groups))
    {
        logger.log("[SatisfyDisCon] FAIL ! Lack nodes !");
        return false;
    }
    for (auto i : groups)
    {
        for (auto j : i.second)
        {
            // mems is a group
            // check hard condition
            Group g(j.memList.front());
            for (auto mem : j.memList)
            {
                Group gm(mem);
                if (g != gm)
                {
                    logger.log("[SatisfyDisCon] FAIL ! Not Satisfy Hard Condition !");
                    return false;
                }
            }
            // check dis cons
            for (auto mem_front = j.memList.begin(); mem_front != j.memList.end(); ++mem_front)
            {
                for (auto mem_back = std::next(mem_front); mem_back != j.memList.end(); ++mem_back)
                {
                    if (this->distanceCon)
                    {   
                        if (!db.CalculateDis(*mem_front, *mem_back))
                        {
                            logger.log("[SatisfyDisCon] FAIL ! Not Satisfy Distance Condition !");
                            return false;
                        }
                    }
                    else
                    {
                        if (!db.CalculateBlockCon(*mem_front, *mem_back))
                        {
                            logger.log("[SatisfyDisCon] FAIL ! Not Satisfy Distance Condition !");
                            return false;
                        }
                    }
                }
            }
        }
    }
    logger.log("[SatisfyDisCon] PASS !");
    return true;
}

bool Parser::CheckLackNodes(std::unordered_map<Group, std::vector<GroupedMemList>, Group::Hash> &groups)
{
    for (auto &i : groups)
    {
        for (auto &j : i.second)
        {
            for (auto &k : j.memList)
            {
                check[k->node_id] = 1;
            }
        }
    }

    for (int k = 0; k < this->memId2memPath.size(); k++)
    {
        if (check[k] == 0)
        {
            std::cout << k << " : " << memId2memPath[k] << " : " << memorysMappedByPath[memId2memPath[k]]->connectedMems.size() << " : ";
            for (auto id : memorysMappedByPath[memId2memPath[k]]->connectedMems)
            {
                std::cout << id << ", ";
            }
            std::cout << std::endl;
            logger.log("[CheckLackNodes] ERROR ! " + memId2memPath[k] + " : " + std::to_string(memorysMappedByPath[memId2memPath[k]]->connectedMems.size()));
            // return false;
        }
    }
    return true;
}

void Parser::BronKerbosh(std::deque<int> R, std::deque<int> P, std::deque<int> S)
{
    if (P.empty() && S.empty())
    {
        std::deque<Memory*> mems;
        for (auto index : R)
        {
           mems.push_back(memorysMappedByPath[memId2memPath[index]]);
        }
        maxNodes.emplace_back(GroupedMemList(db.power_max, mems));
    }

    while (!P.empty())
    {
        int nodeID = *(P.begin());
        // int maxsize = -1;
        // for (auto it = P.begin(); it != P.end(); ++it)
        // {
        //     int i = *it;
        //     auto iter = memorysMappedByPath.find(memId2memPath[i]);
        //     if (iter != memorysMappedByPath.end())
        //     {
        //         int tmpsize = iter->second->connectedMems.size();
        //         bool flag = (tmpsize > maxsize);
        //         if (flag)
        //         {
        //             nodeID = i;
        //             maxsize = iter->second->connectedMems.size();
        //         }
        //     }
        //     else
        //     {
        //         logger.log("[BronKerbosh] ERROR ! No such mem in memorysMappedByPath");
        //     }
        // }
        std::deque<int> R_new = R;
        // if (nodeID == -1)
        // {
        //     logger.log("[BronKerbosh] ERROR ! nodeID = -1 !");
            // nodeID = *(P.begin());
        // }
        R_new.push_back(nodeID);
        auto P_new = memorysMappedByPath[memId2memPath[nodeID]]->FindBothNeigbor(P);
        BronKerbosh(R_new, memorysMappedByPath[memId2memPath[nodeID]]->FindBothNeigbor(P), memorysMappedByPath[memId2memPath[nodeID]]->FindBothNeigbor(S));
        S.push_back(nodeID);
        auto it = std::find(P.begin(), P.end(), nodeID);
        if (it != P.end())
            P.erase(it);
        else
            logger.log("ERROR ! No such node " + std::to_string(nodeID) + " in deque P !");
        if (!P.empty())
        {
            P = memorysMappedByPath[memId2memPath[nodeID]]->FindDifference(P);
        }
    }
}

void Parser::GetMaxClique(std::deque<Memory*> mems)
{
    std::deque<int> R = {};
    std::deque<int> P = {};
    std::deque<int> S = {};
    CheckSort(mems);
    for (auto m : mems)
    {
        P.push_back(m->node_id);
    }

    BronKerbosh(R, P, S);
}

std::vector<GroupedMemList> Parser::RemoveDuplicateMems()
{
    std::vector<GroupedMemList> res;
    // int maxSize = 0;
    while (1)
    {
        GroupedMemList maxList;
        // find the max Clique
        for (auto &i : this->maxNodes)
        {
            // i.Print();
            if (i.memList.size() > maxList.memList.size())
            {
                maxList = i;
            }
        }
        if (maxList.memList.size() == 0)
        {
            break;
        }
        // std::cout << "Max:" << std::endl;
        // maxList.Print();
        res.emplace_back(maxList);

        for (auto &i : this->maxNodes)
        {
            if (i.memList.size() != 0)
            {
                i = i.RemoveDuplicate(maxList);
                // i.Print();
            }
        }
    }
    return res;
}

std::vector<GroupedMemList> Parser::RemoveDuplicateMems_t()
{
    std::vector<GroupedMemList> res;
    // std::vector<DuplicateMem> DuplicateMems;
    std::unordered_map<Memory*, std::vector<int>> DuplicateMems;  // <mem, group id>
    GroupedMemList maxList;
    // find the max Clique
    for (int index = 0; index < this->maxNodes.size(); ++index)
    {
        auto &mems = this->maxNodes[index];
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
    
    for (auto &i : DuplicateMems)
    {
        auto mem = i.first;
        double maxPower = -1.0;
        int maxID = -1;
        if (i.second.size() == 1)
            continue;
        for (int index : i.second)
        {
            double modPower = std::fmod(this->maxNodes[index].totalPower, db.power_max);
            if (modPower > maxPower)
            {
                maxPower = modPower;
                if (maxID > -1)
                {
                    if (!this->maxNodes[index].DelteMem(mem))
                        logger.log("[RemoveDuplicateMems_t] ERROR 1 ! No such mem " + mem->mem_Path);
                }
                maxID = index;
            }
            else
            {
                if (!this->maxNodes[index].DelteMem(mem))
                    logger.log("[RemoveDuplicateMems_t] ERROR 2 ! No such mem " + mem->mem_Path);
            }
        }
    }
    return this->maxNodes;
}

void Parser::PrintBK()
{
   // std::cout << "Max Clique : " << this->maxNodes[0].size() << std::endl;
    for (auto &i : AfterGroupByDis)
    {
        
        std::sort(i.second.begin(), i.second.end(), GroupedMemList::Compare);
        for (auto j : i.second)
        {
            j.Print();
        }
        std::cout << "-------------" << std::endl;
    }
}


