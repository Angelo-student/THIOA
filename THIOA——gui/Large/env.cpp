#ifndef __ENVIRONMENT__
#include "env.h"  // 包含环境类的定义
#endif

#include <math.h>

void MakeRandSol( TEvaluator* eval , TIndi& indi );  // 生成随机解的函数
void Make2optSol( TEvaluator* eval , TIndi& indi );  // 生成2-opt解的函数

TEnvironment::TEnvironment()
{
  fEvaluator = new TEvaluator();  // 初始化评估器
}


TEnvironment::~TEnvironment()
{
  delete [] fIndexForMating;  // 释放用于交配选择的索引数组
  delete [] tCurPop;          // 释放当前种群的内存
  delete fEvaluator;          // 释放评估器
  delete tCross;              // 释放交叉操作对象

  int N = fEvaluator->Ncity;  // 获取城市数量
  for( int i = 0; i < N; ++i )
    delete [] fEdgeFreq[ i ];  // 释放边频率数组
  delete [] fEdgeFreq;         // 释放边频率指针数组
}


void TEnvironment::Define()
{
  fEvaluator->SetInstance( fFileNameTSP );  // 设置TSP实例
  int N = fEvaluator->Ncity;  // 获取城市数量

  fIndexForMating = new int [ fNumOfPop + 1 ];  // 初始化用于交配选择的索引数组

  tCurPop = new TIndi [ fNumOfPop ];  // 创建当前种群的个体数组
  for ( int i = 0; i < fNumOfPop; ++i )
    tCurPop[i].Define( N );  // 为每个个体定义城市数量

  tBest.Define( N );  // 定义最优解的城市数量

  tCross = new TCross( N );  // 初始化交叉操作
  tCross->eval = fEvaluator;  // 传递评估器指针给交叉操作
  tCross->fNumOfPop = fNumOfPop;  // 设置种群大小

  tKopt = new TKopt( N );  // 初始化2-opt操作
  tKopt->eval = fEvaluator;  // 传递评估器指针给2-opt操作
  tKopt->SetInvNearList();  // 设置邻近表

  fEdgeFreq = new int* [ N ];  // 初始化边频率矩阵
  for( int i = 0; i < N; ++i )
    fEdgeFreq[ i ] = new int [ fEvaluator->fNearNumMax+1 ];  // 初始化每条边的频率，尺寸较大
}


void TEnvironment::DoIt()
{
  this->fTimeStart = clock();  // 记录开始时间

  if( fFileNameInitPop == NULL )
    this->InitPop();  // 如果没有初始种群文件，生成随机初始种群
  else
    this->ReadPop( fFileNameInitPop );  // 从文件读取初始种群

  this->fTimeInit = clock();  // 记录初始化时间

  this->Init();  // 初始化遗传算法
  this->GetEdgeFreq();  // 计算边频率

  while( 1 )
  {
    this->SetAverageBest();  // 计算当前种群的平均路径长度和最优路径长度
    printf( "%d: %d %lf\n", fCurNumOfGen, fBestValue, fAverageValue );

    if( this->TerminationCondition() ) break;  // 判断是否满足终止条件

    this->SelectForMating();  // 选择用于交配的个体

    for( int s = 0; s < fNumOfPop; ++s )
    {
      this->GenerateKids( s );  // 生成子代
      this->SelectForSurvival( s );  // 选择用于存活的个体
    }
    ++fCurNumOfGen;  // 代数增加
  }

  this->fTimeEnd = clock();  // 记录结束时间
}

/* 初始化遗传算法的参数 */
void TEnvironment::Init()
{
  fAccumurateNumCh = 0;  // 累计子代数清零
  fCurNumOfGen = 0;      // 当前代数清零
  fStagBest = 0;         // 最优解停滞代数清零
  fMaxStagBest = 0;      // 最大停滞代数清零
  fStage = 1;            // 进入第一阶段
  fFlagC[ 0 ] = 4;       // 多样性保持策略：1:贪婪, 4:熵 (参考论文第4节)
  fFlagC[ 1 ] = 1;       // Eset类型：1:单一AB，2:Block2 (参考论文第3节)
}

