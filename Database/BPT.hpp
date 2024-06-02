#ifndef BPT_HPP
#define BPT_HPP

#include <fstream>
#include <iostream>
#include <vector>
#include <cstring>
#include <utility>
#include "exceptions.hpp"
#include "MemoryRiver.hpp"

template <typename valueType, int degree = 500>
class BPT
{
public:
    template <typename value>
    class KV
    {
    public:
        char first[24] = {'\0'};
        value second;

        KV() : second() {}
        KV(const KV &other)
        {
            std::strcpy(first, other.first);
            second = other.second;
        }
        KV &operator=(const KV &other)
        {
            if (this != &other)
            {
                std::strcpy(first, other.first);
                second = other.second;
            }
            return *this;
        }

        bool operator==(const KV &other) const // 键值都相等
        {
            return std::strcmp(first, other.first) == 0 && second == other.second;
        }
        bool operator!=(const KV &other) const
        {
            return !(*this == other);
        }
        bool operator<(const KV &other) const
        {
            if (std::strcmp(first, other.first) == 0)
            {
                return second < other.second;
            }
            return std::strcmp(first, other.first) < 0;
        }
        bool operator>(const KV &other) const
        {
            if (std::strcmp(first, other.first) == 0)
            {
                return second > other.second;
            }
            return std::strcmp(first, other.first) > 0;
        }
    };
    typedef KV<valueType> KV0; // 重命名

public:
    static const int Degree = degree; // 最大Degree
    static const int info = 3;
    static const int info_len = info * sizeof(int);
    const int SIZE = sizeof(Node);

    class Node
    {
    public:
        int pos;                     // 序列位置
        KV0 kvs[Degree + 1] = {};    // 键值对数组,最多只能存Degree个
        int leaves[Degree + 1] = {}; // 叶子节点数组(指针)，最多Degree + 1个
        bool leaf = true;            // 是否为叶子节点
        int size = 0;                // 当前节点大小
        int father = 0;              // 父节点
        int prev = 0;                // 前向node
        int next = 0;                // 后向node
        Node() = default;
        Node(const Node &other) : pos(other.pos), leaf(other.leaf), prev(other.prev), next(other.next), size(other.size), father(other.father)
        {
            for (int i = 0; i < Degree + 1; i++)
            {
                kvs[i] = other.kvs[i];
                leaves[i] = other.leaves[i];
            }
        }
        Node &operator=(const Node &other)
        {
            if (this != &other)
            {
                pos = other.pos;
                leaf = other.leaf;
                prev = other.prev;
                next = other.next;
                size = other.size;
                father = other.father;
                for (int i = 0; i < Degree + 1; i++)
                {
                    kvs[i] = other.kvs[i];
                    leaves[i] = other.leaves[i];
                }
            }
            return *this;
        }
    };
    Node root;
    MemoryRiver<Node, info> File; // info:总节点数，下一节点编号index, 根节点编号

    BPT() = default;

    void initialise(std::string file_name)
    {
        File.initialise(file_name);
    }

    BPT(std::string file_name)
    {
        File.initialise(file_name);
    }

    std::pair<int, int> Find_index(Node &, const KV0 &); // 查找插入键值对位置，返回节点位置与节点中键值对位置

    std::pair<int, int> Find_pos(Node &, const KV0 &, int mode); // 查找键值对位置，返回节点位置与节点中键值对位置, mode: 0无值查找，1有值删除

    std::vector<valueType> Find3(const KV0 &);

    std::vector<valueType> Find3(const std::string &key)
    {
        KV0 kv;
        std::strcpy(kv.first, key.c_str());
        return Find3(kv);
    }

    valueType Find2(const KV0 &);

    valueType Find2(const std::string &key)
    {
        KV0 kv;
        std::strcpy(kv.first, key.c_str());
        return Find2(kv);
    }

    bool Find1(const KV0 &); // 查找键值对是否存在

    bool Find1(const std::string &key)
    {
        KV0 kv;
        std::strcpy(kv.first, key.c_str());
        return Find1(kv);
    }

    void insert(std::string key, valueType value)
    {
        KV0 kv;
        std::strcpy(kv.first, key.c_str());
        kv.second = value;
        Insert(kv);
    }

