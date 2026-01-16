#ifndef __Cross__
#include "cross.h"
#endif


TCross::TCross( int N )
{
  fMaxNumOfABcycle = 6000; /* Set an appropriate value (6000 usually is enough) */

  fN = N;
  tBestTmp.Define( fN );

  near_data = new int* [ fN ];
  for ( int j = 0; j < fN; ++j ) 
    near_data[j] = new int [ 5 ];

  /*
  fABcycle = new int* [ fMaxNumOfABcycle ];
  for ( int j = 0; j < fMaxNumOfABcycle; ++j ) 
    fABcycle[j] = new int [ fN + 4 ];
  */
  fABcycleL = new int [ fN * 3 ];
  fPosi_ABL = new int [ fN * 3 ];

  koritsu = new int [ fN ];
  bunki = new int [ fN ];
  kori_inv = new int [ fN ];
  bun_inv = new int [ fN ];
  check_koritsu = new int [ fN ];
  fRoute = new int [ 2*fN + 1 ];
  fPermu = new int [ fMaxNumOfABcycle ];

  fC = new int [ 2*fN+4 ];
  fJun = new int[ fN+ 1 ];
  fOrd1 = new int [ fN ];
  fOrd2 = new int [ fN ];

  // Speed Up Start
  fOrder = new int [ fN ];
  fInv = new int [ fN ];
  fSegment = new int* [ fN ];
  for ( int j = 0; j < fN; ++j ) 
    fSegment[ j ] = new int [ 2 ];
  fSegUnit = new int [ fN ]; 
  fSegPosiList = new int[ fN ];
  LinkAPosi = new int [ fN ];
  LinkBPosi = new int* [ fN ];
  for ( int j = 0; j < fN; ++j ) 
    LinkBPosi[ j ] = new int [ 2 ];
  fPosiSeg = new int [ fN ];
  fNumOfElementInUnit = new int [ fN ]; 
  fCenterUnit = new int [ fN ]; 
  for ( int j = 0; j < fN; ++j ) 
    fCenterUnit[ j ] = 0;
  fListOfCenterUnit = new int [ fN+2 ]; 
  fSegForCenter = new int [ fN ]; 
  fGainAB = new int [ fN ]; 
  fModiEdge = new int* [ fN ]; 				 
  for ( int j = 0; j < fN; ++j ) 
    fModiEdge[ j ] = new int [ 4 ]; 				 
  fBestModiEdge = new int* [ fN ]; 				 
  for ( int j = 0; j < fN; ++j ) 
    fBestModiEdge[ j ] = new int [ 4 ]; 				 
  fAppliedCylce = new int [ fN ];
  fBestAppliedCylce = new int [ fN ];
  // Speed Up End

  // Block2
  fNumOfElementINAB = new int [ fMaxNumOfABcycle ];
  fInEffectNode = new int* [ fN ];
  for( int i = 0; i < fN; ++i )
    fInEffectNode[ i ] = new int [ 2 ];
  fWeight_RR = new int* [ fMaxNumOfABcycle ];
  for( int i = 0; i < fMaxNumOfABcycle; ++i )
    fWeight_RR[ i ] = new int [ fMaxNumOfABcycle ];
  fWeight_SR = new int [ fMaxNumOfABcycle ];
  fWeight_C = new int [ fMaxNumOfABcycle ];
  fUsedAB = new int [ fN ];
  fMoved_AB = new int [ fN ];
  fABcycleInEset = new int [ fMaxNumOfABcycle ];

  /*
  fABcycleOrd = new int** [ fMaxNumOfABcycle ];          // Large
  for ( int j = 0; j < fMaxNumOfABcycle; ++j ){ 
    fABcycleOrd[ j ] = new int* [ fN + 4 ];
    for ( int j2 = 0; j2 < fN+4; ++j2 ) 
      fABcycleOrd[j][j2] = new int [ 2 ];
  }
  */
  fABcycleLOrd = new int* [ fN * 3 ];          // Large
  for ( int j = 0; j < fN * 3; ++j ){ 
    fABcycleLOrd[ j ] = new int [ 2 ];
  }

  fModiEdgeOrd = new int** [ fN ];          // Large
  for ( int j = 0; j < fN; ++j ){ 
    fModiEdgeOrd[ j ] = new int* [ 4 ];
    for ( int j2 = 0; j2 < 4; ++j2 ) 
      fModiEdgeOrd[j][j2] = new int [ 2 ];
  }

  fBestModiEdgeOrd = new int** [ fN ];          // Large
  for ( int j = 0; j < fN; ++j ){ 
    fBestModiEdgeOrd[ j ] = new int* [ 4 ];
    for ( int j2 = 0; j2 < 4; ++j2 ) 
      fBestModiEdgeOrd[j][j2] = new int [ 2 ];
  }
}

TCross::~TCross()
{
  delete [] koritsu;
  delete [] bunki;
  delete [] kori_inv;
  delete [] bun_inv;
  delete [] check_koritsu;
  delete [] fRoute;
  delete [] fPermu;

  for ( int j = 0; j < fN; ++j ) 
    delete[] near_data[ j ];
  delete[] near_data;

  delete[] fABcycleL;
  delete[] fPosi_ABL;

  delete [] fC;
  delete [] fJun; 
  delete [] fOrd1; 
  delete [] fOrd2; 


  // Speed Up Start
  delete [] fOrder;
  delete [] fInv;

  for ( int j = 0; j < fN; ++j ) 
    delete[] fSegment[ j ];
  delete[] fSegment;
  delete[] fSegUnit;
  delete [] fSegPosiList;
  delete [] LinkAPosi;
  for ( int j = 0; j < fN; ++j ) 
    delete[] LinkBPosi[ j ];
  delete [] LinkBPosi;
  delete [] fPosiSeg;
  delete [] fNumOfElementInUnit; 
  delete [] fCenterUnit;
  delete [] fListOfCenterUnit;
  delete [] fSegForCenter;
  delete [] fGainAB;

  for ( int j = 0; j < fN; ++j ) 
    delete[] fModiEdge[ j ];
  delete [] fModiEdge;
  for ( int j = 0; j < fN; ++j ) 
    delete[] fBestModiEdge[ j ];
  delete [] fBestModiEdge;
  delete [] fAppliedCylce;
  delete [] fBestAppliedCylce;
  // Speed Up End

  // Block2
  delete [] fNumOfElementINAB;
  for ( int j = 0; j < fN; ++j ) 
    delete [] fInEffectNode[ j ];
  delete [] fInEffectNode;
  for( int i = 0; i < fMaxNumOfABcycle; ++i )
    delete [] fWeight_RR[ i ];
  delete [] fWeight_SR;
  delete [] fWeight_C;
  delete [] fUsedAB;
  delete [] fMoved_AB;
  delete [] fABcycleInEset;
  delete [] fNumOfElementINAB;


  for ( int j = 0; j < fN * 3 ; ++j )  // Large
    delete[] fABcycleLOrd[ j ];
  delete[] fABcycleLOrd;

  for ( int j = 0; j < fN; ++j ){   // Large
    for ( int j2 = 0; j2 < 4; ++j2 ){ 
      delete[] fModiEdgeOrd[ j ][ j2 ];
    }
    delete [] fModiEdgeOrd[ j ];
  }
  delete[] fModiEdgeOrd;

  for ( int j = 0; j < fN; ++j ){   // Large
    for ( int j2 = 0; j2 < 4; ++j2 ){ 
      delete[] fBestModiEdgeOrd[ j ][ j2 ];
    }
    delete [] fBestModiEdgeOrd[ j ];
  }
  delete[] fBestModiEdgeOrd;
}


void TCross::SetParents(const TIndi& tPa1, const TIndi& tPa2, int flagC[10], int numOfKids)
{
  // 调用 SetABcycle 函数，生成 AB-cycles
  this->SetABcycle(tPa1, tPa2, flagC, numOfKids);

  // 初始化 fDis_AB 为 0，表示两个父母个体之间的差异度
  fDis_AB = 0;

  // 定义控制遍历父代路径的变量
  int curr, next, st, pre;
  st = 0;       // 路径起始节点，通常选择节点 0 作为开始
  curr = -1;    // 当前节点初始化为无效值
  next = st;    // 下一个节点初始化为起点

  // 遍历整个节点序列，更新路径信息
  for (int i = 0; i < fN; ++i)
  {
    pre = curr;     // 记录前一个节点
    curr = next;    // 移动到下一个节点

    // 根据前一个节点信息，选择下一个节点
    if (tPa1.fLink[curr][0] != pre)
      next = tPa1.fLink[curr][0];  // 如果0号链接与前一个节点不同，选择作为下一个
    else
      next = tPa1.fLink[curr][1];  // 否则选择1号链接作为下一个

    // 检查当前节点在第二个父路径上的连接，如果不吻合，增加差异度计数
    if (tPa2.fLink[curr][0] != next && tPa2.fLink[curr][1] != next)
      ++fDis_AB;

    // 记录当前节点的顺序到 fOrder，以及逆序信息到 fInv
    fOrder[i] = curr;
    fInv[curr] = i;
  }

  // 确保路径形成一个完整的循环，即回到起始节点
  assert(next == st);

  // 当 flagC[1] == 2 时执行 Block2 中的操作
  if (flagC[1] == 2) {            /* Block2 */
    fTmax = 10;                   // 设置最大板块为 10（可能用于某种操作限制）
    fMaxStag = 20;                // 设置最大停滞为 20（1: 贪心本地搜索，20: 禁忌搜索）
    this->SetWeight(tPa1, tPa2);  // 调用 SetWeight 方法，估计父代权重差异
  }
}



