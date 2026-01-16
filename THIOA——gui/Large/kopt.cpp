#ifndef __KOPT__
#include "kopt.h"
#endif


TKopt::TKopt( int N )
{
  fN = N;  // 设置城市数量 fN 为传入的参数 N

  // 动态分配二维数组 fLink，用于保存城市连接信息，包含每个城市的两个连接
  fLink = new int* [fN];  // 分配一维数组，包含 N 个指针
  for (int i = 0; i < fN; ++i)
    fLink[i] = new int[2];  // 为每个指针分配一个大小为 2 的整型数组

  fOrdCity = new int[fN];  // 动态分配数组，存储城市的顺序

  fOrdSeg = new int[fN];  // 动态分配数组，存储段的顺序

  fSegCity = new int[fN];  // 动态分配数组，存储每个段中的城市信息

  fOrient = new int[fN];  // 动态分配数组，存储每个段的方向信息

  // 动态分配二维数组 fLinkSeg，用于每个段的连接信息
  fLinkSeg = new int*[fN];  // 分配一维数组，包含 N 个指针
  for (int i = 0; i < fN; ++i)
    fLinkSeg[i] = new int[2];  // 为每个指针分配一个大小为 2 的整型数组

  fSizeSeg = new int[fN];  // 动态分配数组，存储每个段的大小信息

  // 动态分配二维数组 fCitySeg，用于存储城市在各个段中的位置
  fCitySeg = new int*[fN];  // 分配一维数组，包含 N 个指针
  for (int i = 0; i < fN; ++i)
    fCitySeg[i] = new int[2];  // 为每个指针分配一个大小为 2 的整型数组

  fT = new int[5];  // 动态分配一个大小为 5 的整型数组，用于临时存储或计算

  // 动态分配数组 fActiveV，用于标识活跃的城市或变量
  fActiveV = new int[fN];

  // 动态分配逆近邻列表，存储每个城市的逆近邻信息
  fInvNearList = new int*[fN];  // 分配一维数组，包含 N 个指针
  for (int i = 0; i < fN; ++i)
    fInvNearList[i] = new int[500];  // 为每个城市分配一个大小为 500 的整型数组

  fNumOfINL = new int[fN];  // 动态分配数组，存储每个城市逆近邻的数量

  fArray = new int[fN + 2];  // 动态分配数组，大小为 N + 2，用于中间计算信息

  fCheckN = new int[fN];  // 动态分配数组，存储检查数量的信息

  fB = new int[fN];  // 动态分配备用数组，通常用于临时存储数据

  fGene = new int[fN];  // 动态分配数组，用于表示路径或基因信息
}


TKopt::~TKopt()
{
  for( int i = 0; i < fN; ++i ) 
    delete [] fLink[ i ];
  delete [] fLink;

  for( int i = 0; i < fN; ++i ) 
    delete [] fLinkSeg[ i ];
  delete [] fLinkSeg;

  for( int i = 0; i < fN; ++i ) 
    delete [] fCitySeg[ i ];
  delete [] fCitySeg;

  for( int i = 0; i < fN; ++i ) 
    delete [] fInvNearList[ i ];
  delete [] fInvNearList;

  delete [] fOrdCity;
  delete [] fOrdSeg;
  delete [] fSegCity;
  delete [] fOrient;
  delete [] fSizeSeg;
  delete [] fT;
  delete [] fActiveV;
  delete [] fNumOfINL;
  delete [] fArray;
  delete [] fCheckN;
  delete [] fB;
  delete [] fGene;
}


void TKopt::SetInvNearList()
{
  assert(eval->fNearNumMax >= 50);  // 确保评估器中的最大邻居数量大于或等于 50

  int c;  // 定义一个整型变量 c，用于存储邻居城市的索引

  // 将每个城市的逆近邻数量初始化为 0
  for (int i = 0; i < fN; ++i)
    fNumOfINL[i] = 0;

  // 遍历每个城市
  for (int i = 0; i < fN; ++i) {
    // 处理每个城市的前 50 个近邻城市
    for (int k = 0; k < 50; ++k) {
      c = eval->fNearCity[i][k];  // 获取城市 i 的第 k 个近邻城市的索引
      // 如果逆近邻的数量小于 500，则将城市 i 添加到城市 c 的逆近邻列表中
      if (fNumOfINL[c] < 500)
        fInvNearList[c][fNumOfINL[c]++] = i;
      else {  // 如果逆近邻数量已经达到上限，则输出错误信息
        printf("Check fNumOfINL[c] < 500 ) in kopt.cpp \n");
        fflush(stdout);  // 确保上面的信息立即输出到控制台
      }
    }
  }
}



