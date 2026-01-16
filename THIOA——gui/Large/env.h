#ifndef __ENVIRONMENT__
#define __ENVIRONMENT__

#ifndef __INDI__
#include "indi.h"  // 包含个体类的定义
#endif

#ifndef __RAND__
#include "rand.h"  // 包含随机数生成相关的定义
#endif

#ifndef __EVALUATOR__
#include "evaluator.h"  // 包含评估器类的定义
#endif

#ifndef __Cross__
#include "cross.h"  // 包含交叉操作相关的定义
#endif

#ifndef __KOPT__
#include "kopt.h"  // 包含2-opt局部搜索相关的定义
#endif

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <time.h>

class TEnvironment {
public:
  TEnvironment();   // 构造函数
  ~TEnvironment();  // 析构函数
  
  void Define();    /* 定义变量，通常包括初始化一些算法参数 */
  
  void DoIt();      /* 遗传算法的主过程 */
  void Init();      /* 初始化遗传算法 */
  
  bool TerminationCondition();   /* 判断是否满足终止条件 
                                    (决定是否进入下一阶段或终止算法) */
  
  void SetAverageBest();  /* 计算种群的平均路径长度和最佳路径长度 */
  
  void InitPop();   /* 创建初始种群 */
  
  void SelectForMating();  /* 在每一代中选择用于交配的父母 */
  
  void SelectForSurvival(int s); /* 不执行任何操作（选择用于生存的个体） */
  
  void GenerateKids(int s);  /* 生成子代解并选择用于生存的个体 */
  
  void GetEdgeFreq();  /* 计算种群中各条边的频率 */

  void PrintOn(int n, char* dstFile);  /* 显示并写入实验结果摘要 */
  
  void WriteBest(char* dstFile);       /* 将最佳解写入文件 */
  
  void WritePop(int n, char* dstFile); /* 将当前种群写入文件 */
  
  void ReadPop(char* fileName);        /* 读取初始种群文件 */

  // 遗传算法相关的变量
  TEvaluator* fEvaluator;    /* 评估器，用于计算边的距离 */
  TCross* tCross;            /* 边组装交叉（EAX）操作 */
  TKopt* tKopt;              /* 2-opt局部搜索操作 */
  char *fFileNameTSP;        /* TSP实例文件的文件名 */
  char *fFileNameInitPop;    /* 初始种群文件的文件名 */

  int fNumOfPop;             /* 种群的数量 (论文中的 N_pop) */
  int fNumOfKids;            /* 子代的数量 (论文中的 N_ch) */
  TIndi* tCurPop;            /* 当前种群中的个体 */
  TIndi tBest;               /* 当前种群中最优的解 */
  
  int fCurNumOfGen;          /* 当前的代数 */
  long int fAccumurateNumCh;  /* 累计的子代数量 */
  int fBestNumOfGen;         /* 发现当前最佳解的代数 */
  long int fBestAccumeratedNumCh;  /* 发现当前最佳解时的累计子代数量 */
  
  int **fEdgeFreq;           /* 种群中每条边的频率 */
  double fAverageValue;       /* 种群的平均路径长度 */
  int fBestValue;            /* 种群中最佳路径的长度 */
  int fBestIndex;            /* 最优个体在种群中的索引 */
  
  int* fIndexForMating;      /* 交配列表 (论文中的 r[]) */

  int fStagBest;             /* 在最佳路径没有改进的代数 */
  int fFlagC[10];            /* 指定EAX和选择策略的配置 */
  int fStage;                /* 当前阶段 */
  int fMaxStagBest;          /* 如果 fStagBest = fMaxStagBest，进入下一阶段 */
  int fCurNumOfGen1;         /* 阶段 I 终止时的代数 */

  // 计时相关变量
  clock_t fTimeStart, fTimeInit, fTimeEnd;  /* 用于测量执行时间 */
};

#endif