void TCross::DoIt(TIndi& tKid, TIndi& tPa2, int numOfKids, int flagP, int flagC[10], int **fEdgeFreq) {
  int Num;  // 将要生成的子代数量
  int jnum, centerAB;  // 当前正在处理的 AB 循环编号
  int gain;  // 描述应用AB cycle后的收益
  int BestGain;  // 最佳收益
  double pointMax, point;  // 费用比较（最大收益的比例）
  double DLoss;  // 损失（距离损失或熵损失）

  // 设置评估和 E 集类型
  fEvalType = flagC[0];  /* 1:Greedy, 2:---, 3:---, 4:Entropy */
  fEsetType = flagC[1];  /* 1:Single-AB, 2:Block2 */

  if (fEvalType == 3) {
    printf("Distance preserving is not supported \n");
    exit(1); // 退出程序，因为不支持某种评估类型
  }
  assert(fEvalType == 1 || fEvalType == 4);  // 验证只支持贪婪和熵
  assert(fEsetType == 1 || fEsetType == 2);  // 验证 E 集的类型

  // 计算子代数量，最多生成 fNumOfABcycle（AB循环的总数） 个
  if (numOfKids <= fNumOfABcycle)
    Num = numOfKids;
  else
    Num = fNumOfABcycle;

  // 初始化 AB 循环序列 初始化fpermu
  if (fEsetType == 1) {  /* Single-AB */
    tRand->Permutation(fPermu, fNumOfABcycle, fNumOfABcycle);  // 生成一个排列，有funmofABcycle个元素，保存到fPermu中
  } else if (fEsetType == 2) {  /* Block2 */
    for (int k = 0; k < fNumOfABcycle; ++k)
      fNumOfElementINAB[k] = fABcycleL[fPosi_ABL[k] + 0];
    tSort->Index_B(fNumOfElementINAB, fNumOfABcycle, fPermu, fNumOfABcycle);
  }

  // 初始化操作
  fNumOfGeneratedCh = 0;
  pointMax = 0.0;
  BestGain = 0;
  fFlagImp = 0;

  // 对每个潜在的组合进行计算和尝试
  for (int j = 0; j < Num; ++j) {
    fNumOfABcycleInEset = 0;

    if (fEsetType == 1) {  /* Single-AB */
      jnum = fPermu[j];
      fABcycleInEset[fNumOfABcycleInEset++] = jnum;
    } else if (fEsetType == 2) {  /* Block2 */
      jnum = fPermu[j];
      centerAB = jnum;

      // 尝试生成多个 AB-cycles
      for (int s = 0; s < fNumOfABcycle; ++s) {
        if (s == centerAB)
          fABcycleInEset[fNumOfABcycleInEset++] = s;
        else {
          if (fWeight_RR[centerAB][s] > 0 &&
              fABcycleL[fPosi_ABL[s] + 0] < fABcycleL[fPosi_ABL[centerAB] + 0]) {
            if (rand() % 2 == 0)         // ABcycle与中心ABcycle有公共点并且长度较小（应该指点的个数更小）
              fABcycleInEset[fNumOfABcycleInEset++] = s;
          }
        }
      }
      this->Search_Eset(centerAB);
    }

    // 初始化单元和收益
    fNumOfSPL = 0;
    gain = 0;
    fNumOfAppliedCycle = 0;
    fNumOfModiEdge = 0;

    fNumOfAppliedCycle = fNumOfABcycleInEset;
    for (int k = 0; k < fNumOfAppliedCycle; ++k) {
      fAppliedCylce[k] = fABcycleInEset[k];
      jnum = fAppliedCylce[k];
      this->ChangeSol(tKid, jnum, flagP);  // 修改解决方案，这里是对父代1进行修改
      gain += fGainAB[jnum];  // 累加收益
    }

    this->MakeUnit();  // 得到图的联通分量
    this->MakeCompleteSol(tKid);  // 完成解的构建
    gain += fGainModi;  // 考虑到修正边后的总收益

    ++fNumOfGeneratedCh;  // 生成子代的数量增加

    // 根据不同的评估类型计算损失
    if (fEvalType == 1) {  /* Greedy */
      DLoss = 1.0;
    } else if (fEvalType == 4) {  /* Entropy preservation */
      DLoss = this->Cal_ENT_Loss(fEdgeFreq);
    }

    if (DLoss <= 0.0) DLoss = 0.00000001;

    // 计算得分，更新评估值
    point = (double)gain / DLoss;
    tKid.fEvaluationValue = tKid.fEvaluationValue - gain;

    // 更新最佳得分和应用情况
    if (pointMax < point && (2 * fBest_Num_E < fDis_AB ||
                             tKid.fEvaluationValue != tPa2.fEvaluationValue)) {
      pointMax = point;
      BestGain = gain;
      fFlagImp = 1;

      fNumOfBestAppliedCycle = fNumOfAppliedCycle;
      for (int s = 0; s < fNumOfBestAppliedCycle; ++s)
        fBestAppliedCylce[s] = fAppliedCylce[s];

      fNumOfBestModiEdge = fNumOfModiEdge;
      for (int s = 0; s < fNumOfBestModiEdge; ++s) {
        for (int m = 0; m < 4; ++m) {
          fBestModiEdge[s][m] = fModiEdge[s][m];
          fBestModiEdgeOrd[s][m][0] = fModiEdgeOrd[s][m][0]; // Large
          fBestModiEdgeOrd[s][m][1] = fModiEdgeOrd[s][m][1]; // Large
        }
      }
    }

    this->BackToPa1(tKid);  // 回滚到父代状态
    tKid.fEvaluationValue = tKid.fEvaluationValue + gain;
  }

  // 如果找到最佳情况，则应用
  if (fFlagImp == 1) {
    this->GoToBest(tKid);  // 转到最佳解
    tKid.fEvaluationValue = tKid.fEvaluationValue - BestGain;
    this->IncrementEdgeFreq(fEdgeFreq);  // 增加边频率的统计量
  }
}



void TCross::SetABcycle(const TIndi& tPa1, const TIndi& tPa2, int flagC[10], int numOfKids)
{
    fSPosi_ABL = 0;  // 初始化 AB循环起始位置的索引
    // 初始化节点状态和数据结构
    bunki_many = 0;
    koritsu_many = 0;
    for (int j = 0; j < fN; ++j)
    {
        // 初始化 near_data 中的父代连接关系
        near_data[j][1] = tPa1.fLink[j][0];
        near_data[j][3] = tPa1.fLink[j][1];
        near_data[j][0] = 2; // 表示此节点有两个连接

        // 添加所有节点到 koritsu 列表
        koritsu[koritsu_many] = j;
        koritsu_many++;

        // 初始化第二个父代的连接关系
        near_data[j][2] = tPa2.fLink[j][0];
        near_data[j][4] = tPa2.fLink[j][1];
    }

    // 初始化检查和索引逆序映射
    for(int j = 0; j < fN; ++j)
    {
        check_koritsu[j] = -1;
        kori_inv[koritsu[j]] = j;
    }

    // 初始化 AB 循环数量
    fNumOfABcycle = 0;
    flag_st = 1;  // 起始标志

    // 主循环：孤立点数量不为0
    while(koritsu_many != 0)
    {
        if(flag_st == 1) // 如果刚开始，选择一个新的起点
        {
            fPosiCurr = 0;  //当前是第几个节点，第一个
            r = rand() % koritsu_many;   //随机找一个节点作为起点，这里给的是序号
            st = koritsu[r];        // 将城市列表中第r个点作为起点
            check_koritsu[st] = fPosiCurr;
            fRoute[fPosiCurr] = st;
            ci = st;   // current iterm 当前点
            pr_type = 2; // 设置连接检查类型（初始为 2）
        }
        else if(flag_st == 0) // 如果不是刚开始，继续已有路径
        {
            ci = fRoute[fPosiCurr];
        }

        flag_circle = 0; // 初始化循环标志
        while(flag_circle == 0) // 找到终点
        {
            fPosiCurr++;
            pr = ci;

            // 基于前一连接类型获取当前节点的下一个节点
            switch(pr_type)
            {
                case 1:
                    ci = near_data[pr][fPosiCurr % 2 + 1];  // 找父代2中与pr连接的前一个节点
                    break;
                case 2:
                    r = rand() % 2;
                    ci = near_data[pr][fPosiCurr % 2 + 1 + 2 * r];  // 交替在父代中找下一个点
                    if (r == 0)  // ci为父代中的前一个
                    {
                        this->Swap(near_data[pr][fPosiCurr % 2 + 1], near_data[pr][fPosiCurr % 2 + 3]);
                    }
                    break;
                case 3:
                    ci = near_data[pr][fPosiCurr % 2 + 3];   // 找父代2中与pr连接的后一个节点
                    break;
            }

            fRoute[fPosiCurr] = ci;   //更新路径

            // 如果节点连接为 2 表示未被访问
            if (near_data[ci][0] == 2)
            {
                if (ci == st) // 如果回到起点
                {
                    if (check_koritsu[st] == 0)
                    {
                        if ((fPosiCurr - check_koritsu[st]) % 2 == 0) // 判断是否需要形成 AB循环 点的个数必须为偶数
                        {
                            if (near_data[st][fPosiCurr % 2 + 1] == pr)
                            {
                                this->Swap(near_data[ci][fPosiCurr % 2 + 1], near_data[ci][fPosiCurr % 2 + 3]);
                            }
                            st_appear = 1; // 设置起点出现次数
                            this->FormABcycle(tPa1, tPa2); // 形成 AB 循环
                            if (flagC[1] == 1 && fNumOfABcycle == numOfKids) goto LLL; // 检查条件提前退出
                            if (fNumOfABcycle == fMaxNumOfABcycle) goto LLL; // 检查最大循环数

                            flag_st = 0; // 继续当前路径
                            flag_circle = 1;
                            pr_type = 1;
                        }
                        else
                        {
                            this->Swap(near_data[ci][fPosiCurr % 2 + 1], near_data[ci][fPosiCurr % 2 + 3]);
                            pr_type = 2;
                        }
                        check_koritsu[st] = fPosiCurr;
                    }
                    else
                    {
                        st_appear = 2; // 设置起点第二次出现
                        this->FormABcycle(tPa1, tPa2);
                        if (flagC[1] == 1 && fNumOfABcycle == numOfKids) goto LLL;
                        if (fNumOfABcycle == fMaxNumOfABcycle) goto LLL;

                        flag_st = 1; // 准备新的路径
                        flag_circle = 1;
                    }
                }
                else if (check_koritsu[ci] == -1) // 如节点未被访问
                {
                    check_koritsu[ci] = fPosiCurr;
                    if (near_data[ci][fPosiCurr % 2 + 1] == pr)
                    {
                        this->Swap(near_data[ci][fPosiCurr % 2 + 1], near_data[ci][fPosiCurr % 2 + 3]);
                    }
                    pr_type = 2;
                }
                else if (check_koritsu[ci] > 0) // 如节点已被访问
                {
                    this->Swap(near_data[ci][fPosiCurr % 2 + 1], near_data[ci][fPosiCurr % 2 + 3]);
                    if ((fPosiCurr - check_koritsu[ci]) % 2 == 0)
                    {
                        st_appear = 1;
                        this->FormABcycle(tPa1, tPa2);
                        if (flagC[1] == 1 && fNumOfABcycle == numOfKids) goto LLL;
                        if (fNumOfABcycle == fMaxNumOfABcycle) goto LLL;

                        flag_st = 0;
                        flag_circle = 1;
                        pr_type = 1;
                    }
                    else
                    {
                        this->Swap(near_data[ci][(fPosiCurr + 1) % 2 + 1], near_data[ci][(fPosiCurr + 1) % 2 + 3]);
                        pr_type = 3;
                    }
                }
            }
            else if (near_data[ci][0] == 1) // 如果节点仅有一连接
            {
                if (ci == st) // 处理特殊情况
                {
                    st_appear = 1;
                    this->FormABcycle(tPa1, tPa2);
                    if (flagC[1] == 1 && fNumOfABcycle == numOfKids) goto LLL;
                    if (fNumOfABcycle == fMaxNumOfABcycle) goto LLL;

                    flag_st = 1;
                    flag_circle = 1;
                }
                else pr_type = 1;
            }
        }
    }

    // 处理孤立节点
    while(bunki_many != 0)
    {
        fPosiCurr = 0;
        r = rand() % bunki_many;
        st = bunki[r];
        fRoute[fPosiCurr] = st;
        ci = st;

        flag_circle = 0;
        while(flag_circle == 0)
        {
            pr = ci;
            fPosiCurr++;
            ci = near_data[pr][fPosiCurr % 2 + 1];
            fRoute[fPosiCurr] = ci;
            if (ci == st)
            {
                st_appear = 1;
                this->FormABcycle(tPa1, tPa2);
                if (flagC[1] == 1 && fNumOfABcycle == numOfKids) goto LLL;
                if (fNumOfABcycle == fMaxNumOfABcycle) goto LLL;

                flag_circle = 1;
            }
        }
    }

LLL: ;

    // 检查最大 AB 循环限制
    if (fNumOfABcycle == fMaxNumOfABcycle) {
        printf("fMaxNumOfABcycle(%d) must be increased\n", fMaxNumOfABcycle);
        exit(1);
    }
}



