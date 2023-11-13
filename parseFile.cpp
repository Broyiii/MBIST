#ifndef _PARSEFILE_CPP_
#define _PARSEFILE_CPP_

#include "global.hpp"
#include "parser.hpp"
#include "Logger.hpp"

extern dataBase db;
extern Logger& logger;

std::vector<std::string> SplitBySpace(std::string str)
{
    std::string buf;
    std::stringstream ss(str);

    std::vector<std::string> tokens;

    while (ss >> buf)
        tokens.emplace_back(buf);

    return tokens;
}

std::string Parser::GetInfoFromStr(std::string &line)
{
    std::string res;
    size_t pos = line.find(":");
    while (++pos < line.size())
    {
        if (line[pos] == ';')
            break;
        else if (line[pos] == ' ')
            continue;
        else
            res += line[pos];
    }
    return res;
}

bool Parser::ParseMemList()
{
    std::ifstream input(db.memorylist_file);
    if (!input.is_open())
    {
        std::cout << "ERROR 0! No such file " << db.memorylist_file << std::endl;
        logger.log("[ParseMemList] ERROR 0! No such file " + db.memorylist_file);
        return false;
    }
    std::string line = "";

    std::map<std::string, Memory> tmp;
    std::vector<std::string> lib_names;
    std::string name;

    while (getline(input, line))
    {
        if (line.empty() || line.find("totally") != std::string::npos)
            continue;
            
        if (line.find(':') != std::string::npos)
        {
            int t = line.find(':');
            name = line.substr(0, t);
            lib_names.emplace_back(name);
        }
        else
        {
            Memory *m = new Memory;
            m->mem_Path = line;
            m->mem_Name = name;

            auto it_path = memorysMappedByPath.find(m->mem_Path);
            if (it_path != memorysMappedByPath.end())
            {
                it_path->second = m;
            }
            else
            {
                memorysMappedByPath.insert(std::pair<std::string, Memory*>(m->mem_Path, m));
            }

            auto it_name = memorysMappedByName.find(m->mem_Name);
            if (it_name != memorysMappedByName.end())
            {
                it_name->second.insert(m);
            }
            else
            {
                std::set<Memory*> tmp{m};
                memorysMappedByName.insert(std::pair<std::string, std::set<Memory*>>(m->mem_Name, std::move(tmp)));
            }
        }
    }

    this->check = new bool[memorysMappedByPath.size()];
    // std::cout << "num = " << memorysMappedByPath.size() << std::endl;
    input.close();
    return true;
}

double Parser::GetLastDouble(std::string line)
{
    std::string power_rev;
    for (auto it = line.rbegin(); it != line.rend(); ++it)
    {
        if (*it != ' ')
        {
            power_rev += *it;
        }
        else
        {
            if (!power_rev.empty())
                break;
            else
                continue;
        }
    }
    std::string power_str(power_rev.rbegin(),power_rev.rend());
    return std::stod(power_str);
}

double Parser::ParseDS_summ(std::string ds)
{
    std::ifstream input(ds);
    if (!input.is_open())
    {
        std::cout << "ERROR 5! No such file " << ds << std::endl;
        logger.log("[ParseDataSheet] ERROR 5! No such file " + ds);
    }
    std::string line = "";

    while (getline(input,line))
    {
        if (line.find("Power") != std::string::npos)
        {
            do {
                getline(input,line);
                return GetLastDouble(line);
            } while (line.empty());            
        }
    }

    return -1.0;
}

double Parser::ParseDS_ds02(std::string ds)
{
    std::ifstream input(ds);
    if (!input.is_open())
    {
        std::cout << "ERROR 5! No such file " << ds << std::endl;
        logger.log("[ParseDataSheet] ERROR 5! No such file " + ds);
    }

    std::string line = "";

    while (getline(input,line))
    {
        if (line.find("Dynamic Power") != std::string::npos)
        {
            while (getline(input,line))
            {
                if (line.find("------") != std::string::npos)
                    break;
            }
            double max_dy_power = -1.0;
            while (getline(input,line))
            {
                if (line.empty())
                    continue;
                if (line.find("Standby Mode") != std::string::npos)
                    break;
                
                double dy_power = GetLastDouble(line);
                max_dy_power = dy_power > max_dy_power ? dy_power : max_dy_power;
            }

            return max_dy_power;
        }
    }
    return -1.0;
}