void TKopt::TransIndiToTree(TIndi& indi)
{
  // 定义变量
  int num;          // 用于跟踪当前处理到的城市索引
  int size;         // 用于存储当前段的城市数量
  int orient;       // 段的方向标识
/* farray其实就是城市的连接顺序，一个数组 */
  fArray[1] = 0;   // 将 fArray 的第一个有效城市设置为 0（起始城市）
  // 根据 indi 中的连接信息填充 fArray
  for (int i = 2; i <= fN; ++i)
    fArray[i] = indi.fLink[fArray[i-1]][1];
  fArray[0] = fArray[fN];  // 让 fArray[0] 指向最后一个城市
  fArray[fN + 1] = fArray[1];  // 让 fArray 的最后一个元素指向第一个城市（闭合路径）

  num = 1;  // 初始化城市索引，从1开始
  fNumOfSeg = 0;  // 初始化段的数量为 0

  while (1) { // 开始循环处理每个段
    orient = 1;  // 初始化当前段的方向为 1
    size = 0;    // 当前段的城市数量初始化为 0

    fOrient[fNumOfSeg] = orient;       // 记录当前段的方向
    fOrdSeg[fNumOfSeg] = fNumOfSeg;    // 记录当前段的序号

    // 设置当前城市的前后连接
    fLink[fArray[num]][0] = -1;        // 当前城市的前连接设为 -1，表示没有前驱
    fLink[fArray[num]][1] = fArray[num + 1]; // 当前城市的后连接设为下一个城市
    fOrdCity[fArray[num]] = size;      // 记录当前城市在该段中的顺序
    fSegCity[fArray[num]] = fNumOfSeg; // 记录当前城市所属的段
    fCitySeg[fNumOfSeg][this->Turn(orient)] = fArray[num];  // 将当前城市添加到段中
    ++num;   // 移动到下一个城市
    ++size;  // 当前段的城市数量增加

    // 处理同一段内的后续城市
    for (int i = 0; i < (int)sqrt(fN) - 1; ++i) {
      if (num == fN) // 如果已经处理完所有城市，跳出循环
        break;
      // 为当前城市设置连接
      fLink[fArray[num]][0] = fArray[num - 1];  // 当前城市的前连接
      fLink[fArray[num]][1] = fArray[num + 1];  // 当前城市的后连接
      fOrdCity[fArray[num]] = size;              // 更新当前城市在段中的顺序
      fSegCity[fArray[num]] = fNumOfSeg;         // 更新当前城市所属段的信息
      ++num;  // 处理下一个城市
      ++size; // 段内的城市数量增加
    }

    // 检查并处理倒数第二个城市
    if (num == fN - 1) {
      fLink[fArray[num]][0] = fArray[num - 1]; // 为倒数第二个城市设置前连接
      fLink[fArray[num]][1] = fArray[num + 1]; // 为倒数第二个城市设置后连接
      fOrdCity[fArray[num]] = size;            // 更新该城市在段中的顺序
      fSegCity[fArray[num]] = fNumOfSeg;       // 更新该城市所属段的信息
      ++num;  // 处理下一个城市
      ++size; // 增加段内城市数量
    }

    // 处理最后一个城市，将后连接设为 -1
    fLink[fArray[num]][0] = fArray[num - 1]; // 为最后一个城市设置前连接
    fLink[fArray[num]][1] = -1;               // 最后一个城市的后连接设为 -1，表示没有后继
    fOrdCity[fArray[num]] = size;             // 更新该城市在段中的顺序
    fSegCity[fArray[num]] = fNumOfSeg;        // 更新该城市所属段的信息
    fCitySeg[fNumOfSeg][orient] = fArray[num]; // 将当前城市加入段
    ++num;  // 移动到下一个城市
    ++size; // 当前段城市数量增加

    fSizeSeg[fNumOfSeg] = size;  // 记录当前段的城市数量
    ++fNumOfSeg;  // 段的数量增加

    if (num == fN + 1)  // 如果处理完所有城市，跳出循环
      break;
  }

  // 连接段与段之间
  for (int s = 1; s < fNumOfSeg - 1; ++s) {
    fLinkSeg[s][0] = s - 1;  // 当前段的前一个段索引
    fLinkSeg[s][1] = s + 1;  // 当前段的后一个段索引
  }
  fLinkSeg[0][0] = fNumOfSeg - 1;  // 第一个段的前连接指向最后一个段（环形结构）
  fLinkSeg[0][1] = 1;              // 第一个段的后连接指向第二个段
  fLinkSeg[fNumOfSeg - 1][0] = fNumOfSeg - 2; // 最后一个段的前连接
  fLinkSeg[fNumOfSeg - 1][1] = 0;  // 最后一个段的后连接指向第一个段

  fTourLength = indi.fEvaluationValue;  // 记录个体的评价值（路径长度）
  fFixNumOfSeg = fNumOfSeg;              // 固定段的数量
}


void TKopt::TransTreeToIndi(TIndi& indi)
{
    // 定义临时变量，用于存储前节点和后节点的索引
    int t_p, t_n;

    // 遍历所有城市（节点）
    for (int t = 0; t < fN; ++t)
    {
        // 获取当前城市（节点）t 的前一个城市的索引
        t_p = this->GetPrev(t);

        // 获取当前城市（节点）t 的后一个城市的索引
        t_n = this->GetNext(t);

        // 将前一个城市的索引存储到个体（indi）的链接数组中
        indi.fLink[t][0] = t_p;

        // 将后一个城市的索引存储到个体（indi）的链接数组中
        indi.fLink[t][1] = t_n;
    }

    // 计算个体的评估值（例如路径长度），以便进一步优化
    eval->DoIt(indi);
}



void TKopt::DoIt( TIndi& tIndi )
{ 
  this->TransIndiToTree( tIndi );
  //  this->CheckDetail();           // Check
  //  this->CheckValid();            // Check
  this->Sub();
  this->TransTreeToIndi( tIndi );
}


