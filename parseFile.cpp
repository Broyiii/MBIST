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
            m->GetBlock();

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

void Parser::ParseDS_summ(std::string ds, double &power, double &width, double &height)
{
    power = -2.0;
    width = -2.0;
    height = -2.0;
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
                power = GetLastDouble(line);
            } while (line.empty());            
        }
        else if (line.find("Width") != std::string::npos)
        {
            do {
                getline(input,line);
                auto str_vec = SplitBySpace(line);
                width = std::stod(str_vec[str_vec.size()-2]);
                height = std::stod(str_vec[str_vec.size()-1]);
            } while (line.empty());   
        }
    }
}

void Parser::ParseDS_ds02(std::string ds, double &power, double &width, double &height)
{
    power = -3.0;
    width = -3.0;
    height = -3.0;
    std::ifstream input(ds);
    if (!input.is_open())
    {
        std::cout << "ERROR 5! No such file " << ds << std::endl;
        logger.log("[ParseDataSheet] ERROR 5! No such file " + ds);
    }

    std::string line = "";

    while (getline(input,line))
    {
        // ds version 1
        if (line.find("Memory Area") != std::string::npos)
        {
            size_t pos = line.find(":");
            ++pos;
            std::string str = "";
            for (; pos < line.size(); ++pos)
            {
                if (line[pos] == ' ')
                    continue;
                else if (line[pos] == 'x')
                {
                    width = std::stod(str);
                    str.clear();
                    continue;
                }
                else if (line[pos] == '=')
                    break;
                else
                    str += line[pos];
            }
            height = std::stod(str);
            str.clear();
            power = -1.0;
            return;
        }
        // ds version 2
        if (line.find("1. Area") != std::string::npos)
        {
            do { 
                getline(input,line);
            } while (line.find(".") == std::string::npos);
            auto str_vec = SplitBySpace(line);
            width = std::stod(str_vec[1]);
            height = std::stod(str_vec[3]);
        }
        else if (line.find("Dynamic Power") != std::string::npos)
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
                    return;
                
                double dy_power = GetLastDouble(line);
                max_dy_power = dy_power > max_dy_power ? dy_power : max_dy_power;
            }

            power = max_dy_power;
        }
    }
    // return -1.0;
}

void Parser::ParseSUMM(std::string ds)
{
    std::string cellname = "";
    for (auto i : memorysMappedByName)
    {
        if (ds.find(i.first) != std::string::npos)
        {
            cellname = i.first;
            break;
        }
    }
    auto it = memorysMappedByName.find(cellname);

    double power, width, height;
    ParseDS_summ(ds, power, width, height);

    for (auto &i : it->second)
    {
        i->width = width;
        i->height = height;
        if (i->dynamic_power < 0)
            i->dynamic_power = power;
        

    }  
}

void Parser::ParseDataSheet(std::string ds)
{
    std::string cellname = "";
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
    if ((mem->dynamic_power > 0) && (mem->width > 0) && (mem->height > 0))
    {
        return;
    }

    double power, width, height;
    ParseDS_ds02(ds, power, width, height);

    for (auto &i : it->second)
    {
        if (i->width < 0)
            i->width = width;
        if (i->height < 0)
            i->height = height;
        if (i->dynamic_power < 0)  
            i->dynamic_power = power;
    }  
}

