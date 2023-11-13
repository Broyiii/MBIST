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
                    if (!db.CalculateDis(*mem_front, *mem_back))
                    {
                        logger.log("[SatisfyDisCon] FAIL ! Not Satisfy Distance Condition !");
                        return false;
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
            std::cout << k << " : " << memId2memPath[k] << std::endl;
            return false;
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
        int nodeID = *P.begin();
        std::deque<int> R_new = R;
        R_new.push_back(nodeID);
        BronKerbosh(R_new, memorysMappedByPath[memId2memPath[nodeID]]->FindBothNeigbor(P), memorysMappedByPath[memId2memPath[nodeID]]->FindBothNeigbor(S));
        S.push_back(nodeID);
        P.pop_front();
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


