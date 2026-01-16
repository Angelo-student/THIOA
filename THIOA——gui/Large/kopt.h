#ifndef __KOPT__  // 防止多重包含的宏
#define __KOPT__

#ifndef __RAND__
#include "rand.h"  // 引入随机数生成相关的头文件
#endif

#ifndef __Sort__
#include "sort.h"  // 引入排序相关的头文件
#endif

#ifndef __INDI__
#include "indi.h"  // 引入个体相关的头文件
#endif

#ifndef __EVALUATOR__
#include "evaluator.h"  // 引入评估器相关的头文件
#endif

#include <stdio.h>     // C标准输入输出库
#include <stdlib.h>    // C标准库
#include <assert.h>    // 断言库

class TKopt {
public:
  TKopt( int N );                         // 构造函数，接受城市数量 N
  ~TKopt();                               // 析构函数
  void SetInvNearList();                  // 设置逆近邻列表的函数
  void TransIndiToTree( TIndi& indi );    // 将个体（路径）转换为树结构
  void TransTreeToIndi( TIndi& indi );    // 将树结构转换为个体（路径）

  void DoIt( TIndi& tIndi );               // 使用 2-opt 邻域应用局部搜索
  void Sub();                              // 子函数（具体功能需查看实现）
  int GetNext( int t );                   // 获取下一个城市的函数
  int GetPrev( int t );                   // 获取前一个城市的函数
  void IncrementImp( int flagRev );       // 增加改进的函数，标记是否反转
  void CombineSeg( int segL, int segS );  // 合并两个段的函数

  void CheckDetail();                      // 检查详细信息的函数
  void CheckValid();                       // 检查路径有效性的函数

  void Swap(int &a,int &b);               // 交换两个整数的函数
  int Turn( int &orient );                 // 转动方向的函数

  void MakeRandSol( TIndi& indi );        // 设置随机路径的函数

  TEvaluator* eval;                        // 指向评估器的指针

private:
  int fN;                                  // 城市数量

  int fFixNumOfSeg;                       // 固定段的数量
  int fNumOfSeg;                           // 当前段的数量
  int **fLink;                             // 存储城市连接信息的二维数组
  int *fSegCity;                           // 每个段中的城市
  int *fOrdCity;                           // 存储城市的顺序

  int *fOrdSeg;                            // 储存段的顺序
  int *fOrient;                            // 储存方向信息
  int **fLinkSeg;                          // 存储段的连接信息
  int *fSizeSeg;                           // 各段的大小

  int **fCitySeg;                          // 城市在各段中的表示
  int *fT;                                 // 用于临时存储和操作的数组
  int fFlagRev;                            // 标记是否进行反转
  int fTourLength;                         // 当前路径长度

  int *fActiveV;                           // 活跃的变量或城市标记
  int **fInvNearList;                      // 逆近邻列表
  int *fNumOfINL;                          // 每个城市的逆临近城市的数量

  int *fArray;                             // 用于存储中间计算的数组
  int *fCheckN;                            // 检查数量的数组
  int *fGene;                              // 表示基因或路径的信息
  int *fB;                                 // 备用数组
};

#endif  // 结束条件