void TKopt::Sub()
{
    // 初始化变量
    int t1_st;         // 存储初始城市的索引
    int t_next;        // 存储下一个城市的索引（暂时未使用）
    int dis1, dis2;    // 用于存储距离差值

    // 将所有城市标记为活动状态，fActiveV[t] = 1 表示城市 t 是活动的
    for (int t = 0; t < fN; ++t)
        fActiveV[t] = 1;

    // LLL1: 循环标签，用于重新开始当前城市的处理
LLL1:
    // 随机选择一个城市作为起始城市
    t1_st = rand() % fN;
    fT[1] = t1_st;  // 保存为 fT[1]，表示当前处理的城市

    // 开始循环，处理当前城市 t1 的逻辑
    while (1)   // t1's loop
    {
        // 获取当前城市 t1 的下一个城市并更新
        fT[1] = this->GetNext(fT[1]);

        // 如果下一个城市已经被标记为不活动，则跳出循环
        if (fActiveV[fT[1]] == 0)
            goto EEE;  // 跳转到 EEE 标签

        // 处理 t1 的前驱城市
        fFlagRev = 0;  // 设置标志为0，表示正在处理前驱情况
        fT[2] = this->GetPrev(fT[1]);  // 获取 t1 的前驱城市

        // 在 1 到 49 的范围内循环尝试寻找改进路径
        for (int num1 = 1; num1 < 50; ++num1)
        {
            fT[4] = eval->fNearCity[fT[1]][num1]; // 找到 t1 的第 num1 个邻近城市
            fT[3] = this->GetPrev(fT[4]);  // 获取 t4 的前驱城市
            // 计算当前城市到前驱城市的距离与 t1 到 t4 的距离之差
            dis1 = eval->Direct(fT[1], fT[2]) - eval->Direct(fT[1], fT[4]); // 计算距离差

            // 如果距离差大于 0，则存在优化空间
            if (dis1 > 0) {
                // 进一步计算 t3 到 t4 和 t3 到 t2 的距离差
                dis2 = dis1 + eval->Direct(fT[3], fT[4]) - eval->Direct(fT[3], fT[2]);  // 进行距离比较

                // 如果优化的距离差大于 0
                if (dis2 > 0) {
                    // 改进
                    this->IncrementImp(fFlagRev);

                    // 更新与 t1、t2、t3、t4 相关的城市标记为活动
                    for (int a = 1; a <= 4; ++a)
                        for (int k = 0; k < fNumOfINL[fT[a]]; ++k)
                            fActiveV[this->fInvNearList[fT[a]][k]] = 1;

                    // 重新开始 t1 的循环
                    goto LLL1;
                }
            }
            else break;  // 距离差小于等于0时退出当前循环
        }

        // 处理 t1 的后继城市
        fFlagRev = 1;  // 设置标志为1，表示正在处理后继情况
        fT[2] = this->GetNext(fT[1]);  // 获取 t1 的后继城市

        // 在 1 到 49 的范围内循环尝试寻找改进路径
        for (int num1 = 1; num1 < 50; ++num1)
        {
            fT[4] = eval->fNearCity[fT[1]][num1]; // 找到 t1 的第 num1 个邻近城市
            fT[3] = this->GetNext(fT[4]);  // 获取 t4 的后继城市
            // 计算当前城市到后继城市的距离与 t1 到 t4 的距离之差
            dis1 = eval->Direct(fT[1], fT[2]) - eval->Direct(fT[1], fT[4]); // 计算距离差

            // 如果距离差大于 0，则存在优化空间
            if (dis1 > 0) {
                // 进一步计算 t3 到 t4 和 t3 到 t2 的距离差
                dis2 = dis1 + eval->Direct(fT[3], fT[4]) - eval->Direct(fT[3], fT[2]);  // 进行距离比较

                // 如果优化的距离差大于 0
                if (dis2 > 0) {
                    // 改进
                    this->IncrementImp(fFlagRev);

                    // 更新与 t1、t2、t3、t4 相关的城市标记为活动
                    for (int a = 1; a <= 4; ++a)
                        for (int k = 0; k < fNumOfINL[fT[a]]; ++k)
                            fActiveV[this->fInvNearList[fT[a]][k]] = 1;

                    // 重新开始 t1 的循环
                    goto LLL1;
                }
            }
            else break;  // 距离差小于等于0时退出当前循环
        }

        // 标记当前城市 t1 为不活动
        fActiveV[fT[1]] = 0;
EEE:;  // EEE 标签，用于跳出循环

        // 如果当前城市再次回到初始城市，则跳出外层循环
        if (fT[1] == t1_st)
            break;
    }
}



int TKopt::GetNext( int t ) {
  int t_n, seg, orient;

  // 获取当前点 t 所在的路径段
  seg = fSegCity[ t ];

  // 获取该路径段的方向
  orient = fOrient[ seg ];

  // 从当前点 t 出发，沿着路径方向获取链接的下一个节点
  t_n = fLink[ t ][ orient ];

  // 如果链接的下一个节点是无效的（即为 -1）
  if( t_n == -1 ) {
    // 移动到当前段的下一个段（遵循段的连接）
    seg = fLinkSeg[ seg ][ orient ];

    // 获取并反转该新段的方向
    orient = Turn( fOrient[ seg ] );

    // 从新的方向获取该段的一个城市，作为下一步的节点
    t_n = fCitySeg[ seg ][ orient ];
  }

  // 返回得到的下一个节点
  return t_n;
}



int TKopt::GetPrev( int t ) {
  int t_p, seg, orient;

  // 获取当前点 t 所在的路径段
  seg = fSegCity[ t ];

  // 获取该路径段的方向
  orient = fOrient[ seg ];

  // 从当前点 t 出发，沿着反方向获取链接的前一个节点
  t_p = fLink[ t ][ this->Turn( orient ) ];

  // 如果链接的前一个节点是无效的（即为 -1）
  if( t_p == -1 ) {
    // 移动到当前段的前一个段（遵循段的连接的反方向）
    seg = fLinkSeg[ seg ][ Turn(orient) ];

    // 在前一个段中获取直接的方向
    orient = fOrient[ seg ];

    // 从该方向获取该段的一个城市，作为回溯的节点
    t_p = fCitySeg[ seg ][ orient ];
  }

  // 返回得到的前一个节点
  return t_p;
}


void TKopt::Swap(int &a,int &b)
{
  int s;
  s=a;
  a=b;
  b=s;
}

//判断是正向还是逆向
int TKopt::Turn( int &orient )
{
  assert( orient == 0 || orient == 1 );
  if( orient == 0 )
    return 1;
  else if( orient == 1 )
    return 0;
  else 
    assert( 1 == 2 );
}

