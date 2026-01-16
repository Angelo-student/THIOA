#ifndef __INDI__
#include "indi.h"  // 包含头文件定义
#endif

// TIndi 类的定义
class TIndi {
public:
    TIndi(); // 构造函数
    ~TIndi(); // 析构函数
    void Define(int N); // 定义结构体大小的函数
    TIndi& operator=(const TIndi& src); // 赋值操作符重载
    bool operator==(const TIndi& src); // 相等操作符重载

private:
    int fN; // 结构体中的元素数量
    int** fLink; // 指向指针数组的指针，用于链接信息
    int** fOrder; // 另一个指向指针数组的指针，用于顺序信息
    int fEvaluationValue; // 评价值
};

// 构造函数，初始化成员变量
TIndi::TIndi()
{
    fN = 0; // 初始化元素数量为0
    fLink = NULL; // 初始化链接指针为NULL
    fOrder = NULL; // 初始化顺序指针为NULL
    fEvaluationValue = 0; // 初始化评价值为0
}

// 析构函数，释放动态分配的内存
TIndi::~TIndi()
{
    for (int i = 0; i < fN; ++i)
        delete[] fLink[i]; // 释放每个链接数组
    delete[] fLink; // 释放链接指针数组

    for (int i = 0; i < fN; ++i)  // Large
        delete[] fOrder[i]; // 释放每个顺序数组
    delete[] fOrder; // 释放顺序指针数组
}

// 定义结构体的大小，并分配内存
void TIndi::Define(int N)
{
    fN = N; // 设置元素数量

    fLink = new int* [fN]; // 分配指针数组
    for (int i = 0; i < fN; ++i)
        fLink[i] = new int[2]; // 为每个指针分配大小为2的整型数组

    fOrder = new int* [fN]; // 分配顺序指针数组
    for (int i = 0; i < fN; ++i)
        fOrder[i] = new int[2]; // 为每个顺序指针分配大小为2的整型数组
}

// 赋值操作符重载
TIndi& TIndi::operator=(const TIndi& src)
{
    fN = src.fN; // 复制元素数量

    // 复制链接信息
    for (int i = 0; i < fN; ++i) {
        for (int j = 0; j < 2; ++j) {
            fLink[i][j] = src.fLink[i][j]; // 复制链接数组中的数据
            fOrder[i][j] = src.fOrder[i][j];  // Large 复制顺序数组中的数据
        }
    }
    fEvaluationValue = src.fEvaluationValue; // 复制评价值

    return *this; // 返回当前对象的引用
}

// 相等操作符重载
bool TIndi::operator==(const TIndi& src)
{
    int curr, next, pre;
    int flag_identify;
    if (fN != src.fN)
        return false; // 元素数量不同，返回false
    if (fEvaluationValue != src.fEvaluationValue)
        return false; // 评价值不同，返回false
    curr = 0; // 初始化当前索引
    pre = -1; // 初始化前一个索引
    // 检查链接数组是否相同
    for (int i = 0; i < fN; ++i)
    {
        // 确定下一个节点
        if (fLink[curr][0] == pre)
            next = fLink[curr][1]; // 如果当前链接的第一个元素等于前一个元素，则取第二个元素
        else
            next = fLink[curr][0]; // 否则取第一个元素
        // 检查源对象中的链接是否包含当前的下一个元素
        if (src.fLink[curr][0] != next && src.fLink[curr][1] != next)
        {
            return false; // 不相等，返回false
        }
        pre = curr; // 更新前一个索引
        curr = next; // 更新当前索引
    }
    return true; // 如果所有检查通过，返回true
}