    void Insert(const KV0 &);

    void adjust_insert(Node &, const KV0 &, int);

    void erase(std::string key, valueType value)
    {
        KV0 kv;
        std::strcpy(kv.first, key.c_str());
        kv.second = value;
        erase(kv);
    }

    void erase(const KV0 &);

    void adjust_erase_replace(Node &, const KV0 &, int);

    void adjust_erase_delete(Node &, const KV0 &, int);

    void printnode(const Node &node)
    {
        std::cout << "pos: " << node.pos << "  ";
        std::cout << "leaf: " << node.leaf << "  ";
        std::cout << "size: " << node.size << "  ";
        std::cout << "father: " << node.father << "  ";
        std::cout << "prev: " << node.prev << "  ";
        std::cout << "next: " << node.next << "\n";
        std::cout << "kvs: " << "\n";
        for (int i = 0; i < node.size + 1; i++)
        {
            std::cout << node.kvs[i].first << " " << node.kvs[i].second << "\n";
        }
        std::cout << "leaves: ";
        for (int i = 0; i <= node.size + 1; i++)
        {
            std::cout << node.leaves[i] << " ";
        }
        std::cout << "\n"
                  << "\n";
    }
    void print(int pos)
    {
        Node node;
        int total, index, idx_root;
        File.get_info(total, 1);
        File.get_info(index, 2);
        File.get_info(idx_root, 3);
        int info[3];
        File.get_all_info(info);
        std::cout << "total: " << info[0] << " ";
        std::cout << "index: " << info[1] << " ";
        std::cout << "idx_root: " << info[2] << "\n";
        File.read(node, info_len + (pos - 1) * SIZE);
        printnode(node);
    }
};

template <typename valueType, int degree>
std::pair<int, int> BPT<valueType, degree>::Find_index(Node &node, const KV0 &kv)
{
    int i = 0;
    while (i < node.size && kv > node.kvs[i])
    {
        ++i;
    }

    if (node.leaf) // 只找键
    {
        return {node.pos, i};
    }

    Node child;
    File.read(child, info_len + (node.leaves[i] - 1) * SIZE);
    return Find_index(child, kv);
}

template <typename valueType, int degree>
std::pair<int, int> BPT<valueType, degree>::Find_pos(Node &node, const KV0 &kv, int mode)
{
    // int total;
    // File.get_info(total, 1);
    // if (total == 0)
    // {
    //     return {0, 0}; // 空树
    // }
    int i = 0;
    while (i < node.size && kv > node.kvs[i]) // 用空值寻找到相应键值对前一个位置
    {
        ++i;
    }

    if (node.leaf) // 只找键
    {
        // std::cout << kv.first << " " << node.kvs[i].first << "\n";
        if (i == node.size || std::strcmp(kv.first, node.kvs[i].first) != 0)
        {
            if (node.next == 0)
            {
                return {-1, -1}; // 叶子节点未找到
            }
            Node tmp;
            File.read(tmp, info_len + (node.next - 1) * SIZE);
            if (std::strcmp(kv.first, tmp.kvs[0].first) < 0)
            {
                return {-1, -1}; // 叶子节点未找到
            }
            else
            {
                int k = 0;
                while (k < tmp.size && std::strcmp(kv.first, tmp.kvs[k].first) > 0)
                {
                    ++k;
                }
                if (kv.second == tmp.kvs[k].second || mode == 0)
                {
                    return {node.next, k};
                }
                return {-1, -1}; // 叶子节点未找到
            }
            // std::cout << kv.first << " " << node.kvs[i].first << "\n";
            // std::cout << (std::strcmp(kv.first, node.kvs[i].first) != 0) << "\n";
            return {-1, -1}; // 叶子节点未找到
        }
        else if (kv.second == node.kvs[i].second || mode == 0)
        {
            return {node.pos, i}; // 叶子节点找到
        }
    }

    Node child;
    File.read(child, info_len + (node.leaves[i] - 1) * SIZE);
    return Find_pos(child, kv, mode);
}

