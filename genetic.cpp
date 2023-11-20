#include "global.hpp"
#include "genetic.hpp"


Individual Population::GenerateIndividual()
{
    std::vector<int> chro(this->chromosomeRange.size());
    std::vector<GroupedMemList> tmp = this->GroupedMems;
    int index_chro = 0;
    for (auto &i : RestMems)
    {
        auto mem = i.mem;
        double maxPower = -1.0;
        int maxIndex = -1;
        int maxChro_id = -1;
        // for (int index : i.groupIDs)
        for (int chro_id = 0; chro_id < i.groupIDs.size(); ++ chro_id)
        {
            int index = i.groupIDs[chro_id];
            double modPower = std::fmod(tmp[index].totalPower + mem->dynamic_power, db.power_max);
            if (modPower > maxPower)
            {
                maxPower = modPower;
                maxIndex = index;
                maxChro_id = chro_id;
            }
        }
        tmp[maxIndex].AddMemUnsafe(mem);
        chro[index_chro++] = maxChro_id;
    }

    auto group = calculateFitness(chro);

    return Individual(chro, group, this->chromosomeRange);
}


std::vector<GroupedMemList> Population::calculateFitness(std::vector<int> chro)
{
    std::vector<GroupedMemList> tmp;
    int index = 0;
    auto memsGroup = this->GroupedMems;
    for (int i = 0; i < chro.size(); ++i)
    {
        int gid = this->RestMems[i].groupIDs[chro[i]];
        // int gid = chro[i];
        memsGroup[gid].AddMemUnsafe(this->RestMems[i].mem);
    }
    for (auto& memsLi : memsGroup)
    {
        auto &mems = memsLi.memList;
        std::sort(mems.begin(), mems.end(), GroupedMemList::ComparePower);
        
        if (mems.front()->dynamic_power > db.power_max)
        {
            printf("ERR CODE 4, Single mem's power is bigger than max power !\n");
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
    return std::move(tmp);
}


Individual Population::tournamentSelection() 
{
    const int tournamentSize = 2;
    int randomIndex1 = rand() % populationSize;
    int randomIndex2 = rand() % populationSize;

    if (individuals[randomIndex1].fitness.size() < individuals[randomIndex2].fitness.size()) {
        return individuals[randomIndex1];
    } else {
        return individuals[randomIndex2];
    }
}


Individual Population::getFittest() 
{
    Individual fittest = individuals[0];
    for (const Individual& individual : individuals) 
    {
        if (individual.fitness.size() < fittest.fitness.size()) 
        {
            fittest = individual;
        }
    }
    return fittest;
}


void Population::evolve() 
{
    std::vector<Individual> newGeneration;
    newGeneration.resize(this->populationSize);

    newGeneration[0] = this->getFittest();
    for (int i = 1; i < populationSize; ++i) {
        Individual parent1 = tournamentSelection();
        Individual parent2 = tournamentSelection();

        newGeneration[i] = parent1;
        newGeneration[i].crossover(parent2);
        newGeneration[i].mutate();
        newGeneration[i].fitness = calculateFitness(newGeneration[i].chromosome);
    }

    individuals = newGeneration;
}