void Parser::ParseDataSheet(std::string ds)
{
    std::string cellname = "";
    double power = 0.0;
    for (auto i : memorysMappedByName)
    {
        if (ds.find(i.first) != std::string::npos)
        {
            cellname = i.first;
            break;
        }
    }
    auto it = memorysMappedByName.find(cellname);
    auto mem = *(it->second).begin();
    if (mem->dynamic_power > 0)
    {
        return;
    }


    if (ds.find(".summ") != std::string::npos)
    {
        power = ParseDS_summ(ds);
    }
    else
    {
        power = ParseDS_ds02(ds);
    }

    for (auto &i : it->second)
    {
        i->dynamic_power = power;
    }  
}

bool Parser::ParseDef()
{
    std::ifstream input(db.def_file);
    if (!input.is_open())
    {
        std::cout << "ERROR 1! No such file " << db.def_file << std::endl;
        logger.log("[ParseDef] ERROR 1! No such file " + db.def_file);
        return false;
    }
    std::string line = "";
    while (getline(input, line))
    {
        int i = 2;
        std::string path = "";
        std::string mem_type = "";
        std::string Up = "";
        std::string Low = "";

        while (line[i] != ' ') // get path
        {
            path += line[i];
            i++;
        }
        i++;
        while (line[i] != ' ') // get name
        {
            mem_type += line[i];
            i++;
        }

        int k = line.find('(');
        k += 2;
        std::string str1 = "";
        std::string str2 = "";
        while (line[k] != ' ') // get up
        {
            str1 += line[k];
            k++;
        }
        k++;
        while (line[k] != ' ') // get low
        {
            str2 += line[k];
            k++;
        }
        int num1 = atoi(str1.c_str());
        int num2 = atoi(str2.c_str());

        int up = 0;
        int low = 0;
        if (num1 > num2)
        {
            up = num1;
            low = num2;
        }
        else
        {
            up = num2;
            low = num1;
        }

        auto it_path = memorysMappedByPath.find(path);
        if (it_path != memorysMappedByPath.end())
        {
            it_path->second->up_bound = up;
            it_path->second->low_bound = low;
        }
        else
        {
            std::cout << "error: " << path << std::endl;
            logger.log("[ParseDef] ERROR 1! No such mem " + path);
        }

        //cnt++;
    }
    input.close();
    return true;
}

void Parser::ParseLvlib(std::string lvlib)
{
    std::ifstream input(lvlib);

    if (!input.is_open())
    {
        std::cout << "ERROR 2! No such file " << lvlib << std::endl;
        logger.log("[ParseLvlib] ERROR 2! No such file " + lvlib);
    }

    std::string line;
    std::set<Memory *> mems;

    while (getline(input, line))
    {
        if (line.find("CellName") != std::string::npos)
        {
            std::string cellname = GetInfoFromStr(line);
            auto iter = memorysMappedByName.find(cellname);
            if (iter != memorysMappedByName.end())
                mems = iter->second;

            while (getline(input, line))
            {
                if (line.find("Algorithm") != std::string::npos)
                {
                    auto line_vec = SplitBySpace(line);
                    int index;
                    for (index = 0; index < line_vec.size(); ++index)
                    {
                        if (line_vec[index].find(":") != std::string::npos)
                            break;
                    }
                    std::vector<std::string> algos;
                    for (++index; index < line_vec.size(); ++index)
                    {
                        if (line_vec[index].find(";") == std::string::npos)
                            algos.emplace_back(line_vec[index]);
                        else if (line_vec[index].size() == 1)
                            break;
                        else
                        {
                            line_vec[index].pop_back();
                            algos.emplace_back(line_vec[index]);
                        }
                    }
                    for (auto mem : mems)
                    {
                        mem->Algorithms = algos;
                    }
                }
                else if (line.find("NumberofWords") != std::string::npos)
                {
                    int words = std::stoi(GetInfoFromStr(line));
                    for (auto mem : mems)
                    {
                        mem->NumberofWords = words;
                    }
                }
                else if (line.find("NumberofBits") != std::string::npos)
                {
                    int bits = std::stoi(GetInfoFromStr(line));
                    for (auto mem : mems)
                    {
                        mem->NumberofBits = bits;
                    }
                }
                else if (line.find("MilliWattsPerMegaHertz") != std::string::npos)
                {
                    double power = std::stod(GetInfoFromStr(line));
                    for (auto mem : mems)
                    {
                        // mem->MilliWattsPerMegaHertz = std::stod(GetInfoFromStr(line));
                        mem->dynamic_power = power;
                    }
                }
                else if (line.find("OperationSet") != std::string::npos)
                {
                    for (auto mem : mems)
                    {
                        mem->OperationSet = GetInfoFromStr(line);
                    }
                }
            }
        }

    }
    input.close();
}

