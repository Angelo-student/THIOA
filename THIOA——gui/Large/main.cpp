#ifndef __ENVIRONMENT__
#include "env.h"  // 包含环境类的定义
#endif

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;


// 主函数，参数为命令行参数
int main( int argc, char* argv[] )
{
  int maxNumOfTrial;  // 最大试验次数
  
  // 从命令行参数读取最大试验次数
  cout << "请输入最大试验次数：" << endl;
  sscanf( argv[1], "%d", &maxNumOfTrial ); 
  char* dstFile = argv[2];  // 输出文件名
  
  // 创建环境对象
  TEnvironment* gEnv = NULL;
  gEnv = new TEnvironment();
  
  // 初始化随机数生成器
  InitURandom();  

  int d;
  
  // 从命令行参数读取种群大小
  cout << "请输入种群大小：" << endl;
  sscanf( argv[3], "%d", &d );
  gEnv->fNumOfPop = d;  // 种群大小

  // 从命令行参数读取子代数量
  cout << "请输入子代数量：" << endl;
  sscanf( argv[4], "%d", &d );
  gEnv->fNumOfKids = d;  // 子代数量
  
  // 从命令行参数读取TSP文件名
  cout << "请输入TSP文件名：" << endl;
  gEnv->fFileNameTSP = argv[5];  
  gEnv->fFileNameInitPop = NULL;  // 初始化种群文件默认为NULL
  
  // 如果提供了初始化种群文件，则读取
  if( argc == 7 )
    gEnv->fFileNameInitPop = argv[6];

  // 定义环境，包括读取TSP数据文件等操作
  gEnv->Define();
  
  // 主循环，进行多次试验
  for( int n = 0; n < maxNumOfTrial; ++n )
  { 
    // 进行一次遗传算法操作
    gEnv->DoIt();

    // 打印结果到文件中
    gEnv->PrintOn( n, dstFile );       
    gEnv->WriteBest( dstFile );  // 将最优解写入文件

    // 该部分代码被注释掉，可以用于写出种群信息
    // gEnv->WritePop( n, dstFile );
  }
  
  return 0;
}