template <typename valueType, int degree>
std::vector<valueType> BPT<valueType, degree>::Find3(const KV0 &kv)
{
    std::vector<valueType> res;
    int idx_root;
    File.get_info(idx_root, 3);
    File.read(root, info_len + (idx_root - 1) * SIZE);
    std::pair<int, int> idx = Find_pos(root, kv, 0);
    if (idx == std::make_pair(0, 0) || idx == std::make_pair(-1, -1))
    {
        // std::cout << "null\n";
        // return;
        return res;
        // return 0;
    }

    // std::cout << idx.first << " " << idx.second << "\n";
    Node tmp;
    File.read(tmp, info_len + (idx.first - 1) * SIZE);
    // printnode(tmp);
    int pos = idx.second;
    bool flag = true;
    while (std::strcmp(tmp.kvs[pos].first, kv.first) == 0)
    {
        flag = false;
        // std::cout << tmp.kvs[pos].second << " " << pos << " " << tmp.kvs[pos].first << " " << kv.first << "\n";
        //std::cout << tmp.kvs[pos].second << " ";
        res.push_back(tmp.kvs[pos].second);
        // return tmp.kvs[pos].second;
        ++pos;
        if (tmp.next != 0 && pos == tmp.size)
        {
            File.read(tmp, info_len + (tmp.next - 1) * SIZE);
            pos = 0;
        }
    }
    if (flag)
    {
        // std::cout << "null\n";
        // return;
        return res;
    }
    //std::cout << "\n";
}

template <typename valueType, int degree>
valueType BPT<valueType, degree>::Find2(const KV0 &kv)
{
    int idx_root;
    File.get_info(idx_root, 3);
    File.read(root, info_len + (idx_root - 1) * SIZE);
    std::pair<int, int> idx = Find_pos(root, kv, 0);
    if (idx == std::make_pair(0, 0) || idx == std::make_pair(-1, -1))
    {
        // std::cout << "null\n";
        // return;
        return -1;
        // return 0;
    }

    // std::cout << idx.first << " " << idx.second << "\n";
    Node tmp;
    File.read(tmp, info_len + (idx.first - 1) * SIZE);
    // printnode(tmp);
    int pos = idx.second;
    bool flag = true;
    while (std::strcmp(tmp.kvs[pos].first, kv.first) == 0)
    {
        flag = false;
        // std::cout << tmp.kvs[pos].second << " " << pos << " " << tmp.kvs[pos].first << " " << kv.first << "\n";
        //std::cout << tmp.kvs[pos].second << " ";
        return tmp.kvs[pos].second;
        // return tmp.kvs[pos].second;
        ++pos;
        if (tmp.next != 0 && pos == tmp.size)
        {
            File.read(tmp, info_len + (tmp.next - 1) * SIZE);
            pos = 0;
        }
    }
    if (flag)
    {
        // std::cout << "null\n";
        // return;
        return -1;
    }
    return -1;
    //std::cout << "\n";
}

template <typename valueType, int degree>
bool BPT<valueType, degree>::Find1(const KV0 &kv)
{
    int idx_root;
    File.get_info(idx_root, 3);
    File.read(root, info_len + (idx_root - 1) * SIZE);
    std::pair<int, int> idx = Find_pos(root, kv, 0);
    if (idx == std::make_pair(0, 0) || idx == std::make_pair(-1, -1))
    {
        // std::cout << "null\n";
        // return;
        return false;
        // return 0;
    }

    // std::cout << idx.first << " " << idx.second << "\n";
    Node tmp;
    File.read(tmp, info_len + (idx.first - 1) * SIZE);
    // printnode(tmp);
    int pos = idx.second;
    bool flag = true;
    while (std::strcmp(tmp.kvs[pos].first, kv.first) == 0)
    {
        flag = false;
        // std::cout << tmp.kvs[pos].second << " " << pos << " " << tmp.kvs[pos].first << " " << kv.first << "\n";
        // std::cout << tmp.kvs[pos].second << " ";
        return true;
        // return tmp.kvs[pos].second;
        ++pos;
        if (tmp.next != 0 && pos == tmp.size)
        {
            File.read(tmp, info_len + (tmp.next - 1) * SIZE);
            pos = 0;
        }
    }
    if (flag)
    {
        // std::cout << "null\n";
        // return;
        return false;
    }
    return false;
}