bool Parser::ParseDef()
{
    std::ifstream input(db.def_file);
    if (!input.is_open())
    {
        // std::cout << "WARNING ! No def file " << db.def_file << std::endl;
        logger.log("[ParseDef] WARNING ! No def file " + db.def_file);
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

        // get direct
        while (line[k] != ')')
            ++k;
        ++k;
        while (line[k] != ' ')
            ++k;
        std::string dir = "";
        while (line[k] != '+')
        {
            if (line[k] == ' ')
            {
                ++k;
                continue;
            }
            dir += line[k];
            ++k;

            if ((dir.find("W") != std::string::npos) || (dir.find("E") != std::string::npos))
            {
                std::cout << "11111" << std::endl;
            }
        }

        auto it_path = memorysMappedByPath.find(path);
        if (it_path != memorysMappedByPath.end())
        {
            it_path->second->up_bound = up;
            it_path->second->low_bound = low;
            it_path->second->direct = dir;
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

void Parser::ParseSpec()
{
    std::ifstream input(db.spec_file);

    if (!input.is_open())
    {
        std::cout << "ERROR 10! No such file " << db.spec_file << std::endl;
        logger.log("[ParseLvlib] ERROR 2! No such file " + db.spec_file);
    }

    std::string line;

    while (getline(input,line))
    {
        if (line.find("max_distance") != std::string::npos)
        {
            if ((db.dis_max > 0) || (!this->distanceCon))
                continue;
            int t = line.find('=');
            t++;
            while (line[t] == ' ')
            {
                t++;
            }
            std::string dis = "";
            while (line[t] != ';')
            {
                dis += line[t];
                t++;
            }
            db.dis_max = std::stoll(dis.c_str());
        }
        else if (line.find("UNITS_DISTANCE_MICRONS") != std::string::npos)
        {
            // if (!this->distanceCon)
            //     continue;
            size_t pos = line.find("=");
            std::string str = "";
            for (++pos; pos < line.size(); ++pos)
            {
                if (line[pos] == ' ')
                    continue;
                else if (line[pos] == ';')
                    break;
                else
                    str += line[pos];
            }
            db.distance_unit = std::stoi(str);
        }
        else if (line.find("max_design_heirarchical_distance") != std::string::npos)
        {
            if ((db.inputBlock > 0) || (this->distanceCon))
                continue;
            size_t pos = line.find("=");
            std::string str = "";
            for (++pos; pos < line.size(); ++pos)
            {
                if (line[pos] == ' ')
                    continue;
                else if (line[pos] == ';')
                    break;
                else
                    str += line[pos];
            }
            db.block_max = std::stoi(str);
        }
        else if (line.find("max_dynamic_power") != std::string::npos)
        {
            if (db.power_max > 0)
                continue;
            int t = line.find('=');
            t++;
            while (line[t] == ' ')
            {
                t++;
            }
            std::string power = "";
            while (line[t] != ';')
            {
                power += line[t];
                t++;
            }
            db.power_max = std::stod(power.c_str());
        }

    }
    input.close();
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
                else if (line.find("MemoryType") != std::string::npos)
                {
                    for (auto mem : mems)
                    {
                        mem->mem_type = GetInfoFromStr(line);
                    }
                }
                else if (line.find("ogicalPorts") != std::string::npos)  // logicalPorts & LogicalPorts
                {
                    for (auto mem : mems)
                    {
                        mem->logicalPorts = GetInfoFromStr(line);
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
                else if (line.find("ShadowRead") != std::string::npos)
                {
                    for (auto mem : mems)
                    {
                        mem->ShadowRead = GetInfoFromStr(line);
                    }
                }
                else if (line.find("ShadowWrite") != std::string::npos)
                {
                    for (auto mem : mems)
                    {
                        mem->ShadowWrite = GetInfoFromStr(line);
                    }
                }
                else if (line.find("ShadowWriteOK") != std::string::npos)
                {
                    for (auto mem : mems)
                    {
                        mem->ShadowWriteOK = GetInfoFromStr(line);
                    }
                }
                else if (line.find("WriteOutOfRange") != std::string::npos)
                {
                    for (auto mem : mems)
                    {
                        mem->WriteOutOfRange = GetInfoFromStr(line);
                    }
                }
                else if (line.find("LogicalAddressMaP") != std::string::npos)
                {
                    bool breakFlag = false;
                    std::map<int, std::string> tmp_map;  // <Address[id], map_addr[id]>
                    do {
                        std::string addrMapLine;
                        do {
                            getline(input, line);
                            if (line.find("}") != std::string::npos)
                            {
                                breakFlag = true;
                                break;
                            }
                            addrMapLine += line;
                        } while (addrMapLine.find(";") == std::string::npos);

                        if (breakFlag)
                            break;

                        size_t pos = 0;
                        AddrMap map_addr = GetLogicalAddressMaPInfoUtil(addrMapLine, pos);
                        map_addr.Print();
                        while (addrMapLine[pos] != ':')
                        {
                            ++pos;
                        } ++pos;
                        AddrMap addr = GetLogicalAddressMaPInfoUtil(addrMapLine, pos);
                        if (addr.name != "Address")
                        {
                            logger.log("[ParseLvlib] [LogicalAddressMaP] ERROR ! Name not Address !");
                        }
                        addr.Print();
                        
                        for (int addr_id = addr.down; addr_id <= addr.up; ++addr_id)
                        {
                            int offset = addr_id - addr.down;
                            tmp_map.insert(std::pair<int, std::string>(addr_id, map_addr.GetName(offset)));
                        }
                    } while (true);
                    
                    std::vector<std::string> tmp(tmp_map.size());
                    for (int id = 0; id < tmp_map.size(); ++id)
                    {
                        auto iter = tmp_map.find(id);
                        if (iter != tmp_map.end())
                        {
                            tmp[id] = iter->second;
                        }
                        else
                        {
                            logger.log("[ParseLvlib] [LogicalAddressMaP] ERROR ! No such id [ " + std::to_string(id) + " ] !");
                        }
                    }
                    for (auto mem : mems)
                    {
                        mem->LogicalAddressMap = tmp;
                    }
                }
                
            }
        }

    }
    input.close();
}

AddrMap Parser::GetLogicalAddressMaPInfoUtil(std::string &line, size_t &pos)
{
    std::string map_addr = "";
    // get map_addr name
    while (line[pos] != '[')
    {
        auto c = line[pos++];
        if (c == ' ') 
            continue;
        map_addr += c;
    } ++pos;
    // get map_addr id
    std::string up_str = "";
    std::string down_str = "";
    while (line[pos] != ':')
    {
        auto c = line[pos++];
        if (c == ' ') 
            continue;
        else if (c == ']')
        {
            return AddrMap(map_addr, std::stoi(up_str), std::stoi(up_str));
        }
        up_str += c;
    } ++pos;
    while (line[pos] != ']')
    {
        auto c = line[pos++];
        if (c == ' ') 
            continue;
        down_str += c;
    } ++pos;
    return AddrMap(map_addr, std::stoi(up_str), std::stoi(down_str));
}

void Parser::ParseLib(std::string lib)
{
    std::ifstream input(lib);

    if (!input.is_open())
    {
        std::cout << "ERROR 3! No such file " << lib << std::endl;
        logger.log("[ParseLib] ERROR 3! No such file " + lib);
        return;
    }

    std::string line;
    while (getline(input, line))
    {
        if (line.find("cell ( ") != std::string::npos)
        {
            std::string cellname;
            for (size_t pos = line.find("(") + 1; line[pos] != ')'; ++ pos)
            {
                if (line[pos] == ' ')
                    continue;
                cellname += line[pos];
            }
            std::set<Memory *> mems;
            auto iter = memorysMappedByName.find(cellname);
            if (iter != memorysMappedByName.end())
                mems = iter->second;
            else
            {
                logger.log("[ParseLib] ERROR ! No such mem " + cellname);
                goto ParseLibEnd;
            }

            if ((*mems.begin())->mem_type.empty())
            {
                while (getline(input, line))
                {
                    if (line.find("type") != std::string::npos)
                    {
                        for (auto mem : mems)
                        {
                            mem->mem_type = GetInfoFromStr(line);
                        }
                        goto ParseLibEnd;
                    }
                }
            }
            else
                goto ParseLibEnd;
        }
    }

    ParseLibEnd:
    input.close();
}

bool Parser::ParseCLK()
{
    std::ifstream input(db.clk_file);

    if (!input.is_open())
    {
        // std::cout << "WARNING ! No clk file " << db.clk_file << std::endl;
        logger.log("[ParseCLK] WARNING ! No clk file " + db.clk_file);
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
        // std::cout << "Folder doesn't Exist!" << path << std::endl;
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