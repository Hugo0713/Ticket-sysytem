#ifndef BPT_MEMORYRIVER_HPP
#define BPT_MEMORYRIVER_HPP

#include <fstream>
#include <unistd.h>

using std::fstream;
using std::ifstream;
using std::ofstream;
using std::string;

template <class T, int info_len>
class MemoryRiver
{
private:
    fstream file;
    string file_name;
    int sizeofT = sizeof(T);

public:
    MemoryRiver() = default;

    MemoryRiver(const string &file_name) : file_name(file_name) {}

    void initialise(string FN = "")
    {
        if (FN != "")
        {
            file_name = FN;
        }
        if (access(file_name.c_str(), F_OK) == 0)
        {
            return;
        }
        file.open(file_name, std::ios::out);
        int tmp = 0;
        for (int i = 0; i < info_len; ++i)
        {
            file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
        }
        if (file.tellg() == -1)
        {
            file.close();
        }
    }

    void get_info(int &tmp, int n)
    {
        if (n > info_len)
        {
            return;
        }
        if (!file.is_open())
        {
            file.open(file_name);
        }
        
        file.seekg((n - 1) * sizeof(int), std::ios::beg);
        file.read(reinterpret_cast<char *>(&tmp), sizeof(int));
        if (file.tellg() == -1)
        {
            file.close();
        }
        return;
    }

    void get_all_info(int *tmp)
    {
        if (!file.is_open())
        {
            file.open(file_name);
        }
        file.seekg(0, std::ios::beg);
        file.read(reinterpret_cast<char *>(tmp), sizeof(int) * info_len);
        if (file.tellg() == -1)
        {
            file.close();
        }
        return;
    }

    void write_info(int tmp, int n)
    {
        if (n > info_len)
        {
            return;
        }
        if (!file.is_open())
        {
            file.open(file_name);
        }
        file.seekp((n - 1) * sizeof(int), std::ios::beg);
        file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
        if (file.tellg() == -1)
        {
            file.close();
        }
        return;
    }

    void write_all_info(int *tmp)
    {
        if (!file.is_open())
        {
            file.open(file_name);
        }
        file.seekp(0, std::ios::beg);
        file.write(reinterpret_cast<char *>(tmp), sizeof(int) * info_len);
        if (file.tellg() == -1)
        {
            file.close();
        }
        return;
    }

    void write(T &t, long long place, int size = 1)
    {
        if (!file.is_open())
        {
            file.open(file_name);
        }
        file.seekp(place);
        file.write(reinterpret_cast<char *>(&t), sizeofT * size);
        if (file.tellg() == -1)
        {
            file.close();
        }
    }

    void read(T &t, const int index, int size = 1)
    {
        if (!file.is_open())
        {
            file.open(file_name);
        }
        long long position = index;
        file.seekg(position, std::ios::beg);
        file.read(reinterpret_cast<char *>(&t), sizeofT * size);
        if (file.tellg() == -1)
        {
            file.close();
        }
        //file.close();
    }
};

#endif // BPT_MEMORYRIVER_HPP