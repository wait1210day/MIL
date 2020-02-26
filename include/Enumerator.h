#ifndef __ENUMERATOR_H__
#define __ENUMERATOR_H__

#include <vector>
#include <iostream>
#include <string>
#include <cstring>
#include <sstream>

/* Enumerator 模板类可以枚举任意 struct 向量表 */

/*  */

typedef off64_t StructElement;

template <typename _T>
class VectorEnumerator
{
public:
    VectorEnumerator(std::vector<_T>& vecObj);
    VectorEnumerator(std::vector<_T *>& vecObj);
    ~VectorEnumerator();

    template <typename _kT>
    void newEnumeratable(char const *name, StructElement offset);
    void clear();
    void accept(size_t spaces = 3);
    // void setf(const std::ios_base::fmtflags flag);
    // void precision(unsigned int n);

private:
    void setColSize(size_t spaces);

private:
    std::vector<void *> vec;
    std::vector<char const *> hdr;
    std::vector< std::vector<std::string> > result;
    std::vector<size_t> colSize;

    std::ios_base::fmtflags ssflags;
    unsigned int ssprecisions;
};

template <typename _T>
VectorEnumerator<_T>::VectorEnumerator(std::vector<_T>& vecObj)
{
    for (_T& ele : vecObj)
        vec.push_back(&ele);
}

template <typename _T>
VectorEnumerator<_T>::VectorEnumerator(std::vector<_T *>& vecObj)
{
    for (_T *& ele : vecObj)
        vec.push_back(ele);
}

template <typename _T>
VectorEnumerator<_T>::~VectorEnumerator()
{}

template <typename _T>
template <typename _kT>
void VectorEnumerator<_T>::newEnumeratable(char const *name, StructElement offset)
{
    hdr.push_back(name);
    std::vector<std::string> element;

    for (void *& ptr : vec)
    {
        std::ostringstream ss;
        ss.setf(std::ios_base::boolalpha);
        ss << *((_kT *)((uint64_t)ptr + offset));
        element.push_back(ss.str());
    }
    result.push_back(element);
}

template <typename _T>
void VectorEnumerator<_T>::clear()
{
    hdr.clear();
    result.clear();
    colSize.clear();
}

template <typename _T>
void VectorEnumerator<_T>::setColSize(size_t spaces)
{
    size_t max;
    for (int i = 0; i < result.size(); i++)
    {
        max = 0;
        for (std::string& str : result[i])
            if (str.length() > max)
                max = str.length();
        if (strlen(hdr[i]) > max)
            max = strlen(hdr[i]);
        max += spaces;
        colSize.push_back(max);
    }
}

template <typename _T>
void VectorEnumerator<_T>::accept(size_t spaces)
{
    setColSize(spaces);
    std::string line;

    /* 首先打印一遍表头 */
    for (int i = 0; i < hdr.size(); i++)
    {
        line.append(hdr[i]);
        for (int j = 0; j < colSize[i] - strlen(hdr[i]); j++)
            line.push_back(' ');
    }
    std::cout << line << std::endl;

    /* 表头分隔符 */
    size_t lineSize = line.length();
    line.clear();
    for (int i = 0; i < lineSize; i++)
        line.push_back('-');
    std::cout << line << std::endl;
    line.clear();

    /* 正片开始 */
    for (int i = 0; i < vec.size(); i++)
    {
        for (int j = 0; j < result.size(); j++)
        {
            line.append(result[j][i]);
            for (int q = 0; q < colSize[j] - result[j][i].length(); q++)
                line.push_back(' ');
        }
        std::cout << line << std::endl;
        line.clear();
    }
}


#endif /* __ENUMERATOR_H__ */
