#include <random>       // 引入 C++ 标准库中的随机数生成器
#include <ctime>       // 引入时间库，用于获取当前时间
#include <algorithm>    // 引入算法库，提供 shuffle 等算法
#ifndef __RAND__      // 如果没有定义 __RAND__
#include "rand.h"      // 引入 rand.h 头文件
#endif

TRandom* tRand = NULL; // 全局指针，指向 TRandom 类的实例
std::mt19937 rng;      // Mersenne Twister 随机数生成器

// 初始化随机数生成器，不带参数，使用当前时间作为种子
void InitURandom()
{
    unsigned seed = static_cast<unsigned>(time(0)); // 使用当前时间作为种子
    rng.seed(seed);                                   // 用种子初始化随机数生成器
    tRand = new TRandom();                           // 创建 TRandom 类的实例
}

// 初始化随机数生成器，带参数
void InitURandom(int dd)
{
    rng.seed(dd);                                    // 使用提供的值作为种子初始化随机数生成器
    tRand = new TRandom();                           // 创建 TRandom 类的实例
}

// TRandom 构造函数
TRandom::TRandom()
{
}

// TRandom 析构函数
TRandom::~TRandom()
{
}

// 生成指定范围内的随机整数
int TRandom::Integer(int minNumber, int maxNumber)
{
    std::uniform_int_distribution<int> dist(minNumber, maxNumber); // 定义均匀分布
    return dist(rng); // 从分布中生成并返回随机整数
}

// 生成指定范围内的随机浮点数
double TRandom::Double(double minNumber, double maxNumber)
{
    std::uniform_real_distribution<double> dist(minNumber, maxNumber); // 定义均匀浮点数分布
    return dist(rng); // 从分布中生成并返回随机浮点数
}

// 对数组进行随机抽样生成排列
void TRandom::Permutation(int* array, int numOfElement, int numOfSample)
{
    if (numOfElement <= 0) // 如果元素数量小于等于0，直接返回
        return;

    std::vector<int> b(numOfElement, 0); // 创建一个与元素数量相同的标记数组，初始为0
    for (int i = 0; i < numOfSample; i++) // 循环生成样本
    {
        int r = Integer(0, numOfElement - i - 1); // 生成一个随机数 r
        int k = 0; // k 用于记录标记数组中的位置
        for (int j = 0; j <= r; j++) // 查找第 r 个未被标记的元素
        {
            while (b[k] == 1) // 如果该位置已被标记，继续向后查找
            {
                k++;
            }
            k++; // 移动到下一个位置
        }
        array[i] = k - 1; // 记录未被标记的元素的索引
        b[k - 1] = 1; // 将该元素标记为已使用
    }
}

// 生成正态分布随机数
double TRandom::NormalDistribution(double mu, double sigma)
{
    std::normal_distribution<double> dist(mu, sigma); // 定义正态分布
    return dist(rng); // 从分布中生成并返回随机数
}

// 对数组进行随机洗牌
void TRandom::Shuffle(int* array, int numOfElement)
{
    std::vector<int> vec(array, array + numOfElement); // 将数组复制到 vector
    std::shuffle(vec.begin(), vec.end(), rng); // 使用随机数生成器对 vector 进行洗牌
    std::copy(vec.begin(), vec.end(), array); // 将洗牌后的结果复制回原数组
}