template <typename valueType, int degree>
void BPT<valueType, degree>::Insert(const KV0 &kv)
{
    int info[info];
    File.get_all_info(info);
    if (info[0] == 0)
    {
        info[0] = 1;
        info[1] = 2;
        info[2] = 1;
        File.write_all_info(info);

        root.pos = 1;
        root.leaf = true;
        root.size = 1;
        root.kvs[0] = kv;
        File.write(root, info_len + (root.pos - 1) * SIZE);
        return;
    }

    Node cur_node;
    File.read(root, info_len + (info[2] - 1) * SIZE); // 读取root
    std::pair<int, int> idx = Find_index(root, kv);
    int index_node = idx.first;
    int index_kv = idx.second;

    // std::cout << index_node << " " << index_kv << "\n";

    File.read(cur_node, info_len + (index_node - 1) * SIZE);
    if (std::strcmp(kv.first, cur_node.kvs[index_kv].first) == 0 && kv.second == cur_node.kvs[index_kv].second)
    {
        return;
    }
    if (index_kv == cur_node.size && cur_node.next != 0) // 最后一个
    {
        Node next_node;
        File.read(next_node, info_len + (cur_node.next - 1) * SIZE);
        if (std::strcmp(kv.first, next_node.kvs[0].first) == 0 && kv.second == next_node.kvs[0].second)
        {
            return;
        }
    }

    if (cur_node.size < Degree)
    {
        for (int i = cur_node.size; i > index_kv; --i)
        {
            cur_node.kvs[i] = cur_node.kvs[i - 1];
        }
        cur_node.kvs[index_kv] = kv;
        ++cur_node.size;
        File.write(cur_node, info_len + (cur_node.pos - 1) * SIZE);
    }
    else
    {
        ++info[0];
        Node split_node;
        split_node.pos = info[1];
        ++info[1];

        for (int i = cur_node.size; i > index_kv; --i)
        {
            cur_node.kvs[i] = cur_node.kvs[i - 1];
        }
        cur_node.kvs[index_kv] = kv;
        ++cur_node.size;

        int i = (Degree + 1) / 2; // 再分裂
        KV0 tmp = cur_node.kvs[i];

        for (int j = i; j < cur_node.size; ++j) // 已插入
        {
            split_node.kvs[j - i] = cur_node.kvs[j];
        }
        split_node.size = cur_node.size - i;
        for (int j = i; j < cur_node.size; ++j)
        {
            cur_node.kvs[j] = KV0();
        }
        cur_node.size = i;

        if (cur_node.next != 0) // 其他参数调整
        {
            split_node.next = cur_node.next;
            Node next_node;
            File.read(next_node, info_len + (cur_node.next - 1) * SIZE);
            next_node.prev = split_node.pos;
            File.write(next_node, info_len + (next_node.pos - 1) * SIZE);
        }
        cur_node.next = split_node.pos;
        split_node.prev = cur_node.pos;
        if (cur_node.father == 0)
        {
            Node new_root;
            new_root.pos = info[1];
            ++info[0];
            ++info[1];
            info[2] = new_root.pos;
            new_root.leaf = false;
            new_root.leaves[0] = cur_node.pos;
            cur_node.father = new_root.pos;
            split_node.father = new_root.pos;
            File.write(new_root, info_len + (new_root.pos - 1) * SIZE);
        }
        else
        {
            split_node.father = cur_node.father;
        }

        File.write_all_info(info);
        File.write(cur_node, info_len + (cur_node.pos - 1) * SIZE);
        File.write(split_node, info_len + (split_node.pos - 1) * SIZE);

        Node father_node;
        File.read(father_node, info_len + (cur_node.father - 1) * SIZE);
        adjust_insert(father_node, tmp, split_node.pos);
    }
}

