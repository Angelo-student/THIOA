#ifndef __Sort__  // 如果没有定义 __Sort__
#include "sort.h" // 包含 sort.h 头文件
#endif

// 初始化 TSort 指针为 NULL
TSort* tSort = NULL;

// 初始化排序的函数
void InitSort(void)
{
    tSort = new TSort(); // 动态创建 TSort 对象
}

// TSort 构造函数
TSort::TSort()
{
}

// TSort 析构函数
TSort::~TSort()
{
}

// 基于输入的 double 类型数组对索引进行排序
void TSort::Index(double* Arg, int numOfArg, int* indexOrderd, int numOfOrd)
{
    int indexBest = 0; // 记录当前找到的最佳索引
    double valueBest;  // 记录当前最佳值
    int* checked; // 检查数组，记录哪些元素已经被处理
    checked = new int[numOfArg]; // 为检查数组分配内存

    // 确保 Arg 的第一个元素小于一个大值
    assert(Arg[0] < 99999999999.9);

    // 初始化 checked 数组，所有元素设为 0
    for (int i = 0; i < numOfArg; ++i)
        checked[i] = 0;

    // 遍历 numOfOrd 次，找到每次的最佳值
    for (int i = 0; i < numOfOrd; ++i)
    {
        valueBest = 99999999999.9; // 设置当前最佳值为一个极大值
        for (int j = 0; j < numOfArg; ++j)
        {
            // 查找尚未处理的最小值
            if ((Arg[j] < valueBest) && checked[j] == 0) {
                valueBest = Arg[j]; // 更新最佳值
                indexBest = j; // 记录最佳索引
            }
        }
        indexOrderd[i] = indexBest; // 更新输出索引数组
        checked[indexBest] = 1; // 标记该索引已处理
    }

    delete[] checked; // 释放检查数组的内存
}

// 基于输入的 double 类型数组对索引进行反向排序
void TSort::Index_B(double* Arg, int numOfArg, int* indexOrderd, int numOfOrd)
{
    int indexBest = 0;
    double valueBest; // 记录当前最佳值
    int* checked; // 检查数组
    checked = new int[numOfArg]; // 为检查数组分配内存

    // 确保 Arg 的第一个元素大于一个小值
    assert(Arg[0] > -99999999999.9);

    // 初始化 checked 数组
    for (int i = 0; i < numOfArg; ++i)
        checked[i] = 0;

    // 遍历 numOfOrd 次，找到每次的最佳值
    for (int i = 0; i < numOfOrd; ++i)
    {
        valueBest = -99999999999.9; // 设置当前最佳值为一个极小值
        for (int j = 0; j < numOfArg; ++j)
        {
            // 查找尚未处理的最大值
            if ((Arg[j] > valueBest) && checked[j] == 0) {
                valueBest = Arg[j]; // 更新最佳值
                indexBest = j; // 记录最佳索引
            }
        }
        indexOrderd[i] = indexBest; // 更新输出索引数组
        checked[indexBest] = 1; // 标记该索引已处理
    }

    delete[] checked; // 释放检查数组的内存
}

// 基于输入的 int 类型数组对索引进行排序
void TSort::Index(int* Arg, int numOfArg, int* indexOrderd, int numOfOrd)
{
    int indexBest = 0;
    int valueBest; // 记录当前最佳值
    int* checked; // 检查数组
    checked = new int[numOfArg]; // 为检查数组分配内存

    // 确保 Arg 的第一个元素小于一个大值
    assert(Arg[0] < 99999999);

    // 初始化 checked 数组
    for (int i = 0; i < numOfArg; ++i)
        checked[i] = 0;

    // 遍历 numOfOrd 次，找到每次的最佳值
    for (int i = 0; i < numOfOrd; ++i)
    {
        valueBest = 99999999; // 设置当前最佳值为一个极大值
        for (int j = 0; j < numOfArg; ++j)
        {
            // 查找尚未处理的最小值
            if ((Arg[j] < valueBest) && checked[j] == 0) {
                valueBest = Arg[j]; // 更新最佳值
                indexBest = j; // 记录最佳索引
            }
        }
        indexOrderd[i] = indexBest; // 更新输出索引数组
        checked[indexBest] = 1; // 标记该索引已处理
    }

    delete[] checked; // 释放检查数组的内存
}

// 基于输入的 int 类型数组对索引进行反向排序
void TSort::Index_B(int* Arg, int numOfArg, int* indexOrderd, int numOfOrd)
{
    int indexBest = 0;
    int valueBest; // 记录当前最佳值
    int* checked; // 检查数组
    checked = new int[numOfArg]; // 为检查数组分配内存

    // 确保 Arg 的第一个元素大于一个小值
    assert(Arg[0] > -999999999);

    // 初始化 checked 数组
    for (int i = 0; i < numOfArg; ++i)
        checked[i] = 0;

    // 遍历 numOfOrd 次，找到每次的最佳值
    for (int i = 0; i < numOfOrd; ++i)
    {
        valueBest = -999999999; // 设置当前最佳值为一个极小值
        for (int j = 0; j < numOfArg; ++j)
        {
            // 查找尚未处理的最大值
            if ((Arg[j] > valueBest) && checked[j] == 0) {
                valueBest = Arg[j]; // 更新最佳值
                indexBest = j; // 记录最佳索引
            }
        }
        indexOrderd[i] = indexBest; // 更新输出索引数组
        checked[indexBest] = 1; // 标记该索引已处理
    }

    delete[] checked; // 释放检查数组的内存
}

// 对 int 类型数组进行排序
void TSort::Sort(int* Arg, int numOfArg)
{
    int indexBest = 0;
    int valueBest; // 记录当前最佳值
    int stock; // 临时变量用于交换

    // 确保 Arg 的第一个元素小于一个大值
    assert(Arg[0] < 99999999);

    // 冒泡排序算法
    for (int i = 0; i < numOfArg; ++i)
    {
        valueBest = 99999999; // 设置当前最佳值为一个极大值
        for (int j = i; j < numOfArg; ++j)
        {
            // 查找当前未排序部分的最小值
            if ((Arg[j] < valueBest)) {
                valueBest = Arg[j]; // 更新最佳值
                indexBest = j; // 记录最佳索引
            }
        }
        // 交换当前元素和找到的最小值
        stock = Arg[i];
        Arg[i] = Arg[indexBest];
        Arg[indexBest] = stock;
    }
}
