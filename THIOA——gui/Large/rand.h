#ifndef __RAND__  // 如果没有定义 __RAND__
#define __RAND__  // 定义 __RAND__

#include <stdio.h>  // 包含标准输入输出头文件
#include <stdlib.h> // 包含标准库头文件
#include <math.h>   // 包含数学函数头文件
#include <time.h>   // 包含时间函数头文件

// 外部函数声明，初始化随机数生成器
extern void InitURandom(int);
extern void InitURandom(void);

// 随机数生成器类
class TRandom {
public:
	TRandom(); // 构造函数
	~TRandom(); // 析构函数

	// 生成指定范围内的随机整数
	int Integer(int minNumber, int maxNumber);

	// 生成指定范围内的随机浮点数
	double Double(double minNumber, double maxNumber);

	// 对数组进行随机抽样生成排列
	void Permutation(int* array, int numOfelement, int numOfSample);

	// 生成正态分布随机数
	double NormalDistribution(double mu, double sigma);

	// 对数组进行随机洗牌
	void Shuffle(int* array, int numOfElement);
};

// 外部声明 TRandom 指针
extern TRandom* tRand;

#endif  // 结束 __RAND__ 的条件编译