template <typename valueType, int degree>
void BPT<valueType, degree>::adjust_insert(Node &node, const KV0 &kv, int split_pos)
{
    int k = 0;
    while (k < node.size && kv > node.kvs[k])
    {
        ++k;
    }
    for (int j = node.size; j > k; --j)
    {
        node.kvs[j] = node.kvs[j - 1];
        node.leaves[j + 1] = node.leaves[j];
    }
    node.kvs[k] = kv;
    node.leaves[k + 1] = split_pos;
    ++node.size;

    if (node.size <= Degree)
    {
        File.write(node, info_len + (node.pos - 1) * SIZE);
    }
    else
    {
        int info[info];
        File.get_all_info(info);
        ++info[0];
        Node split_node;
        split_node.pos = info[1];
        split_node.leaf = false;
        ++info[1];

        int i = (Degree + 1) / 2;
        KV0 tmp = node.kvs[i];

        for (int j = i; j < node.size - 1; ++j) // i位置向上移
        {
            split_node.kvs[j - i] = node.kvs[j + 1];
        }
        split_node.size = node.size - i - 1;
        for (int j = i; j < node.size; ++j)
        {
            split_node.leaves[j - i] = node.leaves[j + 1];
        }
        for (int j = i; j < node.size; ++j)
        {
            node.kvs[j] = KV0();
            node.leaves[j + 1] = 0;
        }
        node.size = i;
        for (int j = 0; j <= split_node.size; ++j)
        {
            Node tmp_node;
            File.read(tmp_node, info_len + (split_node.leaves[j] - 1) * SIZE);
            tmp_node.father = split_node.pos;
            File.write(tmp_node, info_len + (split_node.leaves[j] - 1) * SIZE);
        }
        // if (node.next != 0) // 其他参数调整
        // {
        //     split_node.next = node.next;
        // }
        // node.next = split_node.pos;
        // split_node.prev = node.pos;

        if (node.father == 0)
        {
            Node new_root;
            new_root.pos = info[1];
            ++info[0];
            ++info[1];
            info[2] = new_root.pos;

            new_root.leaf = false;
            new_root.leaves[0] = node.pos;
            node.father = new_root.pos;
            split_node.father = new_root.pos;
            File.write(new_root, info_len + (new_root.pos - 1) * SIZE);
        }
        else
        {
            split_node.father = node.father;
        }

        File.write_all_info(info);
        File.write(node, info_len + (node.pos - 1) * SIZE);
        File.write(split_node, info_len + (split_node.pos - 1) * SIZE);

        Node father_node;
        File.read(father_node, info_len + (node.father - 1) * SIZE);
        adjust_insert(father_node, tmp, split_node.pos);
    }
}

