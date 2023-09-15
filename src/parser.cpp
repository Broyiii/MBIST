#include "parser.h"
#include <algorithm>
#include <iomanip>
#include <math.h>
#include <fstream>
#include <sstream>

using namespace std;

void Parser::parsermemlist(std::string filename)
{
    std::ifstream file(filename, std::ios::binary);
	std::vector<char> buf_tmp(static_cast<unsigned int>(file.seekg(0, std::ios::end).tellg()));
	file.seekg(0, std::ios::beg).read(&buf_tmp[0], static_cast<std::streamsize>(buf_tmp.size()));
	file.close();

    buf = buf_tmp;
	buf_tmp.clear();
	pointer_of_buf = 0;
	size_of_buf = buf.size();

    Memory m;
    bool flag = false;

    while (pointer_of_buf < size_of_buf)
    {
        string str = "";
        while (buf[pointer_of_buf] != '\n')
        {
            if (buf[pointer_of_buf] == ' ')
            {
                pointer_of_buf++;
                continue;
            }
            if (buf[pointer_of_buf] == ':')
            {
                flag = true;
                break;
            }
            str += buf[pointer_of_buf];
            pointer_of_buf++;
        }
        if (flag)
        {
            m.mem_Name = str;
            m.mem_Paths.clear();
            str = "";
            flag = false;
            pointer_of_buf++;
            continue;
        }
        else
        {
           // m.mem_Path = str;
            //m.mem_Paths.push_back(str);
            
            auto it = memorys.find(m.mem_Name);
            if (it != memorys.end())
            {
                it->second.mem_Paths.push_back(str);
            }
            else
            {
                memorys.insert(pair<string,Memory>(m.mem_Name,m));
            }
            str = "";
            
            //m.mem_Path = "";
            pointer_of_buf++;
            continue;

        }
        
    }
    
}

void Parser::print()
{
    for (auto i : memorys)
    {
        cout << i.first << endl;
        for (auto j : i.second.mem_Paths)
        {
            cout << "Path: " << j << endl;
        }
        cout << endl;
    }
}