/* 判断是否满足终止条件 */
bool TEnvironment::TerminationCondition()
{
    if (fAverageValue - fBestValue < 0.001)  // 如果平均路径长度和最优路径长度差异小于0.001，停止
        return true;

    if (fStage == 1)  /* 阶段I */
    {
        if (fStagBest == int(1500 / fNumOfKids) && fMaxStagBest == 0) {
            fMaxStagBest = int(fCurNumOfGen / 10);  // 设置最大停滞代数为当前代数的十分之一
        }
        else if (fMaxStagBest != 0 && fMaxStagBest <= fStagBest) {
            fStagBest = 0;
            fMaxStagBest = 0;
            fCurNumOfGen1 = fCurNumOfGen;
            fFlagC[1] = 2;  // 进入阶段II
            fStage = 2;
        }
        return false;
    }

    if (fStage == 2) { /* 阶段II */
        if (fStagBest == int(1500 / fNumOfKids) && fMaxStagBest == 0) {
            fMaxStagBest = int((fCurNumOfGen - fCurNumOfGen1) / 10);  // 设置最大停滞代数
        }
        else if (fMaxStagBest != 0 && fMaxStagBest <= fStagBest) {
            return true;  // 阶段II结束，算法停止
        }
        return false;
    }

    return false;
}

/* 计算当前种群的平均路径长度和最优路径长度 */
void TEnvironment::SetAverageBest()
{
  int stockBest = tBest.fEvaluationValue;  // 存储当前最优解的路径长度

  fAverageValue = 0.0;
  fBestIndex = 0;
  fBestValue = tCurPop[0].fEvaluationValue;

  for(int i = 0; i < fNumOfPop; ++i ){
    fAverageValue += tCurPop[i].fEvaluationValue;  // 累计路径长度
    if( tCurPop[i].fEvaluationValue < fBestValue ){  // 更新最优解
      fBestIndex = i;
      fBestValue = tCurPop[i].fEvaluationValue;
    }
  }

  tBest = tCurPop[ fBestIndex ];  // 更新最优个体
  fAverageValue /= (double)fNumOfPop;  // 计算平均路径长度

  if( tBest.fEvaluationValue < stockBest ){
    fStagBest = 0;  // 如果找到更优解，重置停滞计数
    fBestNumOfGen = fCurNumOfGen;  // 记录发现最优解的代数
    fBestAccumeratedNumCh = fAccumurateNumCh;  // 记录发现最优解时的子代数
  }
  else ++fStagBest;  // 否则增加停滞计数
}

/* 生成初始种群 */
void TEnvironment::InitPop()
{
  for ( int i = 0; i < fNumOfPop; ++i ){
    tKopt->MakeRandSol( tCurPop[ i ] );  // 生成随机解
    tKopt->DoIt( tCurPop[ i ] );  // 使用2-opt局部搜索优化解
    fEvaluator->TranceLinkOrder( tCurPop[ i ] );  // 记录解的连接顺序
  }
}

/* 选择交配个体 */
void TEnvironment::SelectForMating()
{
  /* fIndexForMating[] <-- 0到fNumOfPop-1的随机排列 */
  tRand->Permutation( fIndexForMating, fNumOfPop, fNumOfPop );
  fIndexForMating[ fNumOfPop ] = fIndexForMating[ 0 ];  // 循环交配
}

void TEnvironment::SelectForSurvival( int s )
{
  // 这里未实现具体的个体存活选择逻辑
}

