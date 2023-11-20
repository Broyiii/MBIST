#ifndef _GENETIC_HPP_
#define _GENETIC_HPP_

#include "global.hpp"

extern dataBase db;

class Individual 
{
public:
    std::vector<int> chromosome;
    std::vector<GroupedMemList> fitness;
    std::vector<int> chromosomeRange;

    double crossoverRate = 0.7;  // 交叉
    double mutationRate = 0.1;  // 变异

    Individual() {}
    Individual(std::vector<int> c, std::vector<GroupedMemList> f, std::vector<int> r) : chromosome(c), fitness(f), chromosomeRange(r) {}
    
    double getRandomDouble() {
        return rand() / (RAND_MAX + 1.0);
    }
    // 变异
    void mutate(double rate = 0.1)
    {
        for (int index = 0; index < chromosome.size(); ++index)
        {
            if (getRandomDouble() < rate)
            {
                int tmp = (rand() % (chromosomeRange[index] - 1)) + 1;
                if (chromosome[index] + tmp >= chromosomeRange[index])
                {
                    //  chromosome[index] = tmp - (chromosomeRange[index] - 1 - chromosome[index]) - 1;
                    chromosome[index] = tmp - chromosomeRange[index] + chromosome[index];
                }
                else
                {
                    chromosome[index] += tmp;
                }
            }
        }
    }

    // 交叉选择
    void crossover(Individual& otherParent)
    {
        if (getRandomDouble() < crossoverRate)
        {
            int num = (rand() % (chromosome.size() - 1)) + 1;  // [1, chromosome.size() - 1]
            std::set<int> crossIndexs;
            while(crossIndexs.size() < num)
            {
                crossIndexs.insert(rand() % chromosome.size());
            }
            for (auto index : crossIndexs)
            {
                std::swap(this->chromosome[index], otherParent.chromosome[index]);
            }
        }
    }
};

class Population
{
public:
    std::vector<Individual> individuals;
    std::vector<DuplicateMem> RestMems;
    std::vector<GroupedMemList> GroupedMems;
    std::vector<int> chromosomeRange;

    int chromosomeLength;
    const int populationSize = 10;

    // 初始化
    Population (std::vector<DuplicateMem> R, std::vector<GroupedMemList> G) : RestMems(R), GroupedMems(G)
    {
        for (auto &r : RestMems)
            chromosomeRange.emplace_back(r.groupIDs.size());

        Individual initialIndivdual = GenerateIndividual();
        individuals.resize(populationSize, initialIndivdual);
        for (int index = 1; index < populationSize; ++index)
            individuals[index].mutate(0.3);
    }

    ~Population()
    {
        individuals.clear();
        individuals.shrink_to_fit();
        RestMems.clear();
        RestMems.shrink_to_fit();
        GroupedMems.clear();
        GroupedMems.shrink_to_fit();
        chromosomeRange.clear();
        chromosomeRange.shrink_to_fit();
    }

    Individual GenerateIndividual();
    std::vector<GroupedMemList> calculateFitness(std::vector<int> chro);
    Individual tournamentSelection();  // 选择操作（锦标赛选择）
    Individual getFittest();  // 获取当前种群中适应度最高的个体
    void evolve();  // 进化操作

    std::vector<GroupedMemList> DoGenetic(int generation)
    {
        for (int i = 0; i < generation; ++i)
        {
            // std::cout << i << std::endl;
            this->evolve();
        }
        auto indv = this->getFittest();
        return indv.fitness;
    }
};

#endif