void TCross::FormABcycle(const TIndi& tPa1, const TIndi& tPa2) {
    int j;
    int st_count; // 记录起始节点被访问的次数，用于判定循环闭合
    int edge_type; // 边类型，确定循环开始于奇数还是偶数索引
    int st, ci, stock; // 起始节点，当前节点和暂存节点
    int cem; // 循环中的城市计数器
    int diff; // 用于计算循环的距离差异
    int p, c, n, k; // 前一个、当前、下一个节点和临时变量，用于存储城市索引

    // 根据路径当前位置的索引奇偶性确定初始边的类型
    if (fPosiCurr % 2 == 0)
        edge_type = 1;
    else
        edge_type = 2;

    st = fRoute[fPosiCurr]; // 从当前路中获取循环的起始城市
    cem = 0;
    fC[cem] = st; // 将起始城市添加到当前循环城市数组中
    st_count = 0;  // 初始化起始城市计数为0

    while (1) {
        cem++; // 增加循环中的城市计数
        fPosiCurr--; // 移动到路径中的前一个节点
        ci = fRoute[fPosiCurr]; // 获取当前城市

        // 更新当前城市的状态
        if (near_data[ci][0] == 2) { // 如果当前城市有两个连接
            // 从分岔状态转变为独立状态
            koritsu[kori_inv[ci]] = koritsu[koritsu_many - 1]; // 更新分岔节点列表
            kori_inv[koritsu[koritsu_many - 1]] = kori_inv[ci];
            koritsu_many--;
            bunki[bunki_many] = ci; // 将当前城市加入独立节点列表
            bun_inv[ci] = bunki_many;
            bunki_many++;
        } else if (near_data[ci][0] == 1) { // 如果当前城市为分岔点
            // 从独立状态删除
            bunki[bun_inv[ci]] = bunki[bunki_many - 1];
            bun_inv[bunki[bunki_many - 1]] = bun_inv[ci];
            bunki_many--;
        }

        near_data[ci][0]--; // 减少当前城市的连接数

        if (ci == st) // 如果当前城市回到了起始城市
            st_count++; // 增加起始城市的计数

        if (st_count == st_appear) // 如果起始城市出现次数满足条件，循环闭合
            break;

        fC[cem] = ci; // 将当前城市添加到循环城市列表中
    }

    // 检查循环长度
    if (cem == 2)
        return;

    // 记录当前AB循环的开始位置和长度
    fPosi_ABL[fNumOfABcycle] = fSPosi_ABL;
    fABcycleL[fSPosi_ABL + 0] = cem;

    // 如果边类型为2，则旋转循环顺序（调整起始点）
    if (edge_type == 2) {
        stock = fC[0];
        for (int j = 0; j < cem - 1; j++)
            fC[j] = fC[j+1];
        fC[cem-1] = stock;
    }

    // 记录循环中每个城市的顺序
    for (int j = 0; j < cem; j++)
        fABcycleL[fSPosi_ABL + j + 2] = fC[j];

    // 设置循环的开始和结束城市
    fABcycleL[fSPosi_ABL + 1] = fC[cem - 1];
    fABcycleL[fSPosi_ABL + cem + 2] = fC[0];
    fABcycleL[fSPosi_ABL + cem + 3] = fC[1];

    // 根据位置计算AB循环中的城市次序
    for (int j = 2; j <= cem + 2; ++j) {
        p = fABcycleL[fSPosi_ABL + j - 1]; // 前一个城市
        c = fABcycleL[fSPosi_ABL + j];     // 当前城市
        n = fABcycleL[fSPosi_ABL + j + 1]; // 下一个城市

        if (j % 2 == 0) {
            // 偶数索引，根据当前和下一个城市在父代2中寻找连接
            if (tPa2.fLink[c][0] == p)
                fABcycleLOrd[fSPosi_ABL + j][0] = tPa2.fOrder[c][0];
            else
                fABcycleLOrd[fSPosi_ABL + j][0] = tPa2.fOrder[c][1];

            if (tPa1.fLink[c][0] == n)
                fABcycleLOrd[fSPosi_ABL + j][1] = tPa1.fOrder[c][0];
            else
                fABcycleLOrd[fSPosi_ABL + j][1] = tPa1.fOrder[c][1];

        } else {
            // 奇数索引，根据当前和下一个城市在父代1中寻找连接
            if (tPa1.fLink[c][0] == p)
                fABcycleLOrd[fSPosi_ABL + j][0] = tPa1.fOrder[c][0];
            else
                fABcycleLOrd[fSPosi_ABL + j][0] = tPa1.fOrder[c][1];

            if (tPa2.fLink[c][0] == n)
                fABcycleLOrd[fSPosi_ABL + j][1] = tPa2.fOrder[c][0];
            else
                fABcycleLOrd[fSPosi_ABL + j][1] = tPa2.fOrder[c][1];
        }
    }

    // 计算AB循环路径的距离差异
    fC[cem] = fC[0];
    fC[cem+1] = fC[1];
    diff = 0;

    for (j = 0; j < cem / 2; ++j) {
        c = fABcycleL[fSPosi_ABL + 2 * j + 2];
        k = fABcycleLOrd[fSPosi_ABL + 2 * j + 2][1];
        if (k != -1)
            diff += eval->fEdgeDisOrder[c][k];
        else
            diff += eval->Direct(fC[2*j], fC[1+2*j]);

        c = fABcycleL[fSPosi_ABL + 2 * j + 3];
        k = fABcycleLOrd[fSPosi_ABL + 2 * j + 3][1];
        if (k != -1)
            diff -= eval->fEdgeDisOrder[c][k];
        else
            diff -= eval->Direct(fC[2*j+1], fC[2*j+2]);
    }

    fGainAB[fNumOfABcycle] = diff; // 记录此次循环的增益或差异值
    ++fNumOfABcycle; // 增加已识别AB循环的数量
    fSPosi_ABL += (cem + 4); // 更新用于下一循环信息存储的起始位置
}



void TCross::Swap(int &a,int &b)
{
  int s;
  s=a;
  a=b;
  b=s;
}


