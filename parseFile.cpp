#ifndef _PARSEFILE_CPP_
#define _PARSEFILE_CPP_

#include "global.hpp"
#include "parser.hpp"

extern dataBase db;

std::vector<std::string> SplitBySpace(std::string str)
{
    std::string buf;
    std::stringstream ss(str);

    std::vector<std::string> tokens;

    while (ss >> buf)
        tokens.emplace_back(buf);

    return tokens;
}

void Parser::ParseMemList()
{
    std::ifstream input(db.memorylist_file);
    if (!input.is_open())
    {
        std::cout << "ERROR 0! No such file " << db.memorylist_file << std::endl;
    }
    std::string line = "";

    std::map<std::string, Memory> tmp;
    std::vector<std::string> lib_names;
    std::string name;

    while (getline(input, line))
    {
        if (line.empty())
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
    input.close();
}

void Parser::ParseDataSheet(std::string ds)
{
    std::string cellname = "";
    int HZ = 0;
    float power = 0.0;
    for (auto i : memorysMappedByName)
    {
        if (ds.find(i.first) != std::string::npos)
        {
            cellname = i.first;
            break;
        }
    }
    auto it = memorysMappedByName.find(cellname);


    if (ds.find(".summ") != std::string::npos)
    {
        std::ifstream input(ds);
        if (!input.is_open())
        {
            std::cout << "ERROR 5! No such file " << ds << std::endl;
        }
        std::string line = "";
        std::string allpower = "";


        while (getline(input,line))
        {
            if (line.find("Power(mW/MHz)") != std::string::npos)
            {
                getline(input,line);
                int i = 0;
                while (i < line.length())
                {
                    if (i >= line.length())
                    {
                        break;
                    }
                    
                    int i = line.length() - 1;
                    while (line[i] != ' ')
                    {
                        i--;
                    }
                    i++;

                    allpower += line[i];
                    i++;
                }

                power = std::stof(allpower.c_str());
            }
            else
            {
                continue;
            }
        }

        for (auto &i : it->second)
        {
            i->total_power = power;
        } 
    }
    else
    {
        std::ifstream input(ds);
        if (!input.is_open())
        {
            std::cout << "ERROR 6! No such file " << ds << std::endl;
        }
        std::string line = "";
        std::string static_power = "";      
        float Leakage_power = 0.0;
        float dynamic_Power = 0.0;
        

        while (getline(input,line))
        {
            if (line.find("Leakage Current") != std::string::npos)
            {
                int t = line.find('t');
                t += 2;
                if (line[t] == '=')
                {
                    continue;
                }
                else
                {
                    while (line[t] == ' ')
                    {
                        t++;
                    }
                    while (line[t] != '(')
                    {
                        static_power += line[t];
                        t++;
                    }
                    Leakage_power = std::stof(static_power.c_str());   
                }
            }
            else if (line.find("Dynamic Power") != std::string::npos && line.find("Average") != std::string::npos)
            {
                while (line.find("--") == std::string::npos)
                {
                    getline(input,line);
                }
                getline(input,line);
                while (line.length() != 0)
                {
                    std::string Dynamic_Power = "";
                    int t = line.length() - 1;
                    while (line[t] != ' ')
                    {
                        t--;
                    }
                    t++;
                    while (t < line.length())
                    {
                        Dynamic_Power += line[t];
                        t++;
                    }
                    float tmp = std::stof(Dynamic_Power.c_str());
                    if (tmp > dynamic_Power)
                    {
                        dynamic_Power = tmp;
                    }
                    getline(input,line);
                }
                break;
            }
        }

        for (auto &i : it->second)
        {
            i->dynamic_power = dynamic_Power;
            i->leakage_power = Leakage_power;
            i->total_power = i->dynamic_power + i->leakage_power;
        }  
    }
}

void Parser::ParseDef()
{
    std::ifstream input(db.def_file);
    if (!input.is_open())
    {
        std::cout << "ERROR 1! No such file " << db.def_file << std::endl;
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
        }

        //cnt++;
    }
    input.close();
}