void Parser::ParseLib(std::string lib)
{
    std::ifstream input(lib);

    if (!input.is_open())
    {
        std::cout << "ERROR 3! No such file " << lib << std::endl;
        logger.log("[ParseLib] ERROR 3! No such file " + lib);
    }
    std::string line = "";
    std::string cellname = "";
    std::string type = "";
    std::string a_width = "";
    std::string w_width = "";
    std::string Area = "";
    std::string timing_pin = "";
    int a = 0;
    int w = 0;
    float AREA = 0.0;

    while (getline(input, line))
    {
        if (line.find("cell ( ") != std::string::npos)
        {
            int t = line.find('(');
            t += 2;
            while (line[t] != ' ')
            {
                cellname += line[t];
                // if (cellname.empty())
                // {
                //     std::cout << line << std::endl;
                // }
                t++;
            }
            continue;
        }
        else if (line.find("memory () {") != std::string::npos)
        {
            getline(input, line);
            int t = line.find(':');
            t += 2;
            while (line[t] != ' ')
            {
                type += line[t];
                t++;
            }
            getline(input, line);
            t = line.find(':');
            t += 2;
            while (line[t] != ' ')
            {
                a_width += line[t];
                t++;
            }
            getline(input, line);
            t = line.find(':');
            t += 2;
            while (line[t] != ' ')
            {
                w_width += line[t];
                t++;
            }
            a = atoi(a_width.c_str());
            w = atoi(w_width.c_str());
        }
        else if (line.find("area : ") != std::string::npos)
        {
            int t = line.find(':');
            t += 2;
            while (line[t] != ' ')
            {
                Area += line[t];
                t++;
            }
            AREA = std::stof(Area.c_str());
        }
        else
        {
            continue;
        }
    }

    auto it = memorysMappedByName.find(cellname);
    if (it != memorysMappedByName.end())
    {
        for (auto &i : it->second)
        {
            i->mem_type = (type == "ram") ? RAM : ROM;
            i->address_width = a;
            i->word_width = w;
            i->area = AREA;
        }
    }
    else
    {
        std::cout << cellname << " not found in memorysMappedByName" << std::endl;
        logger.log("[ParseLib] ERROR ! [" + cellname + "] not found in memorysMappedByName");
    }

    input.close();
}

bool Parser::ParseCLK()
{
    std::ifstream input(db.clk_file);

    if (!input.is_open())
    {
        std::cout << "ERROR 3! No such file " << db.clk_file << std::endl;
        logger.log("[ParseCLK] ERROR 3! No such file " + db.clk_file);
        return false;
    }

    std::string line = "";
    std::string clk;
    getline(input, line);  // skip the first line
    while (getline(input, line))
    {
        if (line.empty())
            continue;

        if (line.find("-group ") != std::string::npos)
        {
            size_t pos1 = line.find('{');
            size_t pos2 = line.find('}', pos1 + 1);
            
            std::string clks_str = line.substr(pos1 + 1, pos2 - pos1 - 1);
            auto clks = SplitBySpace(clks_str);
        }
        else if (line.find(":") != std::string::npos)  // update clk name
        {
            size_t pos1 = line.find(':');
            clk = line.substr(0, pos1);
            ++this->clkDomainNum;
        }
        else if (line.find("/") != std::string::npos)  // may not have "/"
        {
            auto it = memorysMappedByPath.find(line);
            it->second->clk_domain = clk;
        }
    }
    input.close();
    return true;
}

bool Parser::GetFileNameFromFolder(std::string path, std::vector<std::string> &filenames)
{
    DIR *pDir;
    struct dirent *ptr;
    if (!(pDir = opendir(path.c_str())))
    {
        std::cout << "Folder doesn't Exist!" << path << std::endl;
        logger.log("[GetFileNameFromFolder] Folder doesn't Exist!" + path);
        return false;
    }
    while ((ptr = readdir(pDir)) != 0)
    {
        if (strcmp(ptr->d_name, ".") != 0 && strcmp(ptr->d_name, "..") != 0)
        {
            filenames.push_back(path + ptr->d_name);
        }
    }
    closedir(pDir);
    return true;
}

#endif