void TCross::ChangeSol(TIndi& tKid, int ABnum, int type)
{
    // 局部变量声明
    int j;
    int cem, r1, r2, b1, b2;
    int po_r1, po_r2, po_b1, po_b2;
    int posi;

    // 获取给定 AB-cycle 的起始位置
    posi = fPosi_ABL[ABnum];

    // 获取当前环的节点数（cycle element count）
    cem = fABcycleL[posi + 0];

    // 初始化操作序列，用于记录操作
    fC[0] = fABcycleL[posi + 0];

    // 按照指定类型初始化 fC 数组，存储 AB-cycle 节点序列
    if(type == 2) {
        for(j = 0; j < cem + 3; j++)
            fC[cem + 3 - j] = fABcycleL[posi + j + 1];
    } else {
        for(j = 1; j <= cem + 3; j++)
            fC[j] = fABcycleL[posi + j];
    }

    // 如果类型是 1，进行正向 AB-cycle 变换
    if(type == 1) {
        for(j = 0; j < cem / 2; j++) {
            // 获取每一对 (r1, r2) 和相邻的边 (b1, b2)
            r1 = fABcycleL[posi + 2 + 2*j];
            r2 = fABcycleL[posi + 3 + 2*j];
            b1 = fABcycleL[posi + 1 + 2*j];
            b2 = fABcycleL[posi + 4 + 2*j];

            // 修改第一个节点 r1 的连接
            if(tKid.fLink[r1][0] == r2) {
                tKid.fLink[r1][0] = b1;
                tKid.fOrder[r1][0] = fABcycleLOrd[posi + 2 + 2*j][0];
            } else {
                tKid.fLink[r1][1] = b1;
                tKid.fOrder[r1][1] = fABcycleLOrd[posi + 2 + 2*j][0];
            }

            // 修改第二个节点 r2 的连接
            if(tKid.fLink[r2][0] == r1) {
                tKid.fLink[r2][0] = b2;
                tKid.fOrder[r2][0] = fABcycleLOrd[posi + 3 + 2*j][1];
            } else {
                tKid.fLink[r2][1] = b2;
                tKid.fOrder[r2][1] = fABcycleLOrd[posi + 3 + 2*j][1];
            }

            // 获取节点的当前位置索引
            po_r1 = fInv[r1];
            po_r2 = fInv[r2];
            po_b1 = fInv[b1];
            po_b2 = fInv[b2];

            // 根据节点位置更新切割位置列表
            if(po_r1 == 0 && po_r2 == fN-1)
                fSegPosiList[fNumOfSPL++] = po_r1;
            else if(po_r1 == fN-1 && po_r2 == 0)
                fSegPosiList[fNumOfSPL++] = po_r2;
            else if(po_r1 < po_r2)
                fSegPosiList[fNumOfSPL++] = po_r2;
            else if(po_r2 < po_r1)
                fSegPosiList[fNumOfSPL++] = po_r1;
            else
                assert(1 == 2);

            // 更新节点位置链接缓存
            LinkBPosi[po_r1][1] = LinkBPosi[po_r1][0];
            LinkBPosi[po_r2][1] = LinkBPosi[po_r2][0];
            LinkBPosi[po_r1][0] = po_b1;
            LinkBPosi[po_r2][0] = po_b2;
        }
    }
    // 如果类型是 2，倒序 AB-cycle 变换
    else if(type == 2) {
        for(j = 0; j < cem / 2; j++) {
            // 以倒序获取 r1, r2 和相邻的 b1, b2
            r1 = fABcycleL[posi + cem + 4 - (2 + 2*j)];
            r2 = fABcycleL[posi + cem + 4 - (3 + 2*j)];
            b1 = fABcycleL[posi + cem + 4 - (1 + 2*j)];
            b2 = fABcycleL[posi + cem + 4 - (4 + 2*j)];

            // 修改第一个节点 r1 的连接
            if(tKid.fLink[r1][0] == r2) {
                tKid.fLink[r1][0] = b1;
                tKid.fOrder[r1][0] = fABcycleLOrd[posi + cem + 4 - (2 + 2*j)][1];
            } else {
                tKid.fLink[r1][1] = b1;
                tKid.fOrder[r1][1] = fABcycleLOrd[posi + cem + 4 - (2 + 2*j)][1];
            }

            // 修改第二个节点 r2 的连接
            if(tKid.fLink[r2][0] == r1) {
                tKid.fLink[r2][0] = b2;
                tKid.fOrder[r2][0] = fABcycleLOrd[posi + cem + 4 - (3 + 2*j)][0];
            } else {
                tKid.fLink[r2][1] = b2;
                tKid.fOrder[r2][1] = fABcycleLOrd[posi + cem + 4 - (3 + 2*j)][0];
            }

            // 获取节点的当前位置索引
            po_r1 = fInv[r1];
            po_r2 = fInv[r2];
            po_b1 = fInv[b1];
            po_b2 = fInv[b2];

            // 根据节点位置更新切割位置列表
            if(po_r1 == 0 && po_r2 == fN-1)
                fSegPosiList[fNumOfSPL++] = po_r1;
            else if(po_r1 == fN-1 && po_r2 == 0)
                fSegPosiList[fNumOfSPL++] = po_r2;
            else if(po_r1 < po_r2)
                fSegPosiList[fNumOfSPL++] = po_r2;
            else if(po_r2 < po_r1)
                fSegPosiList[fNumOfSPL++] = po_r1;
            else
                assert(1 == 2);

            // 更新节从位置链接缓存
            LinkBPosi[po_r1][1] = LinkBPosi[po_r1][0];
            LinkBPosi[po_r2][1] = LinkBPosi[po_r2][0];
            LinkBPosi[po_r1][0] = po_b1;
            LinkBPosi[po_r2][0] = po_b2;
        }
    }
}


void TCross::MakeCompleteSol(TIndi& tKid) {
    // 声明局部变量
    int j, j1, j2, j3;
    int st, ci, pre, curr, next, a, b, c, d, aa, bb, a1, b1;
    int city_many; // 城市数量
    int remain_unit_many; // 剩余单元数量
    int ucm; // 单元管理相关变量
    int unit_num; // 单元数量
    int min_unit_city; // 最小城市数的单元
    int near_num; // 近邻城市计数
    int unit_many; // 单元数量
    int center_un; // 中心单元
    int select_un; // 选择的单元
    int diff, max_diff; // 增益差值和最大增益
    int count; // 计数器
    int nearMax; // 限定的最近城市数量
    int dis_ab[2], dis_ac, dis_cd, dis_bd, b_0, b_1; // 距离变量
    int k_0, k_1, k_cd, k_bd; // 辅助变量

    fGainModi = 0; // 初始化增益

    // 当仍有多于一个单元时继续合并
    while(fNumOfUnit != 1) {
        // 找到包含最少城市的单元
        min_unit_city = fN + 12345; // 设置初始最小值
        for(int u = 0; u < fNumOfUnit; ++u) {
            if(fNumOfElementInUnit[u] < min_unit_city) {
                center_un = u; // 更新中心单元
                min_unit_city = fNumOfElementInUnit[u]; // 更新最小城市数
            }
        }

        // 初始化中心单元信息
        st = -1; // 初始化起始城市
        fNumOfSegForCenter = 0; // 初始化中心单元段数量
        for(int s = 0; s < fNumOfSeg; ++s) {
            if(fSegUnit[s] == center_un) {
                int posi = fSegment[s][0]; // 获取段的起始位置
                st = fOrder[posi]; // 更新起始城市
                fSegForCenter[fNumOfSegForCenter++] = s; // 收集中心单元的段
            }
        }
        assert(st != -1); // 确保起始城市有效

        // 收集中心单元的节点信息
        curr = -1; // 当前城市
        next = st; // 下一个城市
        fNumOfElementInCU = 0; // 初始化中心单元中的城市数量
        while(1) {
            pre = curr; // 记录前一个城市
            curr = next; // 更新当前城市
            fCenterUnit[curr] = 1; // 标记当前节点属于中心单元
            fListOfCenterUnit[fNumOfElementInCU] = curr; // 收集城市
            ++fNumOfElementInCU; // 增加城市计数

            // 找到下一个城市
            if(tKid.fLink[curr][0] != pre)
                next = tKid.fLink[curr][0];
            else
                next = tKid.fLink[curr][1];

            if(next == st) break; // 如果回到起始城市，停止循环
        }
        fListOfCenterUnit[fNumOfElementInCU] = fListOfCenterUnit[0]; // 闭合中心单元列表
        fListOfCenterUnit[fNumOfElementInCU + 1] = fListOfCenterUnit[1]; // 记录第二个城市

        assert(fNumOfElementInCU == fNumOfElementInUnit[center_un]); // 确保城市数量一致

        // 寻找能够最大化增益的交换操作
        max_diff = -999999999; // 初始化最大增益
        a1 = -1; b1 = -1; // 初始化交换城市
        nearMax = 10; // 限定最近城市的数量

    RESTART:; // 重新开始搜索标签
        for(int s = 1; s <= fNumOfElementInCU; ++s) {
            a = fListOfCenterUnit[s]; // 当前中心城市

            b_0 = fListOfCenterUnit[s-1]; // 前一个城市
            b_1 = fListOfCenterUnit[s+1]; // 后一个城市

            // 确定城市的连接顺序
            if(tKid.fLink[a][0] == b_0) {
                k_0 = tKid.fOrder[a][0];
                k_1 = tKid.fOrder[a][1];
            } else {
                k_0 = tKid.fOrder[a][1];
                k_1 = tKid.fOrder[a][0];
            }

            // 计算与前一个城市的距离
            if(k_0 != -1)
                dis_ab[0] = eval->fEdgeDisOrder[a][k_0];
            else
                dis_ab[0] = eval->Direct(a, b_0); // 直接计算距离
            if(k_1 != -1)
                dis_ab[1] = eval->fEdgeDisOrder[a][k_1];
            else
                dis_ab[1] = eval->Direct(a, b_1); // 直接计算距离

            // 遍历附近的城市
            for(near_num = 1; near_num < nearMax; ++near_num) {
                c = eval->fNearCity[a][near_num]; // 获取邻近城市
                dis_ac = eval->fEdgeDisOrder[a][near_num]; // 计算与邻近城市的距离

                if(fCenterUnit[c] == 0) { // 确保不是中心单元的城市
                    for(j1 = 0; j1 < 2; ++j1) {
                        b = fListOfCenterUnit[s-1 + 2 * j1]; // 前后城市

                        for(j2 = 0; j2 < 2; ++j2) {
                            d = tKid.fLink[c][j2]; // 获取当前城市的连接城市
                            k_cd = tKid.fOrder[c][j2];

                            if(k_cd != -1)
                                dis_cd = eval->fEdgeDisOrder[c][k_cd]; // 计算距离
                            else
                                dis_cd = eval->Direct(c, d); // 直接计算距离

                            // 计算增益差
                            diff = dis_ab[j1] + dis_cd - dis_ac - eval->Direct(b, d);
                            if(diff > max_diff) { // 如果增益更大，记录交换
                                aa = a; bb = b; a1 = c; b1 = d;
                                max_diff = diff;
                            }

                            // 另一种交换方式
                            diff = dis_ab[j1] + dis_cd - eval->Direct(a, d) - eval->Direct(b, c);
                            if(diff > max_diff) { // 如果增益更大，记录交换
                                aa = a; bb = b; a1 = d; b1 = c;
                                max_diff = diff;
                            }
                        }
                    }
                }
            }
        }

        // 如果没有找到合适的交换，增加近邻搜索的深度
        if(a1 == -1 && nearMax == 10) {
            nearMax = 50; // 增加搜索深度
            goto RESTART; // 重启搜索
        } else if(a1 == -1 && nearMax == 50) {
            // 随机选择一个进行交换
            int r = rand() % (fNumOfElementInCU - 1); // 随机选择城市
            a = fListOfCenterUnit[r];
            b = fListOfCenterUnit[r + 1];
            for(j = 0; j < fN; ++j) {
                if(fCenterUnit[j] == 0) { // 找到一个非中心城市
                    aa = a; bb = b;
                    a1 = j; // 随机选择的城市
                    b1 = tKid.fLink[j][0]; // 获取其连接的城市
                    break;
                }
            }
            // 计算增益
            max_diff = eval->Direct(aa, bb) + eval->Direct(a1, b1) - eval->Direct(a, a1) - eval->Direct(b, b1);
        }

        // 记录并应用找到的最优交换
        fModiEdgeOrd[fNumOfModiEdge][0][1] = eval->GetOrd(aa, a1);
        fModiEdgeOrd[fNumOfModiEdge][1][1] = eval->GetOrd(bb, b1);
        fModiEdgeOrd[fNumOfModiEdge][2][1] = eval->GetOrd(a1, aa);
        fModiEdgeOrd[fNumOfModiEdge][3][1] = eval->GetOrd(b1, bb);

        // 更新城市链接
        if(tKid.fLink[aa][0] == bb) {
            tKid.fLink[aa][0] = a1; // 更新链接
            tKid.fLink[bb][1] = a1; // 更新链接
        } else {
            tKid.fLink[aa][1] = a1; // 更新链接
            tKid.fLink[bb][0] = a1; // 更新链接
        }

        // 记录更新
        fNumOfModiEdge++; // 增加修改计数
        fGainModi += max_diff; // 更新增益值

        // 合并相关单元
        if(fNumOfElementInUnit[center_un] > 0) {
            unit_num = fNumOfUnit; // 获取当前单元数量
            for(int u = 0; u < unit_num; ++u) {
                if(u == center_un) continue; // 跳过中心单元
                if(fNumOfElementInUnit[u] > 0) {
                    // 合并单元，更新单元信息
                    for(int e = 0; e < fNumOfElementInUnit[u]; ++e) {
                        fSegUnit[fSegUnitCount] = u; // 更新单元编号
                        fSegUnit[fSegUnitCount + 1] = center_un; // 更新单元编号
                    }
                    fNumOfElementInUnit[center_un] += fNumOfElementInUnit[u]; // 更新城市数量
                    fNumOfElementInUnit[u] = 0; // 重置被合并单元的城市数量
                }
            }
        }
    }
    // 重置中心单元标记
    for(int i = 0; i < fN; ++i) {
        fCenterUnit[i] = 0; // 解除标记
    }
}