template <typename valueType, int degree>
void BPT<valueType, degree>::erase(const KV0 &kv)
{
    int info[info];
    File.get_all_info(info);
    if (info[0] == 0)
    {
        return;
    }

    File.read(root, info_len + (info[2] - 1) * SIZE); // 读取root
    std::pair<int, int> idx = Find_pos(root, kv, 1);  // 删除键值对位置
    int index_node = idx.first;
    int index_kv = idx.second;
    // std::cout << index_node << " " << index_kv << "\n";
    if (index_node == 0 || index_node == -1)
    {
        return;
    }

    Node cur_node;
    File.read(cur_node, info_len + (index_node - 1) * SIZE);

    for (int i = index_kv; i < cur_node.size; ++i)
    {
        cur_node.kvs[i] = cur_node.kvs[i + 1];
    }
    --cur_node.size; // 删除完成

    if (cur_node.size >= (Degree - 1) / 2 + 1)
    {
        File.write(cur_node, info_len + (cur_node.pos - 1) * SIZE);
        return;
    }
    else // 小于一半
    {
        if (cur_node.next != 0) // 先向后借键值对
        {
            Node next_node;
            File.read(next_node, info_len + (cur_node.next - 1) * SIZE);
            if (next_node.father == cur_node.father) // 是兄弟
            {                                        // 向后借一个键值对
                Node father_node;
                File.read(father_node, info_len + (next_node.father - 1) * SIZE);
                if (next_node.size > (Degree - 1) / 2 + 1) // 向后可以借
                {
                    cur_node.kvs[cur_node.size] = next_node.kvs[0];
                    for (int i = 0; i < next_node.size - 1; ++i)
                    {
                        next_node.kvs[i] = next_node.kvs[i + 1];
                    }
                    next_node.kvs[next_node.size - 1] = KV0();
                    --next_node.size;
                    ++cur_node.size;
                    KV0 tmp = next_node.kvs[0]; // 替换后的父节点索引
                    File.write(cur_node, info_len + (cur_node.pos - 1) * SIZE);
                    File.write(next_node, info_len + (next_node.pos - 1) * SIZE);

                    adjust_erase_replace(father_node, tmp, cur_node.pos);
                }
                else // 向后借不了，并块合并,丢掉next_node
                {
                    // std::cout << " 1";

                    KV0 tmp = next_node.kvs[0]; // 删除父节点的该索引
                    for (int i = 0; i < next_node.size; ++i)
                    {
                        cur_node.kvs[cur_node.size + i] = next_node.kvs[i];
                    }

                    // for (int i = next_node.size + cur_node.size - 1; i > 0; --i)
                    // {
                    //     next_node.kvs[i] = next_node.kvs[i - cur_node.size];
                    // }
                    // for (int i = 0; i < cur_node.size; ++i)
                    // {
                    //     next_node.kvs[i] = cur_node.kvs[i];
                    // }
                    // next_node.size += cur_node.size;
                    // next_node.prev = cur_node.prev;
                    // --info[0];
                    cur_node.size += next_node.size;
                    cur_node.next = next_node.next;
                    if (next_node.next != 0)
                    {
                        Node next_next_node;
                        File.read(next_next_node, info_len + (next_node.next - 1) * SIZE);
                        next_next_node.prev = cur_node.pos;
                        File.write(next_next_node, info_len + (next_next_node.pos - 1) * SIZE);
                    }
                    File.write(cur_node, info_len + (cur_node.pos - 1) * SIZE);

                    adjust_erase_delete(father_node, tmp, cur_node.pos);
                }
                // File.write_all_info(info);
                return;
            }
            else // 不是兄弟，向前检查,肯定能借或并
            {
                // if (cur_node.prev != 0 && prev_node.father == cur_node.father) // 应该这时候前兄弟节点必存在

                Node prev_node;
                File.read(prev_node, info_len + (cur_node.prev - 1) * SIZE);

                Node father_node;
                File.read(father_node, info_len + (cur_node.father - 1) * SIZE);

                if (prev_node.size > (Degree - 1) / 2 + 1) // 向前可以借
                {                                          // 向前借一个键值对
                    for (int i = cur_node.size; i > 0; --i)
                    {
                        cur_node.kvs[i] = cur_node.kvs[i - 1];
                    }
                    cur_node.kvs[0] = prev_node.kvs[prev_node.size - 1];
                    prev_node.kvs[prev_node.size - 1] = KV0();
                    --prev_node.size;
                    ++cur_node.size;
                    KV0 tmp = cur_node.kvs[0]; // 替换后的父节点索引
                    File.write(cur_node, info_len + (cur_node.pos - 1) * SIZE);
                    File.write(prev_node, info_len + (prev_node.pos - 1) * SIZE);

                    adjust_erase_replace(father_node, tmp, prev_node.pos);
                }
                else // 向前借不了，并块合并，丢掉cur_node
                {
                    // std::cout << " 2";

                    for (int i = 0; i < cur_node.size; ++i)
                    {
                        prev_node.kvs[prev_node.size + i] = cur_node.kvs[i];
                    }
                    prev_node.size += cur_node.size;
                    prev_node.next = cur_node.next;
                    KV0 tmp = cur_node.kvs[0]; // 删除父节点的该索引
                    --info[0];
                    // if (cur_node.next != 0)
                    // {
                    //     Node next_node;
                    //     File.read(next_node, info_len + (cur_node.next - 1) * SIZE);
                    //     next_node.prev = prev_node.pos;
                    //     File.write(next_node, info_len + (next_node.pos - 1) * SIZE);
                    // }
                    File.write(prev_node, info_len + (prev_node.pos - 1) * SIZE);

                    adjust_erase_delete(father_node, tmp, prev_node.pos);
                }

                // File.write_all_info(info);
                return;
            }
        }
        else // 处于末尾，向前检查，必然能借或并
        {
            // if (cur_node.prev != 0 && prev_node.father == cur_node.father) // 应该这时候前兄弟节点必存在
            // printnode(cur_node);
            if (cur_node.prev != 0)
            {
                Node prev_node;
                File.read(prev_node, info_len + (cur_node.prev - 1) * SIZE);

                Node father_node;
                File.read(father_node, info_len + (cur_node.father - 1) * SIZE);

                if (prev_node.size > (Degree - 1) / 2 + 1) // 向前可以借
                {                                          // 向前借一个键值对
                    for (int i = cur_node.size; i > 0; --i)
                    {
                        cur_node.kvs[i] = cur_node.kvs[i - 1];
                    }
                    cur_node.kvs[0] = prev_node.kvs[prev_node.size - 1];
                    prev_node.kvs[prev_node.size - 1] = KV0();
                    --prev_node.size;
                    ++cur_node.size;
                    KV0 tmp = cur_node.kvs[0]; // 替换后的父节点索引
                    File.write(cur_node, info_len + (cur_node.pos - 1) * SIZE);
                    File.write(prev_node, info_len + (prev_node.pos - 1) * SIZE);

                    adjust_erase_replace(father_node, tmp, prev_node.pos);
                }
                else // 向前借不了，并块合并,丢掉cur_node
                {
                    // std::cout << " 3\n";
                    // printnode(prev_node);

                    for (int i = 0; i < cur_node.size; ++i)
                    {
                        prev_node.kvs[prev_node.size + i] = cur_node.kvs[i];
                    }
                    prev_node.size += cur_node.size;
                    prev_node.next = cur_node.next;
                    KV0 tmp = cur_node.kvs[0]; // 删除父节点的该索引
                    //--info[0];
                    // if (cur_node.next != 0)
                    // {
                    //     Node next_node;
                    //     File.read(next_node, info_len + (cur_node.next - 1) * SIZE);
                    //     next_node.prev = prev_node.pos;
                    //     File.write(next_node, info_len + (next_node.pos - 1) * SIZE);
                    // }
                    File.write(prev_node, info_len + (prev_node.pos - 1) * SIZE);

                    adjust_erase_delete(father_node, tmp, prev_node.pos); // 似乎tmp没啥用？应该用父节点的索引
                }
                // File.write_all_info(info);
                return;
            }
            else // 根节点
            {
                File.write(cur_node, info_len + (cur_node.pos - 1) * SIZE);
                return;
            }
        }
    }
}