void TKopt::IncrementImp( int flagRev )
{
  int t1_s, t1_e, t2_s, t2_e;  // 存储两个路径的起始和结束点的索引。
  int seg_t1_s, seg_t1_e, seg_t2_s, seg_t2_e;  // 存储每个点所在的路径片段的索引。
  int ordSeg_t1_s, ordSeg_t1_e, ordSeg_t2_s, ordSeg_t2_e;  // 存储每个路径片段的顺序（或优先级、标识）。
  int orient_t1_s, orient_t1_e, orient_t2_s, orient_t2_e;  // 存储每个路径片段的方向信息。
  int numOfSeg1, numOfSeg2;  // 存储第一和第二个路径的片段数目。
  int curr;  // 当前处理的点或片段索引。
  int ord;  // 可能用于存储某种顺序或次序信息。

  int flag_t2e_t1s;    // 标志，用于比较t2结束点和t1开始点。
  int flag_t2s_t1e;    // 标志，用于比较t2开始点和t1结束点。
  int length_t1s_seg;  // t1的开始点所在片段的长度。
  int length_t1e_seg;  // t1的结束点所在片段的长度。
  int seg;  // 用于一般目的的片段变量。

  // Seg1: b->d path
  // Seg2: c->a path

  // 确定路径点的顺序，基于fFlagRev标志。
  if( fFlagRev == 0 ){
    t1_s = fT[1];
    t1_e = fT[3];
    t2_s = fT[4];
    t2_e = fT[2];
  }
  else if( fFlagRev == 1 ){
    t1_s = fT[2];
    t1_e = fT[4];
    t2_s = fT[3];
    t2_e = fT[1];
  }

  // 获取t1和t2路径的起始和结束点所在的片段、顺序、方向信息。
  seg_t1_s = fSegCity[ t1_s ];  // 获取t1开始点所在的路径片段。
  ordSeg_t1_s = fOrdSeg[ seg_t1_s ];  // 获取该片段的序号或顺序。
  orient_t1_s = fOrient[ seg_t1_s ];  // 获取该片段的方向。

  seg_t1_e = fSegCity[ t1_e ];// 获取t1结束点所在的路径片段。
  ordSeg_t1_e = fOrdSeg[ seg_t1_e ];  // 获取该片段的序号或顺序。
  orient_t1_e = fOrient[ seg_t1_e ];  // 获取该片段的方向。

  seg_t2_s = fSegCity[ t2_s ];  // 获取t2开始点所在的路径片段。
  ordSeg_t2_s = fOrdSeg[ seg_t2_s ];  // 获取该片段的序号或顺序。
  orient_t2_s = fOrient[ seg_t2_s ];  // 获取该片段的方向。

  seg_t2_e = fSegCity[ t2_e ];  // 获取t2结束点所在的路径片段。
  ordSeg_t2_e = fOrdSeg[ seg_t2_e ];  // 获取该片段的序号或顺序。
  orient_t2_e = fOrient[ seg_t2_e ];  // 获取该片段的方向。
  
  //////////////////// Type1 ////////////////////////
 if( ( seg_t1_s == seg_t1_e ) && ( seg_t1_s == seg_t2_s ) && ( seg_t1_s == seg_t2_e ) ) {
  // 检查条件：t1的起点和终点，以及t2的起点和终点都在同一个段上。
  // 这是一个特殊的情况，说明所有相关点在同一段路径上。

   if( (fOrient[seg_t1_s] == 1 && (fOrdCity[ t1_s ] > fOrdCity[ t1_e ])) ||
      (fOrient[seg_t1_s] == 0 && (fOrdCity[ t1_s ] < fOrdCity[ t1_e ]))) {

    // 检查段的方向，以及起点和终点的顺序。
    // 如果路径方向和节点顺序不匹配，则交换这些城市和段的信息。

    this->Swap( t1_s, t2_s );
    this->Swap( t1_e, t2_e );
    this->Swap( seg_t1_s, seg_t2_s );
    this->Swap( seg_t1_e, seg_t2_e );
    this->Swap( ordSeg_t1_s, ordSeg_t2_s );
    this->Swap( ordSeg_t1_e, ordSeg_t2_e );
    this->Swap( orient_t1_s, orient_t2_s );
    this->Swap( orient_t1_e, orient_t2_e );
   }

  // 初始化当前城市为t1的起点，并将路径的终点顺序存储到ord
  curr = t1_s;
  ord = fOrdCity[ t1_e ];

  // 循环交换当前城市在路径中的连接，并更新顺序
  while(1) {
    this->Swap( fLink[curr][0], fLink[curr][1] );  // 交换当前城市的连接
    fOrdCity[ curr ] = ord;  // 更新当前城市的顺序编号
    if( curr == t1_e )
      break;  // 如果当前城市已是终点，退出循环
    curr = fLink[curr][Turn(orient_t1_s)];  // 沿着路径方向更新当前城市
    if( orient_t1_s == 0 )
      ++ord;  // 如果方向标志为0，顺序递增
    else
      --ord;  // 如果方向标志为1，顺序递减
  }

  // 重新设置路径中的连接以完成环路的闭合
  fLink[t2_e][orient_t1_s] = t1_e;  // 设置t2终点的连接
  fLink[t2_s][Turn(orient_t1_s)] = t1_s;  // 设置t2起点的反向连接
  fLink[t1_s][orient_t1_s] = t2_s;  // 在t1起点接入t2起点
  fLink[t1_e][Turn(orient_t1_s)] = t2_e;  // 在t1终点接入t2终点

  // 这些检查函数通常用于调试或验证路径的有效性。
  // this->CheckDetail();              // 检查详细信息
  // this->CheckValid();               // 验证有效性

  return; // 完成Type 1 的逻辑处理并退出函数
  }

  //////////////////// Type1 ///////////////////////


 if( ordSeg_t1_e >= ordSeg_t1_s )
  numOfSeg1 = ordSeg_t1_e - ordSeg_t1_s + 1;
else
  numOfSeg1 = ordSeg_t1_e - ordSeg_t1_s + 1 + fNumOfSeg;

// 计算路径1的片段数量。如果片段的结束顺序号大于或等于开始顺序号，
// 片段数量就是两者之差加一。如果不是，说明路径跨越了末尾，
// 所以要加上总的片段数（fNumOfSeg）来获取实际的片段数量。

if( ordSeg_t2_e >= ordSeg_t2_s )
  numOfSeg2 = ordSeg_t2_e - ordSeg_t2_s + 1;
else
  numOfSeg2 = ordSeg_t2_e - ordSeg_t2_s + 1 + fNumOfSeg;

// 类似地计算路径2的片段数量，处理的方法与路径1相同。

if( numOfSeg1 > numOfSeg2 ){
  this->Swap( numOfSeg1, numOfSeg2 );
  this->Swap( t1_s, t2_s );
  this->Swap( t1_e, t2_e );
  this->Swap( seg_t1_s, seg_t2_s );
  this->Swap( seg_t1_e, seg_t2_e );
  this->Swap( ordSeg_t1_s, ordSeg_t2_s );
  this->Swap( ordSeg_t1_e, ordSeg_t2_e );
  this->Swap( orient_t1_s, orient_t2_s );
  this->Swap( orient_t1_e, orient_t2_e );
}

// 如果路径1的片段数量大于路径2，则交换两者的起点、终点、片段、顺序和方向。
// 这样确保在后续处理时，总是处理片段数量较少的路径。

if( fLink[ t2_e ][ orient_t2_e ] == -1 ) flag_t2e_t1s = 1;
else flag_t2e_t1s = 0;

// 设置一个标志（flag_t2e_t1s），用于指示路径2的终点是否连接到路径1的起点。
// 如果连接值为-1，意味着没有直接连接，设置标志为1。

if( fLink[ t2_s ][ this->Turn(orient_t2_s) ] == -1 ) flag_t2s_t1e = 1;
else flag_t2s_t1e = 0;

// 同样地，检查路径2的起点是否连接到路径1的终点。
// 如果连接不存在（即为-1），则设置标志为1。

length_t1s_seg = abs( fOrdCity[ t2_e ]
                      - fOrdCity[ fCitySeg[ seg_t2_e ][ orient_t2_e ] ] );

// 计算从路径2终点到其对应的段城市的路径“长度”。
// 本质上是计算顺序间的差值的绝对值。

length_t1e_seg = abs( fOrdCity[ t2_s ]
          - fOrdCity[ fCitySeg[ seg_t2_s ][ this->Turn(orient_t2_s) ] ] );

// 计算路径2起点与其对应段另一方向城市的路径“长度”。
// 同样，计算的是这两者的顺序差的绝对值。

  
  ///////////////////// Type2 ///////////////////
if( seg_t1_s == seg_t1_e ) {
  // 检查条件：路径1的起点和终点在同一个段上。
  // 以下代码处理这种特定的情况。

  if( flag_t2e_t1s == 1 && flag_t2s_t1e == 1 ) {
    // 条件：路径2的终点连接到路径1的起点，并且路径2的起点连接到路径1的终点。

    orient_t1_s = Turn( fOrient[ seg_t1_s ] );
    // 反转段的方向。

    fOrient[ seg_t1_s ] = orient_t1_s;
    // 更新段的方向。

    fCitySeg[ seg_t1_s ][ orient_t1_s ] = t1_s;
    // 更新城市段开始方向的城市。

    fCitySeg[ seg_t1_s ][ Turn(orient_t1_s) ] = t1_e;
    // 更新城市段另一方向的城市。

    fLinkSeg[ seg_t1_s ][ orient_t1_s ] = seg_t2_s;
    fLinkSeg[ seg_t1_s ][ Turn(orient_t1_s) ] = seg_t2_e;
    // 更新段连接，以反映路径2的段开始和结束。

    // this->CheckDetail();              // 验证详细信息（已注释掉）
    // this->CheckValid();               // 验证段的有效性（已注释掉）

    return; // 处理完毕，返回。
  }

  if( flag_t2e_t1s == 0 && flag_t2s_t1e == 1 ) {
    // 条件：路径2的终点不连接路径1的起点，但起点连接到路径1的终点。

    curr = t1_e;
    ord = fOrdCity[ t1_s ];
    while(1) {
      this->Swap( fLink[curr][0], fLink[curr][1] );
      // 交换当前城市的链接。
      fOrdCity[ curr ] = ord;
      // 更新当前城市的顺序编号。
      if( curr == t1_s )
        break; // 如果当前城市是起点，退出循环。
      curr = fLink[curr][orient_t2_e];
      // 根据路径方向更新当前城市。
      if( orient_t2_e == 0 )
        --ord; // 如果方向是0，顺序减少。
      else
        ++ord; // 如果方向是1，顺序增加。
    }

    fLink[t2_e][orient_t2_e] = t1_e;
    // 更新路径2终点的连接，使其指向路径1终点。

    fLink[t1_s][orient_t2_e] = -1;
    // 断开路径1起点的连接。

    fLink[t1_e][Turn(orient_t2_e)] = t2_e;
    // 使路径1终点连回路径2终点。

    fCitySeg[seg_t2_e][orient_t2_e] = t1_s;
    // 更新路径2段的城市信息。

    // this->CheckDetail();              // 验证详细（已注释掉）
    // this->CheckValid();               // 验证有效性（已注释掉）

    return; // 处理完毕，返回。
  }

  if( flag_t2e_t1s == 1 && flag_t2s_t1e == 0 ) {
    // 条件：路径2的终点连接到路径1的起点，但起点不连接路径1的终点。

    curr = t1_s;
    ord = fOrdCity[ t1_e ];
    while(1) {
      this->Swap( fLink[curr][0], fLink[curr][1] );
      // 交换当前城市的链接。
      fOrdCity[ curr ] = ord;
      // 更新当前城市的顺序编号。
      if( curr == t1_e )
        break; // 如果当前城市是终点，退出循环。
      curr = fLink[curr][Turn(orient_t2_s)];
      // 根据路径方向更新当前城市。
      if( orient_t2_s == 0 )
        ++ord; // 如果方向是0，顺序增加。
      else
        --ord; // 如果方向是1，顺序减少。
    }

    fLink[t2_s][Turn(orient_t2_s)] = t1_s;
    // 更新路径2起点的反向链接，使其指向路径1起点。

    fLink[t1_e][Turn(orient_t2_s)] = -1;
    // 断开路径1终点的连接。

    fLink[t1_s][orient_t2_s] = t2_s;
    // 使路径1起点连回路径2起点。

    fCitySeg[seg_t2_s][Turn(orient_t2_s)] = t1_e;
    // 更新路径2段的城市信息。

    // this->CheckDetail();              // 验证详细（已注释掉）
    // this->CheckValid();               // 验证有效性（已注释掉）

    return; // 处理完毕，返回。
  }
}
  ///////////////////// Type3 ///////////////////

if( flag_t2e_t1s == 1 ){
  fLinkSeg[seg_t1_s][Turn(orient_t1_s)] = seg_t2_s;
}
// 如果路径2的终点连接到路径1的起点，直接更新路径1起点段的连接。

else {
  seg_t1_s = fNumOfSeg++;
  // 分配一个新的段ID给seg_t1_s，并将总段数增加。

  orient_t1_s = orient_t2_e;
  // 将路径1起点段的方向设置为路径2终点的方向。

  fLink[ t1_s ][Turn(orient_t1_s)] = -1;
  fLink[ fCitySeg[seg_t2_e][orient_t2_e]][orient_t1_s] = -1;
  // 清除与路径1起点及其对应城市段的连接。

  fOrient[seg_t1_s] = orient_t1_s;
  fSizeSeg[seg_t1_s] = length_t1s_seg;
  // 更新新段的方向和大小信息。

  fCitySeg[seg_t1_s][Turn(orient_t1_s)] = t1_s;
  fCitySeg[seg_t1_s][orient_t1_s] = fCitySeg[seg_t2_e][orient_t2_e];
  // 设置新增段在两个方向上的城市信息。

  fLinkSeg[seg_t1_s][Turn(orient_t1_s)] = seg_t2_s;
  fLinkSeg[seg_t1_s][orient_t1_s] = fLinkSeg[seg_t2_e][orient_t2_e];
  // 设置新增段在两个方向上的连接信息。

  seg = fLinkSeg[seg_t2_e][orient_t2_e];
  fLinkSeg[seg][Turn(fOrient[seg])] = seg_t1_s;
  // 更新处理下一个相关段的连接。
}

if( flag_t2s_t1e == 1 ){
  fLinkSeg[seg_t1_e][orient_t1_e] = seg_t2_e;
}
// 如果路径2的起点连接到路径1的终点，直接连接路径1终点段与路径2终点段。

else {
  seg_t1_e = fNumOfSeg++;
  orient_t1_e = orient_t2_s;
  // 分配新的段ID给路径1终点，并设置其方向。

  fLink[ t1_e ][orient_t1_e] = -1;
  fLink[ fCitySeg[seg_t2_s][Turn(orient_t2_s)] ][Turn(orient_t1_e)] = -1;
  // 清除路径1终点及其对应城市段的连接。

  fOrient[seg_t1_e] = orient_t1_e;
  fSizeSeg[seg_t1_e] = length_t1e_seg;
  // 更新段方向和其大小。

  fCitySeg[seg_t1_e][orient_t1_e] = t1_e;
  fCitySeg[seg_t1_e][Turn(orient_t1_e)] = fCitySeg[seg_t2_s][Turn(orient_t2_s)];
  // 设置新增段在两个方向上的城市。

  fLinkSeg[seg_t1_e][orient_t1_e] = seg_t2_e;
  fLinkSeg[seg_t1_e][Turn(orient_t1_e)] = fLinkSeg[seg_t2_s][Turn(orient_t2_s)];
  // 更新新增段在两个方向上的连接。

  seg = fLinkSeg[seg_t2_s][Turn(orient_t2_s)];
  fLinkSeg[seg][fOrient[seg]] = seg_t1_e;
  // 更新顺序到下一个段的连接。
}

fLink[t2_e][orient_t2_e] = -1;
fSizeSeg[seg_t2_e] -= length_t1s_seg;
fCitySeg[seg_t2_e][orient_t2_e] = t2_e;
fLinkSeg[seg_t2_e][orient_t2_e] = seg_t1_e;
// 更新路径2终点段的信息，断开某些连接，并更新段大小。

fLink[t2_s][Turn(orient_t2_s)] = -1;
fSizeSeg[seg_t2_s] -= length_t1e_seg;
fCitySeg[seg_t2_s][Turn(orient_t2_s)] = t2_s;
fLinkSeg[seg_t2_s][Turn(orient_t2_s)] = seg_t1_s;
// 更新路径2起点段的信息，类似于路径2终点。

seg = seg_t1_e;
while(1)
{
  fOrient[seg] = Turn(fOrient[seg]);
  // 反转当前段的方向。
  if( seg == seg_t1_s )
    break;
  seg = fLinkSeg[seg][fOrient[seg]];
  // 移动到下一个段。
}

if( fSizeSeg[seg_t2_e] < length_t1s_seg )
{
  seg = fLinkSeg[seg_t2_e][Turn(fOrient[seg_t2_e])];
  fLinkSeg[seg][fOrient[seg]] = seg_t1_s;
  // 如果路径2终点段大小小于路径1起点段，交换其连接。

  seg = fLinkSeg[seg_t2_e][fOrient[seg_t2_e]];
  fLinkSeg[seg][Turn(fOrient[seg])] = seg_t1_s;
  seg = fLinkSeg[seg_t1_s][Turn(fOrient[seg_t1_s])];
  fLinkSeg[seg][fOrient[seg]] = seg_t2_e;
  seg = fLinkSeg[seg_t1_s][fOrient[seg_t1_s]];
  fLinkSeg[seg][Turn(fOrient[seg])] = seg_t2_e;

  this->Swap( fOrient[seg_t2_e], fOrient[seg_t1_s] );
  this->Swap( fSizeSeg[seg_t2_e], fSizeSeg[seg_t1_s] );
  this->Swap( fCitySeg[seg_t2_e][0], fCitySeg[seg_t1_s][0] );
  this->Swap( fCitySeg[seg_t2_e][1], fCitySeg[seg_t1_s][1] );
  this->Swap( fLinkSeg[seg_t2_e][0], fLinkSeg[seg_t1_s][0] );
  this->Swap( fLinkSeg[seg_t2_e][1], fLinkSeg[seg_t1_s][1] );
  this->Swap( seg_t2_e, seg_t1_s );
  // 交换方向、大小、城市信息，进行段间的交换处理。
}

if( fSizeSeg[seg_t2_s] < length_t1e_seg )
{
  seg = fLinkSeg[seg_t2_s][Turn(fOrient[seg_t2_s])];
  fLinkSeg[seg][fOrient[seg]] = seg_t1_e;
  // 类似处理，如果路径2起点段小于路径1终点段，进行交换。

  seg = fLinkSeg[seg_t2_s][fOrient[seg_t2_s]];
  fLinkSeg[seg][Turn(fOrient[seg])] = seg_t1_e;
  seg = fLinkSeg[seg_t1_e][Turn(fOrient[seg_t1_e])];
  fLinkSeg[seg][fOrient[seg]] = seg_t2_s;
  seg = fLinkSeg[seg_t1_e][fOrient[seg_t1_e]];
  fLinkSeg[seg][Turn(fOrient[seg])] = seg_t2_s;

  this->Swap( fOrient[seg_t2_s], fOrient[seg_t1_e] );
  this->Swap( fSizeSeg[seg_t2_s], fSizeSeg[seg_t1_e] );
  this->Swap( fCitySeg[seg_t2_s][0], fCitySeg[seg_t1_e][0] );
  this->Swap( fCitySeg[seg_t2_s][1], fCitySeg[seg_t1_e][1] );
  this->Swap( fLinkSeg[seg_t2_s][0], fLinkSeg[seg_t1_e][0] );
  this->Swap( fLinkSeg[seg_t2_s][1], fLinkSeg[seg_t1_e][1] );
  this->Swap( seg_t2_s, seg_t1_e );
  // 更新方向、大小和城市信息，处理段的交换。
}

while( fNumOfSeg > fFixNumOfSeg )
{
  if( fSizeSeg[ fLinkSeg[fNumOfSeg-1][0] ] <
      fSizeSeg[ fLinkSeg[fNumOfSeg-1][1] ] )
    this->CombineSeg( fLinkSeg[fNumOfSeg-1][0], fNumOfSeg-1 );
  else
    this->CombineSeg( fLinkSeg[fNumOfSeg-1][1], fNumOfSeg-1 );
}
// 合并多余的段，保持段数不超过固定值(fFixNumOfSeg)，选择小的一侧合并。

int ordSeg = 0;
seg = 0;

while(1)
{
  fOrdSeg[seg] = ordSeg;
  ++ordSeg;
  // 更新段序号以维持顺序。

  seg = fLinkSeg[seg][ fOrient[seg] ];
  if( seg == 0 )
    break;
}
// 为所有段重新标记顺序，以确保一致性。

// this->CheckDetail();              // 核对详细信息（已注释）
// this->CheckValid();               // 验证段的有效性（已注释）
return; // 处理完毕，返回。

}