void Parser::ParseLvlib(std::string lvlib)
{
    std::ifstream input(lvlib);

    if (!input.is_open())
    {
        std::cout << "ERROR 2! No such file " << lvlib << std::endl;
    }
    std::string line = "";

    std::string cellname = "";
    float mwmhz = 0.0;
    int cnt = 0;

    while (getline(input, line))
    {
        cnt++;
        if (line.find("Algorithm") != std::string::npos)
        {
            int t = line.find(':');
            t += 2;
            std::string str = "";
            auto it = memorysMappedByName.find(cellname);
            // std::cout << cellname << std::endl;
            while (t < line.length())
            {
                if (t >= line.length())
                {
                    break;
                }
                if (line[t] == ' ' || line[t] == ';')
                {
                    for (auto &i : it->second)
                    {
                        //i->Algorithms.insert(std::pair<std::string, int>(str, 1));
                        i->Algorithms.emplace_back(str);
                    }
                    str = "";
                    t++;
                    continue;
                }
                else
                {
                    str += line[t];
                    t++;
                    continue;
                }
            }
        }
        else if (line.find("NumberOfWords") != std::string::npos)
        {
            auto it = memorysMappedByName.find(cellname);
            int t = line.find(':');
            t += 2;
            std::string str = "";
            while (line[t] != ';')
            {
                str += line[t];
                t++;
            }
            int words = std::stoi(str.c_str());
            for (auto &i : it->second)
            {
                i->NumberOfWords = words;
            }
        }
        else if (line.find("CellName") != std::string::npos)
        {
            int t = line.find(':');
            t += 2;
            while (line[t] != ';')
            {
                cellname += line[t];
                t++;
            }
        }
        else if (line.find("MilliWattsPerMegaHertz") != std::string::npos)
        {
            int t = line.find(':');
            t += 2;
            std::string MwMHz = "";
            auto it = memorysMappedByName.find(cellname);
            while (line[t] != ';')
            {
                MwMHz += line[t];
                t++;
            }
            mwmhz = std::stof(MwMHz.c_str());
            for (auto &i : it->second)
            {
                i->MilliWattsPerMegaHertz = mwmhz;
            }
        }
        else if (line.find("Port (") != std::string::npos || line.find("port (") != std::string::npos)
        {
            std::string str = "";
            while (line.find('}') == std::string::npos)
            {
                str += line;
                getline(input,line);
            }
            if (str.find("Function: Clock") != std::string::npos)
            {
                std::string clock = "";
                int t = str.find('(');
                t++;
                while (str[t] == ' ')
                {
                    t++;
                }
                while (str[t] != ' ' && str[t] != ')')
                {
                    clock += str[t];
                    t++;
                }
                auto it = memorysMappedByName.find(cellname);
                for (auto &i : it->second)
                {
                    i->Clock_Siganls.push_back(clock);
                }
            }
            else
            {
                continue;
            }
        }
        else
        {
            continue;
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
    for (auto &i : it->second)
    {
        i->mem_type = (type == "ram") ? RAM : ROM;
        i->address_width = a;
        i->word_width = w;
        i->area = AREA;
    }

    input.close();
}

void Parser::ParseCLK()
{
    std::ifstream input(db.clk_file);

    if (!input.is_open())
    {
        std::cout << "ERROR 3! No such file " << db.clk_file << std::endl;
    }

    std::string line = "";
    std::string clk;
    decltype(this->clkDomainMap.find(clk)) iter;
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
        }
        else if (line.find("/") != std::string::npos)  // may not have "/"
        {
            iter = this->clkDomainMap.find(clk);
            auto it = memorysMappedByPath.find(line);
            it->second->clk_domain = clk;
            if (iter != this->clkDomainMap.end())
            {
                iter->second.emplace_back(line);
            }
            else
            {
                std::vector<std::string> tmp(1, line);
                this->clkDomainMap.insert(std::pair<std::string, std::vector<std::string>>(clk, std::move(tmp)));
            }
        }
    }
}

void Parser::GetFileNameFromFolder(std::string path, std::vector<std::string> &filenames)
{
    DIR *pDir;
    struct dirent *ptr;
    if (!(pDir = opendir(path.c_str())))
    {
        std::cout << "Folder doesn't Exist!" << std::endl;
        return;
    }
    while ((ptr = readdir(pDir)) != 0)
    {
        if (strcmp(ptr->d_name, ".") != 0 && strcmp(ptr->d_name, "..") != 0)
        {
            filenames.push_back(path + ptr->d_name);
        }
    }
    closedir(pDir);
}

#endif