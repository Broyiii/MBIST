#include "parser.h"
#include <algorithm>
#include <iomanip>
#include <math.h>
#include <fstream>
#include <sstream>

using namespace std;

void Parser::ParserMemList()
{
    std::string filename = this->memorylist_file;
    std::ifstream file(filename, std::ios::binary);
	std::vector<char> buf_tmp(static_cast<unsigned int>(file.seekg(0, std::ios::end).tellg()));
	file.seekg(0, std::ios::beg).read(&buf_tmp[0], static_cast<std::streamsize>(buf_tmp.size()));
	file.close();

    buf = buf_tmp;
	buf_tmp.clear();
	ptr_of_buf = 0;
	size_of_buf = buf.size();

    Memory m;
    bool flag = false;

    while (ptr_of_buf < size_of_buf)
    {
        string str = "";
        while (buf[ptr_of_buf] != '\n')
        {
            if (buf[ptr_of_buf] == ' ')
            {
                ptr_of_buf++;
                continue;
            }
            if (buf[ptr_of_buf] == ':')
            {
                flag = true;
                break;
            }
            str += buf[ptr_of_buf];
            ptr_of_buf++;
        }
        if (flag)
        {
            m.mem_Name = str;
            //m.mem_Paths.clear();
            str = "";
            flag = false;
            ptr_of_buf++;
            continue;
        }
        else
        {
            m.mem_Path = str;

            if (str.length() != 0)
            {
                auto it = memorys.find(m.mem_Name);
            
                if (it != memorys.end())
                {
                    //it->second.mem_Paths.push_back(str);
                    it->second.insert(pair<string,Memory>(str,m));
                }
                else
                {
                    //memorys.insert(pair<string,Memory>(m.mem_Name,m));
                    map<string,Memory> tmp;
                    tmp.insert(pair<string,Memory>(str,m));
                    memorys.insert(pair<string,map<string,Memory>>(m.mem_Name,tmp));
                }
            }
            str = "";
            
            
            ptr_of_buf++;
            continue;

        }
        
    }
    buf.clear();
    
}



void Parser::ParserDef()
{
    std::string filename = this->def_file;
    std::ifstream file(filename, std::ios::binary);
	std::vector<char> buf_tmp(static_cast<unsigned int>(file.seekg(0, std::ios::end).tellg()));
	file.seekg(0, std::ios::beg).read(&buf_tmp[0], static_cast<std::streamsize>(buf_tmp.size()));
	file.close();

    buf = buf_tmp;
	buf_tmp.clear();
	ptr_of_buf = 0;
	size_of_buf = buf.size();

    while (ptr_of_buf < size_of_buf)
    {
        if (buf[ptr_of_buf] != '-') // skip <File Partial>
        {
            ptr_of_buf++;
            continue;
        }
        else
        {
            ptr_of_buf += 2;
            string path_ = "";
            string memory_name = "";
            while (buf[ptr_of_buf] != ' ')
            {
                path_ += buf[ptr_of_buf];
                ptr_of_buf++;
            }
            ptr_of_buf++;
            while (buf[ptr_of_buf] != ' ')
            {
                memory_name += buf[ptr_of_buf];
                ptr_of_buf++;
            }
            while (buf[ptr_of_buf] != '(')
            {
                ptr_of_buf++;
                continue;
            } 
            ptr_of_buf++;
            string low = "";
            string up = "";
            while (buf[ptr_of_buf] != ' ')
            {
                low += buf[ptr_of_buf];
                ptr_of_buf++;
            }
            ptr_of_buf++;
            while (buf[ptr_of_buf] != ')')
            {
                up += buf[ptr_of_buf];
                ptr_of_buf++;
            }
            int Low = atoi(low.c_str());
            int Up = atoi(up.c_str());
            low = "";
            up = "";
            auto it_name = memorys.find(memory_name);
            if (it_name != memorys.end())
            {
                auto it_name_path = it_name->second.find(path_);
                if (it_name_path != it_name->second.end())
                {
                    it_name_path->second.low_bound = Low;
                    it_name_path->second.up_bound = Up;
                }
                else
                {
                    cout << "Not path_: " << path_ << " of" << memory_name << endl;
                }
            }
            else
            {
                cout << "Not name: " << memory_name << endl;
            }
            

        }   
    }
    buf.clear();
}

void Parser::Print()
{
    for (auto i : memorys)
    {
        cout << i.first << endl;
        for (auto j : i.second)
        {
            cout << "Path: " << j.first << " Low_limit: " << j.second.low_bound << " Up_limit: " << j.second.up_bound << endl;
        }
        cout << endl;
    }
    cout << "-----------------------------------------------" << endl;
    for (auto i : Memorys)
    {
        cout << "Name: " << i.mem_Name << " Path: " << i.mem_Path << " UP: " << i.up_bound << " LOW: " << i.low_bound << endl;
    }
}

void Parser::GetAllMemory()
{
    for (auto i : memorys)
    {
        for (auto j : i.second)
        {
            Memorys.push_back(j.second);
        }
    }
}

void Parser::GetInformationFromFile()
{
    // parser file 
    ParserMemList();
    ParserDef();



    // Show Information
    GetAllMemory();
    Print();

}