void TKopt::CombineSeg( int segL, int segS )
{
  int seg;
  int t_s, t_e, direction; t_s = 0; t_e = 0; direction = 0;
  int ord; ord = 0;
  int increment; increment = 0;
  int curr, next;

  if( fLinkSeg[segL][fOrient[segL]] == segS ){ 
    fLink[fCitySeg[segL][fOrient[segL]]][fOrient[segL]] = 
    fCitySeg[segS][Turn(fOrient[segS])]; 

    fLink[fCitySeg[segS][Turn(fOrient[segS])]][Turn(fOrient[segS])] = fCitySeg[segL][fOrient[segL]];
    ord = fOrdCity[fCitySeg[segL][fOrient[segL]]]; 

    fCitySeg[segL][fOrient[segL]] = fCitySeg[segS][fOrient[segS]]; 
    fLinkSeg[segL][fOrient[segL]] = fLinkSeg[segS][fOrient[segS]]; 
    seg = fLinkSeg[segS][fOrient[segS]]; 
    fLinkSeg[seg][Turn(fOrient[seg])] = segL;

    t_s = fCitySeg[segS][Turn(fOrient[segS])];
    t_e = fCitySeg[segS][fOrient[segS]];
    direction = fOrient[segS];


    if( fOrient[segL] == 1 )
      increment = 1;
    else 
      increment = -1;
  }
  else if( fLinkSeg[segL][Turn(fOrient[segL])] == segS ){ 
    fLink[fCitySeg[segL][Turn(fOrient[segL])]][Turn(fOrient[segL])] = 
    fCitySeg[segS][fOrient[segS]]; 

    fLink[fCitySeg[segS][fOrient[segS]]][fOrient[segS]] = fCitySeg[segL][Turn(fOrient[segL])];
    ord = fOrdCity[fCitySeg[segL][Turn(fOrient[segL])]]; 

    fCitySeg[segL][Turn(fOrient[segL])] = fCitySeg[segS][Turn(fOrient[segS])]; 
    fLinkSeg[segL][Turn(fOrient[segL])] = fLinkSeg[segS][Turn(fOrient[segS])]; 
    seg = fLinkSeg[segS][Turn(fOrient[segS])]; 
    fLinkSeg[seg][fOrient[seg]] = segL;

    t_s = fCitySeg[segS][fOrient[segS]];
    t_e = fCitySeg[segS][Turn(fOrient[segS])];
    direction = Turn(fOrient[segS]);


    if( fOrient[segL] == 1 )
      increment = -1;
    else 
      increment = 1;
  }
  curr = t_s;
  ord = ord + increment;

  while(1)
  {
    fSegCity[curr] = segL;
    fOrdCity[curr] = ord;

    next = fLink[curr][direction];
    if( fOrient[segL] != fOrient[segS] )
      this->Swap( fLink[curr][0], fLink[curr][1] ); 

    if( curr == t_e )
      break;
    curr = next;
    ord += increment;
  }
  fSizeSeg[segL] += fSizeSeg[segS];
  --fNumOfSeg;
}