void TCross::MakeUnit() {
    int flag = 1;

    // 检查 fSegPosiList 中是否存在位置为 0 的元素
    for(int s = 0; s < fNumOfSPL; ++s) {
        if(fSegPosiList[s] == 0) {
            flag = 0;
            break;
        }
    }

    // 如果没有发现位置 0，添加并进行特殊处理
    if(flag == 1) {
        // 在分段位置中添加 0（如果起点未包含）
        fSegPosiList[fNumOfSPL++] = 0;

        // 更新索引及链接关系
        LinkBPosi[fN-1][1] = LinkBPosi[fN-1][0];
        LinkBPosi[0][1] = LinkBPosi[0][0];
        LinkBPosi[fN-1][0] = 0;
        LinkBPosi[0][0] = fN-1;
    }

    // 对分段位置进行排序
    tSort->Sort(fSegPosiList, fNumOfSPL);

    // 初始化分段信息
    fNumOfSeg = fNumOfSPL;
    for(int s = 0; s < fNumOfSeg-1; ++s) {
        fSegment[s][0] = fSegPosiList[s];
        fSegment[s][1] = fSegPosiList[s+1] - 1;
    }
    fSegment[fNumOfSeg-1][0] = fSegPosiList[fNumOfSeg-1];
    fSegment[fNumOfSeg-1][1] = fN - 1;

    // 更新链接位置和分段位置索引
    for(int s = 0; s < fNumOfSeg; ++s) {
        LinkAPosi[fSegment[s][0]] = fSegment[s][1];
        LinkAPosi[fSegment[s][1]] = fSegment[s][0];
        fPosiSeg[fSegment[s][0]] = s;
        fPosiSeg[fSegment[s][1]] = s;
    }

    // 初始化单元信息
    for(int s = 0; s < fNumOfSeg; ++s)
        fSegUnit[s] = -1;

    fNumOfUnit = 0;

    int p_st, p1, p2, p_next, p_pre;
    int segNum;

    // 创建单元
    while(1) {
        flag = 0;

        // 寻找尚未标记的分段
        for(int s = 0; s < fNumOfSeg; ++s) {
            if(fSegUnit[s] == -1) {
                p_st = fSegment[s][0];
                p_pre = -1;
                p1 = p_st;
                flag = 1;
                break;
            }
        }

        if(flag == 0)
            break;

        // 创建单个路径单元
        while(1) {
            segNum = fPosiSeg[p1];
            fSegUnit[segNum] = fNumOfUnit; // 将分段标记给当前单元

            // 更新下一个路径点
            p2 = LinkAPosi[p1];
            p_next = LinkBPosi[p2][0];
            if(p1 == p2) {
                if(p_next == p_pre)
                    p_next = LinkBPosi[p2][1];
            }

            // 如果找到起始点结束路径单元的创建
            if(p_next == p_st) {
                ++fNumOfUnit;
                break;
            }

            p_pre = p2;
            p1 = p_next;
        }
    }

    // 初始化每个单元中的元素数量
    for(int s = 0; s < fNumOfUnit; ++s)
        fNumOfElementInUnit[s] = 0;

    int unitNum = -1;
    int tmpNumOfSeg = -1;

    // 合并和更新分段信息
    for(int s = 0; s < fNumOfSeg; ++s) {
        if(fSegUnit[s] != unitNum) {
            ++tmpNumOfSeg;
            fSegment[tmpNumOfSeg][0] = fSegment[s][0];
            fSegment[tmpNumOfSeg][1] = fSegment[s][1];
            unitNum = fSegUnit[s];
            fSegUnit[tmpNumOfSeg] = unitNum;
            fNumOfElementInUnit[unitNum] += fSegment[s][1] - fSegment[s][0] + 1;
        } else {
            fSegment[tmpNumOfSeg][1] = fSegment[s][1];
            fNumOfElementInUnit[unitNum] += fSegment[s][1] - fSegment[s][0] + 1;
        }
    }

    // 更新分段数量
    fNumOfSeg = tmpNumOfSeg + 1;
}



void TCross::BackToPa1( TIndi& tKid )
{
  int aa, bb, a1, b1; 
  int jnum;

  for( int s = fNumOfModiEdge -1; s >= 0; --s ){ 
    aa = fModiEdge[ s ][ 0 ];
    bb = fModiEdge[ s ][ 1 ];  
    a1 = fModiEdge[ s ][ 2 ];  
    b1 = fModiEdge[ s ][ 3 ];

    if( tKid.fLink[aa][0] == a1 ){
      tKid.fLink[aa][0] = bb;
      tKid.fOrder[aa][0] = fModiEdgeOrd[ s ][0][0];
    }
    else{
      tKid.fLink[aa][1] = bb;
      tKid.fOrder[aa][1] = fModiEdgeOrd[ s ][0][0];
    }
    if( tKid.fLink[a1][0] == aa ){
      tKid.fLink[a1][0] = b1;
      tKid.fOrder[a1][0] = fModiEdgeOrd[ s ][2][0];
    }
    else{
      tKid.fLink[a1][1] = b1;   
      tKid.fOrder[a1][1] = fModiEdgeOrd[ s ][2][0];
    }
    if( tKid.fLink[b1][0] == bb ){
      tKid.fLink[b1][0] = a1;
      tKid.fOrder[b1][0] = fModiEdgeOrd[ s ][3][0];
    }
    else{
      tKid.fLink[b1][1] = a1; 
      tKid.fOrder[b1][1] = fModiEdgeOrd[ s ][3][0];
    }
    if( tKid.fLink[bb][0] == b1 ){
      tKid.fLink[bb][0] = aa;
      tKid.fOrder[bb][0] = fModiEdgeOrd[ s ][1][0];
    }
    else{
      tKid.fLink[bb][1] = aa;
      tKid.fOrder[bb][1] = fModiEdgeOrd[ s ][1][0];
    }
  }
  
  for( int s = 0; s < fNumOfAppliedCycle; ++s ){
    jnum = fAppliedCylce[ s ];
    this->ChangeSol( tKid, jnum, 2 );
  }
}