/* 生成子代 */
void TEnvironment::GenerateKids( int s )
{
  tCross->SetParents( tCurPop[fIndexForMating[s]], tCurPop[fIndexForMating[s+1]], fFlagC, fNumOfKids );  // 设置交叉父母

  /* 注意：在tCross->DoIt()中，tCurPop[fIndexForMating[s]]将被子代中的最优解替换，边频率也会更新 */
  tCross->DoIt( tCurPop[fIndexForMating[s]], tCurPop[fIndexForMating[s+1]], fNumOfKids, 1, fFlagC, fEdgeFreq );

  fAccumurateNumCh += tCross->fNumOfGeneratedCh;  // 累计生成的子代数
}

/* 计算边频率 */
void TEnvironment::GetEdgeFreq()  // 计算边频率矩阵，矩阵尺寸较大
{
  int N = fEvaluator->Ncity;  // 获取城市数量
  int k0, k1;

  for( int j1 = 0; j1 < N; ++j1 )
    for( int j2 = 0; j2 < fEvaluator->fNearNumMax; ++j2 )
      fEdgeFreq[ j1 ][ j2 ] = 0;  // 初始化边频率为0

  for( int i = 0; i < fNumOfPop; ++i )
  {
    for(int j = 0; j < N; ++j )
    {
      k0 = tCurPop[ i ].fOrder[ j ][ 0 ];  // 获取路径中的两条边
      k1 = tCurPop[ i ].fOrder[ j ][ 1 ];
      if( k0 != -1 )
        ++fEdgeFreq[ j ][ k0 ];  // 增加边k0的频率
      if( k1 != -1 )
        ++fEdgeFreq[ j ][ k1 ];  // 增加边k1的频率
    }
  }
}

/* 打印结果 */
void TEnvironment::PrintOn( int n, char* dstFile )
{
  printf( "n = %d val = %d Gen = %d Time = %d %d\n" ,
	  n,
	  tBest.fEvaluationValue,
	  fCurNumOfGen,
	  (int)((double)(this->fTimeInit - this->fTimeStart)/(double)CLOCKS_PER_SEC),
	  (int)((double)(this->fTimeEnd - this->fTimeStart)/(double)CLOCKS_PER_SEC) );
  fflush(stdout);

  FILE *fp;
  char filename[ 80 ];
  sprintf( filename, "%s_Result", dstFile );
  fp = fopen( filename, "a");

  fprintf( fp, "%d %d %d %d %d\n" ,
	   n,
	   tBest.fEvaluationValue,
	   fCurNumOfGen,
	   (int)((double)(this->fTimeInit - this->fTimeStart)/(double)CLOCKS_PER_SEC),
	   (int)((double)(this->fTimeEnd - this->fTimeStart)/(double)CLOCKS_PER_SEC) );

  fclose( fp );
}

/* 写出最优解 */
void TEnvironment::WriteBest( char* dstFile )
{
  FILE *fp;
  char filename[ 80 ];
  sprintf( filename, "%s_BestSol", dstFile );
  fp = fopen( filename, "a");

  fEvaluator->WriteTo( fp, tBest );  // 写出最优解

  fclose( fp );
}

/* 写出种群 */
void TEnvironment::WritePop( int n, char* dstFile )
{
  FILE *fp;
  char filename[ 80 ];
  sprintf( filename, "%s_POP_%d", dstFile, n );
  fp = fopen( filename, "w");

  for( int s = 0; s < fNumOfPop; ++s )
    fEvaluator->WriteTo( fp, tCurPop[ s ] );  // 写出当前种群

  fclose( fp );
}

/* 读取种群 */
void TEnvironment::ReadPop( char* fileName )
{
  FILE* fp;

  if( ( fp = fopen( fileName, "r" ) ) == NULL ){
    printf( "Read Error1\n");
    fflush( stdout );
    exit( 1 );
  }

  for ( int i = 0; i < fNumOfPop; ++i ){
    if( fEvaluator->ReadFrom( fp, tCurPop[ i ] ) == false ){
      printf( "Read Error2\n");
      fflush( stdout );
      exit( 1 );
    }
    fEvaluator->TranceLinkOrder( tCurPop[ i ] );  // 记录解的连接顺序
  }
  fclose( fp );
}
