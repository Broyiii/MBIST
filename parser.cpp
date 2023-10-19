#include "parser.hpp"

// using namespace std;

void Parser::ParseMemList()
{
    std::ifstream input(this->memorylist_file);
    if (!input.is_open())
    {
        std::cout << "ERROR 0! No such file " << this->memorylist_file << std::endl;
    }
    std::string line = "";

    Memory m;
    std::map<std::string, Memory> tmp;
    std::vector<std::string> lib_names;

    while (getline(input, line))
    {
        if (line.find(':') != std::string::npos)
        {
            int t = line.find(':');
            m.mem_Name = line.substr(0, t);
            lib_names.push_back(m.mem_Name);
        }
        else if (!line.length())
        {
            continue;
        }
        else
        {
            m.mem_Path = line;
            auto it = memorys.find(m.mem_Name);
            if (it != memorys.end())
            {
                it->second.insert(std::pair<std::string, Memory>(m.mem_Path, m));
            }
            else
            {
                m.mem_Path = line;
                tmp.insert(std::pair<std::string, Memory>(m.mem_Path, m));
                memorys.insert(std::pair<std::string, std::map<std::string, Memory>>(m.mem_Name, tmp));
                tmp.clear();
            }
        }
    }
    input.close();
}

void Parser::ParseDataSheet(std::string ds)
{
    std::string cellname = "";
    float power = 0.0;
    for (auto i : memorys)
    {
        if (ds.find(i.first) != std::string::npos)
        {
            cellname = i.first;
            break;
        }
    }
    auto it = memorys.find(cellname);


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
        
    }
    else
    {
        std::ifstream input(ds);
        if (!input.is_open())
        {
            std::cout << "ERROR 6! No such file " << ds << std::endl;
        }
        std::string line = "";
        std::string Leakage_power = "";
        std::string temp = "";
        

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
                        Leakage_power += line[t];
                        t++;
                    }
                    power = std::stof(Leakage_power.c_str());   
                }
            }
            else if (line.find("Breakout per pin") != std::string::npos)
            {
                while (line.find("--") == std::string::npos)
                {
                    getline(input,line);
                }
                getline(input,line);
                while (line.length() != 0)
                {
                    int t = 0;
                    while (t < line.length())
                    {
                        if (t >= line.length())
                        {
                            break;
                        }

                        if (line.find('[') != std::string::npos)
                        {   
                            t = line.find(']');
                            t++;
                            while (!(line[t] >= '0' && line[t] <= '9'))
                            {
                                t++;
                            }
                            while (line[t] != '(')
                            {
                                temp += line[t];
                                t++;
                            }

                        }
                        else
                        {
                            while (!(line[t] >= '0' && line[t] <= '9'))
                            {
                                t++;
                            }
                            while (line[t] != '(')
                            {
                                temp += line[t];
                                t++;
                            }
                        }
                        

                        break;
                    }
                    power += std::stof(temp.c_str());
                    temp = "";
                    getline(input,line);
                }
            }
        }
        
    }

    for (auto &i : it->second)
    {
        i.second.total_power = power;
    }


}