void TKopt::CheckDetail() {
  int seg, seg_p, seg_n;          // 当前段及其前后段
  int ord, ord_p, ord_n;          // 段的顺序及其前后顺序
  int orient;                     // 段的方向
  int curr;                       // 当前城市

  seg = 0;                        // 初始化段变量

  // 遍历所有段
  for( int s = 0; s < fNumOfSeg; ++s ){
    seg = s;                      // 当前段
    orient = fOrient[ seg ];      // 获取当前段的方向
    seg_p = fLinkSeg[ seg ][ this->Turn(orient) ]; // 获取前一段
    seg_n = fLinkSeg[ seg ][ orient ]; // 获取下一段

    ord = fOrdSeg[ seg ];        // 获取当前段的顺序
    ord_p = ord - 1;             // 前段顺序
    if( ord_p < 0 )
      ord_p = fNumOfSeg - 1;     // 保证顺序循环
    ord_n = ord + 1;             // 后段顺序
    if( ord_n >= fNumOfSeg )
      ord_n = 0;                 // 保证顺序循环

    // 验证前段和后段的顺序是否正确
    assert( ord_p == fOrdSeg[seg_p] );
    assert( ord_n == fOrdSeg[seg_n] );

    curr = fCitySeg[ s ][ 0 ];   // 从当前段的第一个城市开始
    int count = 0;                // 初始化计数器

    // 遍历当前段中的城市
    while(1) {
      ++count;                     // 增加计数
      if( curr == fCitySeg[ s ][1] ) // 如果到达当前段的最后一个城市
        break;
      curr = fLink[curr][1];      // 获取当前城市的下一个城市
      assert( curr != -1 );        // 确保下一个城市有效
    }
    // 验证当前段城市数是否正确
    assert( count == fSizeSeg[s] );
  }

  int t;                          // 当前城市
  int t_n, t_p, t_s, t_e;        // 当前城市的前后城市和两端城市

  // 遍历所有城市
  for( t = 0; t < fN; ++t ) {
    seg = fSegCity[ t ];         // 获取当前城市所在的段
    orient = fOrient[ seg ];      // 获取段的方向
    t_s = fCitySeg[ seg ][ 0 ];   // 当前段的起始城市
    t_e = fCitySeg[ seg ][ 1 ];   // 当前段的结束城市

    t_p = fLink[ t ][ 0 ];       // 当前城市的前一个城市
    t_n = fLink[ t ][ 1 ];       // 当前城市的下一个城市

    // 如果当前城市是起始城市
    if( t == t_s ){
      assert( t_p == -1 );       // 前一个城市应为无效链接
    }
    else{
      assert( t_p != -1 );       // 否则前一个城市应有效
      assert( t == fLink[ t_p ][ 1 ] ); // 当前城市应是前一个城市的下一个城市
      assert( seg == fSegCity[ t_p ] ); // 确保前一个城市属于同一段
      assert( fOrdCity[ t ] == fOrdCity[ t_p ] + 1 ); // 顺序检查
    }

    // 如果当前城市是结束城市
    if( t == t_e ){
      assert( t_n == -1 );       // 下一个城市应为无效链接
    }
    else{
      assert( t_n != -1 );       // 否则下一个城市应有效
      assert( t == fLink[ t_n ][ 0 ] ); // 当前城市应是下一个城市的前一个城市
      assert( seg == fSegCity[ t_n ] ); // 确保下一个城市属于同一段
      assert( fOrdCity[ t ] == fOrdCity[ t_n ] - 1 ); // 顺序检查
    }
  }
}



