#ifndef __EVALUATOR__
#include "evaluator.h"
#endif

TEvaluator::TEvaluator()
{
  fEdgeDisOrder = NULL;
  fNearCity = NULL;
  Ncity = 0;
  fNearNumMax = 100;                               // Large
}

TEvaluator::~TEvaluator()
{
  for ( int i = 0; i < Ncity; ++i ) 
    delete [] fEdgeDisOrder[ i ];
  delete [] fEdgeDisOrder;
  for ( int i = 0; i < Ncity; ++i ) 
    delete [] fNearCity[ i ];
  delete [] fNearCity;
  for ( int i = 0; i < Ncity; ++i ) 
    delete [] fEdgeDisOrder[ i ];                   // Large
  delete [] fEdgeDisOrder;
  
  delete [] x;
  delete [] y;
}

void TEvaluator::SetInstance( char filename[] )
{
  FILE* fp;
  int flag;
  int n;
  char word[ 80 ];
  int *DisTmp;

  // 打开文件以读取实例数据
  fp = fopen( filename, "r" );

  ////// 读取实例数据 //////

  // 持续读取文件，直到遇到关键词，解析出必要的信息
  while( 1 ){
    if( fscanf( fp, "%s", word ) == EOF )
      break; // 如果到达文件末尾，退出循环

    // 读取城市数量
    if( strcmp( word, "DIMENSION" ) == 0 ){
      fscanf( fp, "%s", word );
      assert( strcmp( word, ":" ) == 0 ); // 检查文件格式是否正确
      fscanf( fp, "%d", &Ncity ); // 读取城市数量
    }

    // 读取边权重类型
    if( strcmp( word, "EDGE_WEIGHT_TYPE" ) == 0 ){
      fscanf( fp, "%s", word );
      assert( strcmp( word, ":" ) == 0 ); // 检查文件格式
      fscanf( fp, "%s", fType ); // 读取边权重类型
    }

    // 检测到节点坐标部分，退出循环
    if( strcmp( word, "NODE_COORD_SECTION" ) == 0 )
      break;
  }

  // 如果没有成功读取到节点坐标部分，则输出错误并退出
  if( strcmp( word, "NODE_COORD_SECTION" ) != 0 ){
    printf( "Error in reading the instance\n" );
    exit(0);
  }

  // 初始化城市坐标数组
  x = new double [ Ncity ];
  y = new double [ Ncity ];
  int checkedN[ Ncity ];  // 用于标识哪些城市已被检查

  int xi, yi;
  // 读取每个城市的坐标
  for( int i = 0; i < Ncity; ++i )
  {
    fscanf( fp, "%d", &n );  // 读取城市编号（不使用）
    assert( i+1 == n ); // 检查城市编号是否与顺序一致
    fscanf( fp, "%s", word );
    x[ i ] = atof( word ); // 读取并存储X坐标
    fscanf( fp, "%s", word );
    y[ i ] = atof( word ); // 读取并存储Y坐标
  }

  fclose(fp);  // 关闭文件
  //////////////////////////

  // 初始化适用于最大邻近城市数量的邻近城市和边距离排序矩阵
  fEdgeDisOrder = new int* [ Ncity ];
  for( int i = 0; i < Ncity; ++i )
    fEdgeDisOrder[ i ] = new int [ fNearNumMax+1 ];
  fNearCity = new int* [ Ncity ];
  for( int i = 0; i < Ncity; ++i )
    fNearCity[ i ] = new int [ fNearNumMax+1 ];

  DisTmp = new int [ Ncity ];  // 临时存储城市距离的数组
  int city_num;
  int min_dis;

  // 检查是否需要修改代码以适配不同的边权重类型，仅支持 "EUC_2D"
  if( strcmp( fType, "EUC_2D" ) != 0 ){
    printf( "Please modify program code in TEvaluator::Direct( ) appropriately when EDGE_WEIGHT_TYPE is not EUC_2D.\n" );
    exit( 1 );  // 如果类型不支持，输出信息并退出
  }

  // 计算并存储城市之间的距离和邻近城市信息
  for( int i = 0; i < Ncity ; ++i )
  {
    if( strcmp( fType, "EUC_2D" ) == 0  ) {
      // 使用欧几里得距离计算城市之间的距离
      for( int j = 0; j < Ncity ; ++j )
	  DisTmp[ j ] = (int)(sqrt((x[i]-x[j])*(x[i]-x[j])+(y[i]-y[j])*(y[i]-y[j]))+0.5);
    }
    else if( strcmp( fType, "ATT" ) == 0  ) {
      // 其他距离标准，与 "ATT" 类型匹配
      for( int j = 0; j < Ncity ; ++j ){
	  double r = (sqrt(((x[i]-x[j])*(x[i]-x[j])+(y[i]-y[j]))/10.0));
	  int t = (int)r;
	  if( (double)t < r ) DisTmp[ j ] = t+1;
	  else DisTmp[ j ] = t;
      }
    }
    else if( strcmp( fType, "CEIL_2D" ) == 0  ) {
      // 使用上取整的欧几里得距离计算所有城市之间的距离
      for( int j = 0; j < Ncity ; ++j )
	  DisTmp[ j ] = (int)ceil(sqrt((x[i]-x[j])*(x[i]-x[j])+(y[i]-y[j])*(y[i]-y[j])));
    }
    else{
      // 如果类型不支持，给出错误提示并退出
      printf( "EDGE_WEIGHT_TYPE is not supported\n" );
      exit( 1 );
    }

    // 初始化检查数组并存储某城市到邻近城市及其距离的排序结果
    for( int j3 = 0; j3 < Ncity; ++j3 )         // 初始化检查数组
      checkedN[ j3 ] = 0;

    checkedN[ i ] = 1;                          // 标记当前城市已经检查
    fNearCity[ i ][ 0 ] = i;                    // 自己与自己的距离为0
    fEdgeDisOrder[ i ][ 0 ] = 0;                // 自己与自己的边长为0

    // 为每个城市找到 fNearNumMax 数目的最近城市
    for( int j1 = 1; j1 <= fNearNumMax; ++j1 )
    {
      min_dis = 100000000; // 定义一个很大的距离，表示最短距离的初始化
      for( int j2 = 0; j2 < Ncity ; ++j2 )
      {
	if( DisTmp[ j2 ] <= min_dis && checkedN[ j2 ] == 0 ) // 找到更短未检查过的城市
	{
	  city_num = j2;
	  min_dis = DisTmp[ j2 ];
	}
      }
      fNearCity[ i ][ j1 ] = city_num; // 存储最近城市
      fEdgeDisOrder[ i ][ j1 ] = min_dis; // 存储距离排序结果
      checkedN[ city_num ] = 1; // 标记该城市已检查
    }
  }

  // 释放临时存储距离的数组
  delete [] DisTmp;
}



