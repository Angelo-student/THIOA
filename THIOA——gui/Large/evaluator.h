#ifndef __EVALUATOR__
#define __EVALUATOR__

#ifndef __INDI__
#include "indi.h"  // 包含TIndi个体类的定义
#endif

#include <string.h>  // 用于字符串操作
#include <assert.h>  // 用于断言检查

// 评估器类，负责读取TSP实例数据、评估解的质量、提供邻近城市等功能
class TEvaluator {
 public:
  TEvaluator();  // 构造函数
  ~TEvaluator();  // 析构函数
  
  // 设置TSP问题的实例，读取城市坐标数据
  void SetInstance( char filename[] );
  
  // 评估解的路径长度
  void DoIt( TIndi& indi );

  // 返回城市i到城市j的直接距离（可能包含大数据集操作）
  int Direct( int i, int j );           // Large 

  // 根据城市的顺序计算出连接关系
  void TranceLinkOrder( TIndi& indi );  // Large

  // 获取城市a和b之间的排序顺序（用于边频率等计算）
  int GetOrd( int a, int b );           // Large

  // 将个体解写入文件
  void WriteTo( FILE* fp, TIndi& indi );    

  // 从文件中读取个体解
  bool ReadFrom( FILE* fp, TIndi& indi );   

  // 检查解是否有效（用于检测路径合法性）
  bool CheckValid( int* array, int value ); 

  char fType[ 80 ];  // 问题类型，通常是TSP，但可以扩展为其他类型的优化问题  // Large 

  int fNearNumMax;  // 每个城市最多的邻近城市数量
  int **fNearCity;  // 邻近城市的矩阵，存储城市的邻近列表
  int **fEdgeDisOrder;  // 边的距离排序矩阵，存储城市之间的距离  // Large
  int Ncity;  // 城市数量
  double *x;  // 每个城市的X坐标数组
  double *y;  // 每个城市的Y坐标数组
};

#endif