void TKopt::CheckValid() {
  int t_st, t_c, t_n, st;  // 用于临时存储当前城市、下一个城市和起始城市
  int count;               // 计数已访问的城市数量
  int seg, orient;         // 路径段和方向
  int Invalid = 0;         // 标志用于指示路径是否无效

  // 初始化检查数组 fCheckN，使所有城市都未被检查
  for( int i = 0; i < fN; ++i )
    fCheckN[ i ] = 0;

  // 从一个随机城市开始检查路径
  t_st = rand() % fN;  // 随机选择一个城市作为起始点
  t_n = t_st;          // 设置当前城市为起始城市

  count = 0;           // 初始化路径节点计数器
  while(1) {
    t_c = t_n;         // 记录当前城市
    fCheckN[ t_c ] = 1; // 标记当前节点为已检查
    ++count;           // 增加已检查城市的计数

    // 获取当前节点所在路径段和方向
    seg = fSegCity[ t_c ];
    orient = fOrient[ seg ];

    // 使用 GetNext 函数找到下一个路径节点
    t_n = this->GetNext( t_c );

    // 如果下一个节点是起始节点，说明路径形成一个完整的环
    if( t_n == t_st )
      break;

    // 如果计数超过城市总数加1，说明路径无效，设置 Invalid 标志
    if( count == fN+1 ) {
      Invalid = 1; // 标记路径为无效
      break;       // 跳出循环
    }
  }

  // 检查所有城市是否都被访问过
  for( int i = 0; i < fN; ++i )
    if( fCheckN[ i ] == 0 ) // 如果有城市未被访问
      Invalid = 1;          // 标记路径为无效


  // 如果路径无效，输出错误信息并触发断言
  if( Invalid == 1 ) {
    printf( "Invalid \n" );   // 打印无效提示
    fflush( stdout );         // 确保输出被刷新
    assert( 1 == 2 );         // 触发断言，表明程序错误
  }
}