void Parser::ParseDef()
{
    std::ifstream input(this->def_file);
    if (!input.is_open())
    {
        std::cout << "ERROR 1! No such file " << this->def_file << std::endl;
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

        auto it = memorys.find(mem_type);
        if (it != memorys.end())
        {
            auto it_path = it->second.find(path);
            if (it_path != it->second.end())
            {
                it_path->second.up_bound = up;
                it_path->second.low_bound = low;
            }
            else
            {
                std::cout << "error: " << path << std::endl;
            }
        }
        else
        {
            std::cout << "error: " << mem_type << std::endl;
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
            auto it = memorys.find(cellname);
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
                        i.second.Algorithms.insert(std::pair<std::string, int>(str, 1));
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
            auto it = memorys.find(cellname);
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
                i.second.NumberOfWords = words;
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
            auto it = memorys.find(cellname);
            while (line[t] != ';')
            {
                MwMHz += line[t];
                t++;
            }
            mwmhz = std::stof(MwMHz.c_str());
            for (auto &i : it->second)
            {
                i.second.MilliWattsPerMegaHertz = mwmhz;
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
                auto it = memorys.find(cellname);
                for (auto &i : it->second)
                {
                    i.second.Clock_Siganls.push_back(clock);
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

    auto it = memorys.find(cellname);
    for (auto &i : it->second)
    {
        i.second.mem_type = (type == "ram") ? RAM : -1;
        i.second.address_width = a;
        i.second.word_width = w;
        i.second.area = AREA;
    }

    input.close();
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

void Parser::GetAllFileNames()
{
    GetFileNameFromFolder(this->work_dir + "ds/", this->ds_files);
    GetFileNameFromFolder(this->work_dir + "lib/", this->lib_files);
    GetFileNameFromFolder(this->work_dir + "lvlib/", this->lvlib_files);
    GetFileNameFromFolder(this->work_dir + "verilog/", this->verilog_files);

    ParseMemList();
    ParseDef();  

    for (int i = 0; i < this->lvlib_files.size(); i++)
    {
        for (auto &j : memorys)
        {
            if (this->lvlib_files[i].find(j.first) != std::string::npos)
            {
                ParseLvlib(this->lvlib_files[i]);
            }

            if (this->lib_files[i].find(j.first) != std::string::npos)
            {
                ParseLib(this->lib_files[i]);
            }

            if (this->ds_files[i].find(j.first) != std::string::npos)
            {
                ParseDataSheet(this->ds_files[i]);
            }

        }
    }

}

void Parser::DivByRowCol()
{
    for (auto i : memorys)
    {
        for (auto j : i.second)
        {
            std::string numofwords = std::to_string(j.second.NumberOfWords);
            std::string widthofwords = std::to_string(j.second.word_width);
            std::string str = numofwords + '_' + widthofwords;
            if (AfterDivByRowCol.find(str) != AfterDivByRowCol.end())
            {
                auto it = AfterDivByRowCol.find(str);
                it->second.push_back(j.second);
            }
            else
            {
                std::vector<Memory> tmp;
                tmp.push_back(j.second);
                AfterDivByRowCol.insert(std::pair<std::string,std::vector<Memory>>(str,tmp));
                tmp.clear();
            }
        }
    }
}

void Parser::Print()
{
    std::cout << "\nmemory size = " << memorys.size() << std::endl;
    for (auto i : memorys)
    {
        std::cout << i.first << std::endl;
        for (auto &j : i.second)
        {
            std::cout << "Path: " << j.first << " Low_limit: " << j.second.low_bound << " Up_limit: " << j.second.up_bound << " NumberOfWords: " << j.second.NumberOfWords << std::endl;
            std::cout << "Algorithms: ";
            for (auto &k : j.second.Algorithms)
            {
                std::cout << k.first << " ";
            }
            std::cout << std::endl;
            std::cout << "Clock_Siganls: ";
            for (auto t : j.second.Clock_Siganls)
            {
                std::cout << t << " ";
            }
            std::cout << std::endl;
            std::cout << "Area: " << j.second.area << " mem_type: " << j.second.mem_type << " MilliWattsPerMegaHertz: " << j.second.MilliWattsPerMegaHertz << std::endl;
            std::cout << "Total power: " << j.second.total_power << std::endl;
            std::cout << " address_width: " << j.second.address_width << " word_width: " << j.second.word_width << "\n" << std::endl;
        }
    }
    std::cout << "------------------------------------------------------------------------------------------------" << std::endl;
    for (auto i : AfterDivByRowCol)
    {
        std::cout << i.first << ": " << std::endl;
        for (auto j : i.second)
        {
           std::cout << j.mem_Path + '/' + j.mem_Name << std::endl;
        }
        std::cout << std::endl;
    }
}

void Parser::GetInformationFromFile()
{
    // parser file
    GetAllFileNames();

    PrintMemInfo();

    //grouping
    DivByRowCol();


    // Show Information
    Print();
}