void TCross::GoToBest( TIndi& tKid )
{
  int aa, bb, a1, b1; 
  int jnum;

  for( int s = 0; s < fNumOfBestAppliedCycle; ++s ){
    jnum = fBestAppliedCylce[ s ];
    this->ChangeSol( tKid, jnum, 1 );
  }


  for( int s = 0; s < fNumOfBestModiEdge; ++s )
  { 
    aa = fBestModiEdge[ s ][ 0 ];
    bb = fBestModiEdge[ s ][ 1 ];   
    a1 = fBestModiEdge[ s ][ 2 ];   
    b1 = fBestModiEdge[ s ][ 3 ];

    if( tKid.fLink[aa][0] == bb ){
      tKid.fLink[aa][0] = a1;
      tKid.fOrder[aa][0] = fBestModiEdgeOrd[ s ][0][1]; // Large
    }
    else{
      tKid.fLink[aa][1] = a1;
      tKid.fOrder[aa][1] = fBestModiEdgeOrd[ s ][0][1]; // Large
    }
    if( tKid.fLink[bb][0] == aa ){
      tKid.fLink[bb][0] = b1;
      tKid.fOrder[bb][0] = fBestModiEdgeOrd[ s ][1][1]; // Large
    }
    else{
      tKid.fLink[bb][1] = b1;   
      tKid.fOrder[bb][1] = fBestModiEdgeOrd[ s ][1][1]; // Large
    }
    if( tKid.fLink[a1][0] == b1 ){
      tKid.fLink[a1][0] = aa;
      tKid.fOrder[a1][0] = fBestModiEdgeOrd[ s ][2][1]; // Large
    }
    else{
      tKid.fLink[a1][1] = aa;
      tKid.fOrder[a1][1] = fBestModiEdgeOrd[ s ][2][1]; // Large
    }
    if( tKid.fLink[b1][0] == a1 ){
      tKid.fLink[b1][0] = bb;
      tKid.fOrder[b1][0] = fBestModiEdgeOrd[ s ][3][1]; // Large
    }
    else{ 
      tKid.fLink[b1][1] = bb; 
      tKid.fOrder[b1][1] = fBestModiEdgeOrd[ s ][3][1]; // Large
    }
  }
}


void TCross::IncrementEdgeFreq( int **fEdgeFreq )
{
  int j, jnum, cem;
  int r1, r2, b1, b2;
  int aa, bb, a1;
  int k;
  int posi;
  
  // AB-cycle$B$K$h$k99?7(B 
  for( int s = 0; s < fNumOfBestAppliedCycle; ++s ){
    jnum = fBestAppliedCylce[ s ];

    posi = fPosi_ABL[jnum];      // Large
    cem = fABcycleL[ posi + 0 ]; // cem = fABcycle[ jnum ][ 0 ];  

    for( j = 0; j <cem/2; ++j )
    {                           
      // r1 = fABcycle[ jnum ][2+2*j]; r2 = fABcycle[ jnum ][3+2*j]; 
      // b1 = fABcycle[ jnum ][1+2*j]; b2 = fABcycle[ jnum ][4+2*j]; 
      r1 = fABcycleL[ posi + 2+2*j ]; r2 = fABcycleL[ posi + 3+2*j ]; 
      b1 = fABcycleL[ posi + 1+2*j ]; b2 = fABcycleL[ posi + 4+2*j ]; 

      // r1 - b1 add 
      // r1 - r2 remove
      // r2 - r1 remove
      // r2 - b2 add

      k = fABcycleLOrd[ posi + 2+2*j ][ 0 ]; // fABcycleOrd[ jnum ][ 2+2*j ][ 0 ]; // Large Large Large
      if( k != -1 ) 
	++fEdgeFreq[ r1 ][ k ];
      k = fABcycleLOrd[ posi + 2+2*j ][ 1 ]; // fABcycleOrd[ jnum ][ 2+2*j ][ 1 ];
      if( k != -1 ) 
	--fEdgeFreq[ r1 ][ k ];
      k = fABcycleLOrd[ posi + 3+2*j ][ 0 ]; // fABcycleOrd[ jnum ][ 3+2*j ][ 0 ];
      if( k != -1 ) 
	--fEdgeFreq[ r2 ][ k ];
      k = fABcycleLOrd[ posi + 3+2*j ][ 1 ]; // fABcycleOrd[ jnum ][ 3+2*j ][ 1 ];
      if( k != -1 ) 
	++fEdgeFreq[ r2 ][ k ];
    }
  }

  // Modification$B$K$h$k99?7(B
  for( int s = 0; s < fNumOfBestModiEdge; ++s )
  { 
    aa = fBestModiEdge[ s ][ 0 ];
    bb = fBestModiEdge[ s ][ 1 ];   
    a1 = fBestModiEdge[ s ][ 2 ];   
    b1 = fBestModiEdge[ s ][ 3 ];

    k = fBestModiEdgeOrd[ s ][0][0];          // Large Large Large
    if( k != -1 ) --fEdgeFreq[ aa ][ k ];
    k = fBestModiEdgeOrd[ s ][0][1];
    if( k != -1 ) ++fEdgeFreq[ aa ][ k ];

    k = fBestModiEdgeOrd[ s ][1][0];
    if( k != -1 ) --fEdgeFreq[ bb ][ k ];
    k = fBestModiEdgeOrd[ s ][1][1];
    if( k != -1 ) ++fEdgeFreq[ bb ][ k ];

    k = fBestModiEdgeOrd[ s ][2][0];
    if( k != -1 ) --fEdgeFreq[ a1 ][ k ];
    k = fBestModiEdgeOrd[ s ][2][1];
    if( k != -1 ) ++fEdgeFreq[ a1 ][ k ];

    k = fBestModiEdgeOrd[ s ][3][0];
    if( k != -1 ) --fEdgeFreq[ b1 ][ k ];
    k = fBestModiEdgeOrd[ s ][3][1];
    if( k != -1 ) ++fEdgeFreq[ b1 ][ k ];
  }
}