void TEvaluator::DoIt( TIndi& indi )
{
  int d;
  d = 0;  
  // 计算整个路径的总长度
  for(int i = 0; i < Ncity; ++i )
  {
    d = d + this->Direct( i, indi.fLink[i][0] );
    d = d + this->Direct( i, indi.fLink[i][1] );
  }
  // 将总距离的一半赋值给 indi 的评价值
  indi.fEvaluationValue = d/2;
}



int TEvaluator::Direct(int i, int j) {
    int d = 0;  // 初始化 d
    double r;

    if (strcmp(fType, "EUC_2D") == 0) {
        d = (int)(sqrt((x[i] - x[j]) * (x[i] - x[j]) + (y[i] - y[j]) * (y[i] - y[j])) + 0.5); // 补全括号
    }
    else if (strcmp(fType, "CEIL_2D") == 0) {
        d = (int)ceil(sqrt((x[i] - x[j]) * (x[i] - x[j]) + (y[i] - y[j]) * (y[i] - y[j]))); // 补全括号
    }
    else if (strcmp(fType, "ATT") == 0) {
        r = sqrt(((x[i] - x[j]) * (x[i] - x[j]) + (y[i] - y[j]) * (y[i] - y[j])) / 10.0);
        int t = (int)r;
        d = (double)t < r ? t + 1 : t; // 优化为单行表达式
    }
    else {
        // 处理其他类型的边权重
        // 可以在此添加自定义的距离计算
        // 例如使用简单的欧几里得距离，或抛出错误
        d = (int)(sqrt((x[i] - x[j]) * (x[i] - x[j]) + (y[i] - y[j]) * (y[i] - y[j])) + 0.5); // 默认处理
    }

    return d;
}



void TEvaluator::TranceLinkOrder( TIndi& indi )  // Large
{
  int a, b;
  // 遍历所有城市
  for(int i = 0; i < Ncity; ++i )
  {
    // 当前城市
    a = i;
    // 连接城市1
    b = indi.fLink[ i ][ 0 ];
    // 计算当前城市与连接城市1之间的顺序，并存储在fOrder中
    indi.fOrder[ i ][ 0 ] = this->GetOrd( a, b );

    // 连接城市2
    b = indi.fLink[ i ][ 1 ];
    // 计算当前城市与连接城市2之间的顺序，并存储在fOrder中
    indi.fOrder[ i ][ 1 ] = this->GetOrd( a, b );
  }
}



int TEvaluator::GetOrd( int a, int b )  // Large
{
  for( int s = 0; s <= fNearNumMax; ++s )
  {
    if( fNearCity[ a ][ s ] == b )
      return s;
  }
  return -1;
}


