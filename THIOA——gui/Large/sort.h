#ifndef __SORT__  // 如果没有定义 __SORT__，则执行下面的代码
#define __SORT__  // 定义 __SORT__

#include <stdio.h>  // 引入标准输入输出库
#include <stdlib.h> // 引入标准库，提供内存分配等功能
#include <math.h>   // 引入数学库，提供数学函数
#include <assert.h> // 引入断言库，用于调试

// 函数声明：初始化排序
extern void InitSort(void);

// TSort 类的定义
class TSort {
public:
    TSort(); // 构造函数
    ~TSort(); // 析构函数

    // 用于根据 Arg 数组对索引进行排序（支持 double 类型）
    void Index(double* Arg, int numOfArg, int* indexOrderd, int numOfOrd);

    // 用于根据 Arg 数组对索引进行排序（支持 int 类型）
    void Index(int* Arg, int numOfArg, int* indexOrderd, int numOfOrd);

    // 另一种基于 Arg 数组的索引排序（支持 double 类型）
    void Index_B(double* Arg, int numOfArg, int* indexOrderd, int numOfOrd);

    // 另一种基于 Arg 数组的索引排序（支持 int 类型）
    void Index_B(int* Arg, int numOfArg, int* indexOrderd, int numOfOrd);

    // 排序函数，接受一个 int 数组
    void Sort(int* Arg, int numOfArg);
};

// 外部变量声明：指向 TSort 类的指针
extern TSort* tSort;

#endif  // 结束 __SORT__ 的条件编译