double TCross::Cal_ENT_Loss( int **fEdgeFreq ) // Large Large Large
{
  int j, jnum, cem;
  int r1, r2, b1, b2;
  int aa, bb, a1;
  double DLoss; 
  double h1, h2;
  int k;
  int posi;
  
  DLoss = 0;
  // AB-cycle
  for( int s = 0; s < fNumOfAppliedCycle; ++s ){
    jnum = fAppliedCylce[ s ];
    
    posi = fPosi_ABL[jnum];   // Large
    cem = fABcycleL[ posi + 0 ]; // fABcycle[ jnum ][ 0 ];  

    for( j = 0; j <cem/2; ++j )
    {                           
      // r1 = fABcycle[ jnum ][2+2*j]; r2 = fABcycle[ jnum ][3+2*j];  
      // b1 = fABcycle[ jnum ][1+2*j]; b2 = fABcycle[ jnum ][4+2*j]; 
      r1 = fABcycleL[ posi + 2+2*j]; r2 = fABcycleL[ posi + 3+2*j]; 
      b1 = fABcycleL[ posi + 1+2*j]; b2 = fABcycleL[ posi + 4+2*j]; 

      // r1 - b1 add   
      // r1 - r2 remove
      // r2 - r1 remove
      // r2 - b2 add

      // Remove
      k = fABcycleLOrd[ posi + 2+2*j ][ 1 ]; // fABcycleOrd[ jnum ][ 2+2*j ][ 1 ];
      if( k != -1 ){
	h1 = (double)( fEdgeFreq[ r1 ][ k ] - 1 )/(double)fNumOfPop;
	h2 = (double)( fEdgeFreq[ r1 ][ k ] )/(double)fNumOfPop;
	if( fEdgeFreq[ r1 ][ k ] - 1 != 0 )
	  DLoss -= h1 * log( h1 );
	DLoss += h2 * log( h2 );
	--fEdgeFreq[ r1 ][ k ]; 	
      }
      k = fABcycleLOrd[ posi + 3+2*j ][ 0 ]; // fABcycleOrd[ jnum ][ 3+2*j ][ 0 ];      
      if( k != -1 )      
	--fEdgeFreq[ r2 ][ k ]; 	

      // Add
      k = fABcycleLOrd[ posi + 3+2*j ][ 1 ]; // fABcycleOrd[ jnum ][ 3+2*j ][ 1 ];
      if( k != -1 ){
	h1 = (double)( fEdgeFreq[ r2 ][ k ] + 1 )/(double)fNumOfPop;
	h2 = (double)( fEdgeFreq[ r2 ][ k ])/(double)fNumOfPop;
	DLoss -= h1 * log( h1 );
	if( fEdgeFreq[ r2 ][ k ] != 0 )
	  DLoss += h2 * log( h2 );
	++fEdgeFreq[ r2 ][ k ]; 
      }
      k = fABcycleLOrd[ posi + 4+2*j ][ 0 ]; // fABcycleOrd[ jnum ][ 4+2*j ][ 0 ];            
      if( k != -1 )      
	++fEdgeFreq[ b2 ][ k ]; 
    }
  }

  // Modification
  for( int s = 0; s < fNumOfModiEdge; ++s )
  { 
    aa = fModiEdge[ s ][ 0 ];
    bb = fModiEdge[ s ][ 1 ];   
    a1 = fModiEdge[ s ][ 2 ];   
    b1 = fModiEdge[ s ][ 3 ];

    // Remove
    k = fModiEdgeOrd[ s ][0][0];   
    if( k != -1 ){
      h1 = (double)( fEdgeFreq[ aa ][ k ] - 1 )/(double)fNumOfPop;
      h2 = (double)( fEdgeFreq[ aa ][ k ] )/(double)fNumOfPop;
      if( fEdgeFreq[ aa ][ k ] - 1 != 0 )
	DLoss -= h1 * log( h1 );
      DLoss += h2 * log( h2 );
      --fEdgeFreq[ aa ][ k ];
    }
    k = fModiEdgeOrd[ s ][1][0];   
    if( k != -1 )
      --fEdgeFreq[ bb ][ k ];

    k = fModiEdgeOrd[ s ][2][0];   
    if( k != -1 ){
      h1 = (double)( fEdgeFreq[ a1 ][ k ] - 1 )/(double)fNumOfPop;
      h2 = (double)( fEdgeFreq[ a1 ][ k ] )/(double)fNumOfPop;
      if( fEdgeFreq[ a1 ][ k ] - 1 != 0 )
	DLoss -= h1 * log( h1 );
      DLoss += h2 * log( h2 );
      --fEdgeFreq[ a1 ][ k ];
    }
    k = fModiEdgeOrd[ s ][3][0];   
    if( k != -1 )
      --fEdgeFreq[ b1 ][ k ];

    // Add
    k = fModiEdgeOrd[ s ][0][1];   
    if( k != -1 ){
      h1 = (double)( fEdgeFreq[ aa ][ k ] + 1 )/(double)fNumOfPop;
      h2 = (double)( fEdgeFreq[ aa ][ k ])/(double)fNumOfPop;
      DLoss -= h1 * log( h1 );
      if( fEdgeFreq[ aa ][ k ] != 0 )
	DLoss += h2 * log( h2 );
      ++fEdgeFreq[ aa ][ k ];
    }
    k = fModiEdgeOrd[ s ][2][1];   
    if( k != -1 )
      ++fEdgeFreq[ a1 ][ k ];

    k = fModiEdgeOrd[ s ][1][1];   
    if( k != -1 ){
      h1 = (double)( fEdgeFreq[ bb ][ k ] + 1 )/(double)fNumOfPop;
      h2 = (double)( fEdgeFreq[ bb ][ k ])/(double)fNumOfPop;
      DLoss -= h1 * log( h1 );
      if( fEdgeFreq[ bb ][ k ] != 0 )
	DLoss += h2 * log( h2 );
      ++fEdgeFreq[ bb ][ k ];
    }
    k = fModiEdgeOrd[ s ][3][1];   
    if( k != -1 )
      ++fEdgeFreq[ b1 ][ k ];
  }
  DLoss = -DLoss;  
  
  // restore EdgeFreq
  for( int s = 0; s < fNumOfAppliedCycle; ++s ){
    jnum = fAppliedCylce[ s ];

    posi = fPosi_ABL[jnum];   // Large
    cem = fABcycleL[ posi + 0 ]; // fABcycle[ jnum ][ 0 ];  

    for( j = 0; j <cem/2; ++j )
    {                           
      // r1 = fABcycle[ jnum ][2+2*j]; r2 = fABcycle[ jnum ][3+2*j]; 
      // b1 = fABcycle[ jnum ][1+2*j]; b2 = fABcycle[ jnum ][4+2*j]; 
      r1 = fABcycleL[ posi + 2+2*j ]; r2 = fABcycleL[ posi + 3+2*j ]; 
      b1 = fABcycleL[ posi + 1+2*j ]; b2 = fABcycleL[ posi + 4+2*j ]; 

      k = fABcycleLOrd[ posi + 2+2*j ][ 1 ]; // fABcycleOrd[ jnum ][ 2+2*j ][ 1 ];
      if( k != -1 ) ++fEdgeFreq[ r1 ][ k ]; 	
      k = fABcycleLOrd[ posi + 3+2*j ][ 0 ]; // fABcycleOrd[ jnum ][ 3+2*j ][ 0 ];      
      if( k != -1 ) ++fEdgeFreq[ r2 ][ k ]; 	     

      k = fABcycleLOrd[ posi + 3+2*j ][ 1 ]; // fABcycleOrd[ jnum ][ 3+2*j ][ 1 ];
      if( k != -1 ) --fEdgeFreq[ r2 ][ k ]; 
      k = fABcycleLOrd[ posi + 4+2*j ][ 0 ]; // fABcycleOrd[ jnum ][ 4+2*j ][ 0 ];            
      if( k != -1 ) --fEdgeFreq[ b2 ][ k ];      
    }
  }

  for( int s = 0; s < fNumOfModiEdge; ++s )
  { 
    aa = fModiEdge[ s ][ 0 ];
    bb = fModiEdge[ s ][ 1 ];   
    a1 = fModiEdge[ s ][ 2 ];   
    b1 = fModiEdge[ s ][ 3 ];

    k = fModiEdgeOrd[ s ][0][0];   
    if( k != -1 ) ++fEdgeFreq[ aa ][ k ];
    k = fModiEdgeOrd[ s ][1][0];   
    if( k != -1 ) ++fEdgeFreq[ bb ][ k ];

    k = fModiEdgeOrd[ s ][2][0];   
    if( k != -1 ) ++fEdgeFreq[ a1 ][ k ];
    k = fModiEdgeOrd[ s ][3][0];   
    if( k != -1 ) ++fEdgeFreq[ b1 ][ k ];

    k = fModiEdgeOrd[ s ][0][1];   
    if( k != -1 ) --fEdgeFreq[ aa ][ k ];
    k = fModiEdgeOrd[ s ][2][1];   
    if( k != -1 ) --fEdgeFreq[ a1 ][ k ];

    k = fModiEdgeOrd[ s ][1][1];   
    if( k != -1 ) --fEdgeFreq[ bb ][ k ];
    k = fModiEdgeOrd[ s ][3][1];   
    if( k != -1 ) --fEdgeFreq[ b1 ][ k ];
  }

  return DLoss;
}


void TCross::SetWeight(const TIndi& tPa1, const TIndi& tPa2)
{
  int cem;
  int r1, r2, v1, v2, v_p;
  int AB_num;
  int posi;

  // 初始化fInEffectNode数组，表示每个节点在那些AB-cycle间有效
  for (int i = 0; i < fN; ++i) {
    fInEffectNode[i][0] = -1;
    fInEffectNode[i][1] = -1;
  }

  // Step 1:
  // 遍历所有AB-cycles，并记录每个AB-cycle中的节点对
  for (int s = 0; s < fNumOfABcycle; ++s) {
    posi = fPosi_ABL[s];      // 获取当前AB-cycle的起始位置
    cem = fABcycleL[posi + 0]; // 表示这个cycle中的元素数量

    for (int j = 0; j < cem / 2; ++j) { // 遍历当前cycle中每对节点（成对处理）
      r1 = fABcycleL[posi + 2*j+2]; // 获取cycle中的第一个节点
      r2 = fABcycleL[posi + 2*j+3]; // 获取cycle中的第二个节点

      // 对r1节点记录该cycle的索引
      if (fInEffectNode[r1][0] == -1) fInEffectNode[r1][0] = s;
      else if (fInEffectNode[r1][1] == -1) fInEffectNode[r1][1] = s;
      else assert(1 == 2); // 如果某节点在超过两个不同的AB-cycle间有效，断言失败

      // 对r2节点记录该cycle的索引
      if (fInEffectNode[r2][0] == -1) fInEffectNode[r2][0] = s;
      else if (fInEffectNode[r2][1] == -1) fInEffectNode[r2][1] = s;
      else assert(1 == 2);
    }
  }

  // Step 2:
  // 为只在一个AB-cycle有效的节点，补充它们的封闭路径
  for (int i = 0; i < fN; ++i) {
    if (fInEffectNode[i][0] != -1 && fInEffectNode[i][1] == -1) {
      AB_num = fInEffectNode[i][0];  // 获取节点所在的AB-cycle索引
      v1 = i;

      // 找到和当前节点（v1）不同的接点（不同于父代路径中另一父代的连接点）
      if (tPa1.fLink[v1][0] != tPa2.fLink[v1][0] && tPa1.fLink[v1][0] != tPa2.fLink[v1][1])
        v_p = tPa1.fLink[v1][0];
      else if (tPa1.fLink[v1][1] != tPa2.fLink[v1][0] && tPa1.fLink[v1][1] != tPa2.fLink[v1][1])
        v_p = tPa1.fLink[v1][1];
      else
        assert(1 == 2);

      // 补充单个有效节点的路径，确保封闭和二效节点一致性
      while (1) {
        assert(fInEffectNode[v1][0] != -1);
        assert(fInEffectNode[v1][1] == -1);
        fInEffectNode[v1][1] = AB_num;  // 标记第二效的membership

        if (tPa1.fLink[v1][0] != v_p)
          v2 = tPa1.fLink[v1][0];
        else if (tPa1.fLink[v1][1] != v_p)
          v2 = tPa1.fLink[v1][1];
        else
          assert(1 == 2);

        // 更新v2的AB-cycle信息
        if (fInEffectNode[v2][0] == -1)
          fInEffectNode[v2][0] = AB_num;
        else if (fInEffectNode[v2][1] == -1)
          fInEffectNode[v2][1] = AB_num;
        else
          assert(1 == 2);

        if (fInEffectNode[v2][1] != -1)
          break; // 如果路径微调形成封闭，退出循环

        v_p = v1; // 继续调整路径节点
        v1 = v2;
      }
    }
  }

  // Step 3:
  assert(fNumOfABcycle < fMaxNumOfABcycle); // 确认AB-cycle数量未超限

  // 初始化权重矩阵，清零开始
  for (int s1 = 0; s1 < fNumOfABcycle; ++s1) {
    fWeight_C[s1] = 0;       // 清零计数，独立AB-cycle
    for (int s2 = 0; s2 < fNumOfABcycle; ++s2) {
      fWeight_RR[s1][s2] = 0; // 初始化双循环共用权重
    }
  }

  // 对每个节点，根据其在AB-cycle中的双重有效性更新权重
  for (int i = 0; i < fN; ++i) {
    assert((fInEffectNode[i][0] == -1 && fInEffectNode[i][1] == -1) ||
           (fInEffectNode[i][0] != -1 && fInEffectNode[i][1] != -1));

    if (fInEffectNode[i][0] != -1 && fInEffectNode[i][1] != -1) {
      ++fWeight_RR[fInEffectNode[i][0]][fInEffectNode[i][1]];
      ++fWeight_RR[fInEffectNode[i][1]][fInEffectNode[i][0]];
    }
    if (fInEffectNode[i][0] != fInEffectNode[i][1]) {
      ++fWeight_C[fInEffectNode[i][0]];
      ++fWeight_C[fInEffectNode[i][1]];
    }
  }

  for (int s1 = 0; s1 < fNumOfABcycle; ++s1)
    fWeight_RR[s1][s1] = 0;

  // 验证每个节点的有效性记录
  for (int i = 0; i < fN; ++i) {
    assert((fInEffectNode[i][0] != -1 && fInEffectNode[i][1] != -1) ||
           (fInEffectNode[i][0] == -1 && fInEffectNode[i][1] == -1));
  }
}