void TEvaluator::WriteTo( FILE* fp, TIndi& indi ) 
{
  // 确保 indi 中的城市数量与 Ncity 一致
  assert( Ncity == indi.fN );

  // 定义一个数组存储路径顺序
  int Array[Ncity];
  // 定义当前城市、下一个城市、前一个城市、起始城市和计数器
  int curr, next, pre, st, count;

  // 初始化参数
  count = 0;
  pre = -1;     // 起始时没有前一个城市
  curr = 0;     // 从城市0开始
  st = 0;       // 路径开始的城市

  while (1)
  {
    // 将当前城市添加到路径数组中（城市编号+1）
    Array[count++] = curr + 1;

    // 如果路径长度大于城市总数，报告错误
    if (count > Ncity) {
      printf("Invalid\n");
      return;
    }

    // 确定下一个城市，根据当前城市的连结，避免与前驱城市重复
    if (indi.fLink[curr][0] == pre)
      next = indi.fLink[curr][1];
    else
      next = indi.fLink[curr][0];

    // 更新前一个城市和当前城市
    pre = curr;
    curr = next;

    // 如果回到了起始城市，则结束循环
    if (curr == st)
      break;
  }

  // 检查路径是否有效
  if (this->CheckValid(Array, indi.fEvaluationValue) == false) {
    printf("Individual is invalid \n");
  }

  // 将个体的城市数量和评价值写入文件
  fprintf(fp, "%d %d\n", indi.fN, indi.fEvaluationValue);
  // 将路径顺序写入文件
  for (int i = 0; i < indi.fN; ++i)
    fprintf(fp, "%d ", Array[i]);
  fprintf(fp, "\n");
}



bool TEvaluator::ReadFrom( FILE* fp, TIndi& indi )
{
  // 确保 indi 中的城市数量与 Ncity 一致
  assert( Ncity == indi.fN );

  // 定义一个数组用于存储路径上的城市顺序
  int Array[Ncity];
  int curr, next, pre, st, count;
  int N, value;

  // 从文件中读取城市数量和路径评价值
  if (fscanf(fp, "%d %d", &N, &value) == EOF)
    return false;  // 如果读取失败，返回 false
  assert(N == Ncity);  // 验证读取的城市数是否与实际城市数匹配
  indi.fN = N;
  indi.fEvaluationValue = value;
  // 从文件中读取城市的访问顺序到数组中
  for (int i = 0; i < Ncity; ++i) {
    if (fscanf(fp, "%d", &Array[i]) == EOF)
      return false;  // 如果读取失败，返回 false
  }
  // 检查路径是否有效
  if (this->CheckValid(Array, indi.fEvaluationValue) == false) {
    printf("Individual is invalid \n");
    return false;  // 如果无效，打印信息并返回 false
  }
  // 调整索引，从文件读取的城市编号通常是1开始的，将它们转换为0开始
  for (int i = 0; i < Ncity; ++i) {
    Array[i] -= 1;
  }
  // 更新每个城市的连接信息
  for (int i = 1; i < Ncity-1; ++i) {
    indi.fLink[Array[i]][0] = Array[i-1];  // 设置上一个城市
    indi.fLink[Array[i]][1] = Array[i+1];  // 设置下一个城市
  }
  // 为第一个和最后一个城市设置连接信息，形成一个循环路径
  indi.fLink[Array[0]][0] = Array[Ncity-1];
  indi.fLink[Array[0]][1] = Array[1];
  indi.fLink[Array[Ncity-1]][0] = Array[Ncity-2];
  indi.fLink[Array[Ncity-1]][1] = Array[0];

  return true;  // 成功读取并初始化返回 true
}




bool TEvaluator::CheckValid( int* array, int value )
{
  // 定义一个数组来检查每个城市是否被访问过
  int check[Ncity];
  int distance;

  // 初始化检查数组为0，表示所有城市都未访问
  for (int i = 0; i < Ncity; ++i) {
    check[i] = 0;
  }

  // 增加每个城市的访问计数
  for (int i = 0; i < Ncity; ++i)
    ++check[array[i] - 1]; // 假设array中的城市编号从1开始，所以减1

  // 检查每个城市是否正好被访问一次
  for (int i = 0; i < Ncity; ++i) {
    if (check[i] != 1) {
      return false; // 如果有城市不是被访问一次，返回false
    }
  }

  // 计算路径的总距离
  distance = 0;
  for (int i = 0; i < Ncity - 1; ++i) {
    distance += this->Direct(array[i] - 1, array[i + 1] - 1); // 获取相邻城市之间的距离
  }
  // 加上路径最后返回起始城市的距离
  distance += this->Direct(array[Ncity - 1] - 1, array[0] - 1);

  // 检查计算的距离是否与提供的评价值匹配
  if (distance != value) {
    return false;
  }

  // 如果所有检查通过，返回true
  return true;
}