void TKopt::MakeRandSol( TIndi& indi ) {
  int k, r; // k 用于循环控制（未使用），r 用于存储随机索引

  // 初始化数组 fB，存储城市索引（0 到 fN - 1）
  for( int j = 0; j < fN; ++j )
    fB[j] = j;

  // 生成随机顺序，用于生成个体的基因（路径）
  for( int i = 0; i < fN; ++i ) {
    r = rand() % (fN - i); // 在未选择的城市中生成一个随机索引
    fGene[i] = fB[r];      // 使用随机索引更新基因
    fB[r] = fB[fN - i - 1]; // 用最后一个未选择的城市替换
  }

  // 设置个体中的链接，构建路径的前后关系
  for( int j2 = 1; j2 < fN - 1; ++j2 ) {
    indi.fLink[fGene[j2]][0] = fGene[j2 - 1]; // 当前城市链接到前一个城市
    indi.fLink[fGene[j2]][1] = fGene[j2 + 1]; // 当前城市链接到后一个城市
  }

  // 特殊处理首尾城市，建立环形连接
  indi.fLink[fGene[0]][0] = fGene[fN - 1]; // 第一个城市链接到最后一个城市
  indi.fLink[fGene[0]][1] = fGene[1];       // 第一个城市链接到第二个城市
  indi.fLink[fGene[fN - 1]][0] = fGene[fN - 2]; // 最后一个城市链接到倒数第二个城市
  indi.fLink[fGene[fN - 1]][1] = fGene[0];       // 最后一个城市链接到第一个城市

  // 评估生成的个体，计算路径的适应度或成本
  eval->DoIt( indi );
}