int TCross::Cal_C_Naive() 
{
  int count_C;
  int tt;

  count_C = 0;

  for( int i = 0; i < fN; ++i ){
    if( fInEffectNode[ i ][ 0 ] != -1 && fInEffectNode[ i ][ 1 ] != -1 ){
      tt = 0;
      if( fUsedAB[ fInEffectNode[ i ][ 0 ] ] == 1 )
	++tt;
      if( fUsedAB[ fInEffectNode[ i ][ 1 ] ] == 1 )
	++tt;
      if( tt == 1 )
	++count_C;
    }
  }
  return count_C;
}

void TCross::Search_Eset(int centerAB)
{
  int nIter, stagImp;
// nIter: 迭代次数，用于记录当前进行了多少轮优化迭代。
// stagImp: 停滞改进计数，用于记录在没有找到更优解的情况下经过了多少次迭代。

int delta_weight, min_delta_weight_nt;
// delta_weight: 当前迭代中，某个 AB 环被添加或移除后对目标函数的权重变化（增益或损失）。
// min_delta_weight_nt: 未实时加权的最小权重变化，帮助确定在宽松条件下选择哪个 AB 环进行添加/删除。

int flag_AddDelete, flag_AddDelete_nt;
// flag_AddDelete: 标志变量，指示在严格条件下是否选择执行添加（1）或删除（-1）操作，而0表示没有操作。
// flag_AddDelete_nt: 标志变量，指示在宽松条件下是否选择执行添加（1）或删除（-1）操作，而0表示没有操作。

int selected_AB, selected_AB_nt;
// selected_AB: 被选择进行添加或删除操作的 AB 环的索引，在严格条件下的选择。
// selected_AB_nt: 在宽松条件下被选择进行添加或删除操作的 AB 环的索引。

int t_max;
// t_max: 最大随机移动时长，用于控制某个选择的 AB 环在特定迭代后再被重新考虑。

int jnum;
// jnum: 用于临时存储当前正在处理的 AB 环编号，用于在循环中操作具体 AB 环。


  fNum_C = 0;  // 初始化 E-set 中的 C 节点数量
  fNum_E = 0;  // 初始化 E-set 中的边的数量

  fNumOfUsedAB = 0;  // 使用过的 AB 环的数量
  for(int s1 = 0; s1 < fNumOfABcycle; ++s1)
  {
    // 初始化各 AB 环的使用状态与权重
    fUsedAB[s1] = 0;
    fWeight_SR[s1] = 0;
    fMoved_AB[s1] = 0;
  }

  // 将已知的 AB 环加入 E-set
  for(int s = 0; s < fNumOfABcycleInEset; ++s)
  {
    jnum = fABcycleInEset[s];
    this->Add_AB(jnum);
  }
  fBest_Num_C = fNum_C;  // 记录当前最优的总节点数量
  fBest_Num_E = fNum_E;  // 记录当前最优的边的数量

  stagImp = 0;  // 初始化停滞提升计数器
  nIter = 0;  // 初始化迭代计数器

  // 迭代过程开始
  while(1)
  {
    ++nIter;  // 增加迭代次数

    min_delta_weight_nt = 99999999;  // 初始化最小增益权重
    flag_AddDelete = 0;  // 标记本次迭代是否发生了添加/删除操作
    flag_AddDelete_nt = 0;  // 用于放宽条件的标记

    // 遍历所有 AB 环
    for(int s1 = 0; s1 < fNumOfABcycle; ++s1)
    {
      if(fUsedAB[s1] == 0 && fWeight_SR[s1] > 0)  // 尚未使用且具有正权重的 AB 环
      {
        // 计算添加该 AB 环后的权重变化
        delta_weight = fWeight_C[s1] - 2 * fWeight_SR[s1];
        if(fNum_C + delta_weight < fBest_Num_C)
        {
          selected_AB = s1;  // 标记选择的 AB 环
          flag_AddDelete = 1;  // 标记为添加
          fBest_Num_C = fNum_C + delta_weight;  // 更新最优 C 节点数量
        }
        if(delta_weight < min_delta_weight_nt && nIter > fMoved_AB[s1])
        {
          selected_AB_nt = s1;  // 标记为放宽条件下的选择
          flag_AddDelete_nt = 1;
          min_delta_weight_nt = delta_weight;  // 更新最小权重变化
        }
      }
      else if(fUsedAB[s1] == 1 && s1 != centerAB)  // 如果 AB 环已在 E-set 中且不是中心环
      {
        // 计算移除该 AB 环后的权重变化
        delta_weight = - fWeight_C[s1] + 2 * fWeight_SR[s1];
        if(fNum_C + delta_weight < fBest_Num_C)
        {
          selected_AB = s1;
          flag_AddDelete = -1;  // 标记为移除
          fBest_Num_C = fNum_C + delta_weight;
        }
        if(delta_weight < min_delta_weight_nt && nIter > fMoved_AB[s1])
        {
          selected_AB_nt = s1;
          flag_AddDelete_nt = -1;
          min_delta_weight_nt = delta_weight;
        }
      }
    }

    // 如果进行了最佳操作
    if(flag_AddDelete != 0)
    {
      if(flag_AddDelete == 1)
      {
        this->Add_AB(selected_AB);  // 添加选中的 AB 环
      }
      else if(flag_AddDelete == -1)
      {
        this->Delete_AB(selected_AB);  // 移除选中的 AB 环
      }

      fMoved_AB[selected_AB] = nIter + tRand->Integer(1, fTmax);  // 更新选择的AB环的移动时间
      assert(fBest_Num_C == fNum_C);  // 验证当前的 C 节点数是否等于上次计算的最优 C 节点数
      fBest_Num_E = fNum_E;

      // 更新 E-set 中的 AB 环
      fNumOfABcycleInEset = 0;
      for(int s1 = 0; s1 < fNumOfABcycle; ++s1)
      {
        if(fUsedAB[s1] == 1)
          fABcycleInEset[fNumOfABcycleInEset++] = s1;
      }
      assert(fNumOfABcycleInEset == fNumOfUsedAB);  // 验证 E-set 中 AB 环数量是否匹配
      stagImp = 0;  // 重置停滞提升计数
    }
    else if(flag_AddDelete_nt != 0)  // 否则如果没有执行最佳操作，但进行了放宽条件下的操作
    {
      if(flag_AddDelete_nt == 1)
      {
        this->Add_AB(selected_AB_nt);  // 添加根据放宽条件选中的 AB 环
      }
      else if(flag_AddDelete_nt == -1)
      {
        this->Delete_AB(selected_AB_nt);  // 删除根据放宽条件选中的 AB 环
      }
      fMoved_AB[selected_AB_nt] = nIter + tRand->Integer(1, fTmax);  // 更新下一次可以移动的迭代时间
    }

    if(flag_AddDelete == 0)
      ++stagImp;  // 增加停滞计数，表示没有改进

    if(stagImp == fMaxStag)  // 如果达到允许的最大停滞次数，结束迭代
      break;
  }
}



void TCross::Add_AB( int AB_num )  
{
  // 更新 E-set 中的C权重，公式为：当前权重加上该 AB 环的权重减去两倍交集权重。
  fNum_C += fWeight_C[ AB_num ] - 2 * fWeight_SR[ AB_num ];

  // 更新 E-set 中的边的数量。fABcycleL 用于储存边的相关信息，其计算可能根据具体实现依赖一个特定位置值。
  fNum_E += fABcycleL[ fPosi_ABL[AB_num] + 0 ] / 2; // 相当于 fABcycle[ AB_num ][ 0 ] / 2;

  // 验证性断言，确保在添加 AB 环之前，该环的使用状态应该是未使用的（0）。
  assert( fUsedAB[ AB_num ] == 0 );

  // 更新 AB 环的使用情况，标记为已使用。
  fUsedAB[ AB_num ] = 1;

  // 增加已使用的 AB 环数量。
  ++fNumOfUsedAB;

  // 更新所有 AB 环的共享权重矩阵 fWeight_SR，增加与当前添加的 AB 环共享的交集权重。
  for( int s1 = 0; s1 < fNumOfABcycle; ++s1 ){
    fWeight_SR[ s1 ] += fWeight_RR[ s1 ][ AB_num ];
  }
}



void TCross::Delete_AB( int AB_num )  
{
  fNum_C -= fWeight_C[ AB_num ] - 2 * fWeight_SR[ AB_num ];   
  fNum_E -= fABcycleL[ fPosi_ABL[AB_num] + 0 ] / 2; // fABcycle[ AB_num ][ 0 ] / 2;  

  assert( fUsedAB[ AB_num ] == 1 );
  fUsedAB[ AB_num ] = 0;
  --fNumOfUsedAB;

  for( int s1 = 0; s1 < fNumOfABcycle; ++s1 ){
    fWeight_SR[ s1 ] -= fWeight_RR[ s1 ][ AB_num ];
  }
}


void TCross::CheckValid( TIndi& indi )
{
  int curr, pre, next, st;
  int count;
  int p, c, n;

  st = 0;
  curr = -1;
  next = st;

  count = 0;
  while(1){ 
    pre = curr;
    curr = next;
    ++count;
    if( indi.fLink[ curr ][ 0 ] != pre )
      next = indi.fLink[ curr ][ 0 ];
    else 
      next = indi.fLink[ curr ][ 1 ]; 
    
    if( next == st ) break;

    c = curr;
    p = indi.fLink[ c ][ 0 ];
    n = indi.fLink[ c ][ 1 ];

    if( indi.fOrder[ c ][ 0 ] != -1 ){
      assert( eval->fNearCity[ c ][  indi.fOrder[ c ][ 0 ] ] == p );
    }
    if( indi.fOrder[ c ][ 1 ] != -1 ){
      assert( eval->fNearCity[ c ][  indi.fOrder[ c ][ 1 ] ] == n );
    }

    if( count > fN ){
      printf( "Invalid = %d\n", count );
      break;
    }
  }       
  if( count != fN )
    printf( "Invalid = %d\n", count );
}