template <typename valueType, int degree>
void BPT<valueType, degree>::adjust_erase_replace(Node &node, const KV0 &kv, int prev_pos) // 第三个参数传前面的节点位置
{
    int k = 0;
    while (k < node.size && node.leaves[k] != prev_pos)
    {
        ++k;
    }
    node.kvs[k] = kv;
    File.write(node, info_len + (node.pos - 1) * SIZE);
}

template <typename valueType, int degree>
void BPT<valueType, degree>::adjust_erase_delete(Node &node, const KV0 &kv, int prev_pos) // 第三个参数传前面的节点位置
{
    int info[info];
    File.get_all_info(info);
    // std::cout << info[0] << " " << info[1] << " " << info[2] << " " << node.size << " " << prev_pos << "\n";

    int k = 0; // 找到需要更改的索引位置
    while (k < node.size && node.leaves[k] != prev_pos)
    {
        ++k;
    }
    for (int i = k; i < node.size - 1; ++i) // 保留prev
    {
        node.kvs[i] = node.kvs[i + 1];
        node.leaves[i + 1] = node.leaves[i + 2];
    }
    node.kvs[node.size - 1] = KV0();
    node.leaves[node.size] = 0;
    --node.size;

    // std::cout << node.size << " ";

    if (node.size >= (Degree - 1) / 2 + 1)
    {
        File.write(node, info_len + (node.pos - 1) * SIZE);
        return;
    }
    else // 需要借或者并
    {
        if (node.father == 0) // 根节点
        {
            // std::cout << " 4d";
            if (node.size == 0)
            {
                // std::cout << " 5l";
                //--info[0];
                // File.write_all_info(info);
                File.write_info(prev_pos, 3);
                Node prev_node;
                File.read(prev_node, info_len + (prev_pos - 1) * SIZE);
                prev_node.father = 0;
                File.write(prev_node, info_len + (prev_pos - 1) * SIZE);
                return;
            }
            File.write(node, info_len + (node.pos - 1) * SIZE);
            return;
        }

        Node father_node;
        File.read(father_node, info_len + (node.father - 1) * SIZE);
        int i = 0;
        while (i < father_node.size && father_node.leaves[i] != node.pos)
        {
            ++i;
        }

        if (father_node.leaves[i + 1] != 0) // 向后查询
        {
            Node next_node;
            File.read(next_node, info_len + (father_node.leaves[i + 1] - 1) * SIZE);
            if (next_node.size > (Degree - 1) / 2 + 1) // 向后可以借
            {
                node.kvs[node.size] = father_node.kvs[i];
                father_node.kvs[i] = next_node.kvs[0];
                for (int j = 0; j < next_node.size - 1; ++j)
                {
                    next_node.kvs[j] = next_node.kvs[j + 1];
                    // next_node.leaves[j] = next_node.leaves[j + 1];
                }
                for (int j = 0; j < next_node.size; ++j)
                {
                    next_node.leaves[j] = next_node.leaves[j + 1];
                }
                next_node.leaves[next_node.size] = 0;
                next_node.kvs[next_node.size - 1] = KV0();
                --next_node.size;
                ++node.size;
                File.write(node, info_len + (node.pos - 1) * SIZE);
                File.write(next_node, info_len + (next_node.pos - 1) * SIZE);
                File.write(father_node, info_len + (father_node.pos - 1) * SIZE);
                return;
            }
            else // 向后借不了，并块合并， 丢弃next_node
            {
                KV0 tmp = father_node.kvs[i];
                node.kvs[node.size] = father_node.kvs[i];
                //--info[0];
                for (int j = 0; j < next_node.size; ++j)
                {
                    node.kvs[node.size + j] = next_node.kvs[j];
                    node.leaves[node.size + j + 1] = next_node.leaves[j];
                }
                node.leaves[node.size + next_node.size + 1] = next_node.leaves[next_node.size];
                node.size += (next_node.size + 1);

                File.write(node, info_len + (node.pos - 1) * SIZE);
                adjust_erase_delete(father_node, tmp, node.pos);
            }
        }
        else // 向前查询
        {
            Node prev_node;
            File.read(prev_node, info_len + (father_node.leaves[i - 1] - 1) * SIZE);
            if (prev_node.size > (Degree - 1) / 2 + 1) // 向前可以借
            {
                for (int j = node.size; j > 0; --j)
                {
                    node.kvs[j] = node.kvs[j - 1];
                    node.leaves[j + 1] = node.leaves[j];
                }
                node.kvs[0] = father_node.kvs[i - 1];
                father_node.kvs[i - 1] = prev_node.kvs[prev_node.size - 1];
                prev_node.kvs[prev_node.size - 1] = KV0();
                node.leaves[0] = prev_node.leaves[prev_node.size];
                --prev_node.size;
                ++node.size;

                File.write(node, info_len + (node.pos - 1) * SIZE);
                File.write(prev_node, info_len + (prev_node.pos - 1) * SIZE);
                File.write(father_node, info_len + (father_node.pos - 1) * SIZE);
                return;
            }
            else // 向前借不了，并块合并, 丢弃cur_node
            {
                KV0 tmp = father_node.kvs[i - 1];
                prev_node.kvs[prev_node.size] = father_node.kvs[i - 1];
                for (int j = 0; j < node.size; ++j)
                {
                    prev_node.kvs[prev_node.size + j + 1] = node.kvs[j];
                    prev_node.leaves[prev_node.size + j + 1] = node.leaves[j];
                }
                prev_node.leaves[prev_node.size + node.size + 1] = node.leaves[node.size];
                prev_node.size += (node.size + 1);
                //--info[0];
                File.write(prev_node, info_len + (prev_node.pos - 1) * SIZE);
                adjust_erase_delete(father_node, tmp, prev_node.pos);
            }
        }
        return;
    }
}

#endif