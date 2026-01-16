#ifndef __CROSS__
#define __CROSS__

#ifndef __RAND__
#include "rand.h"  // 随机数相关的头文件
#endif

#ifndef __Sort__
#include "sort.h"  // 排序相关的头文件
#endif

#ifndef __INDI__
#include "indi.h"  // 个体类头文件
#endif

#ifndef __EVALUATOR__
#include "evaluator.h"  // 评估器类头文件
#endif

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// TCross类，用于实现EAX（Edge Assembly Crossover）交叉算法
class TCross {
public:
	TCross(int N);  // 构造函数，N为城市数量
	~TCross();  // 析构函数

	// 设置父代个体信息，用于交叉操作
	void SetParents(const TIndi& tPa1, const TIndi& tPa2, int flagC[10], int numOfKids);

	// 主要的EAX交叉操作，生成子代
	void DoIt(TIndi& tKid, TIndi& tPa2, int numOfKids, int flagP, int flagC[10], int** fEdgeFreq);

	// 设置AB循环（AB-cycle），步骤2
	void SetABcycle(const TIndi& parent1, const TIndi& parent2, int flagC[10], int numOfKids);

	// 形成AB循环，保存找到的AB-cycle
	void FormABcycle(const TIndi& tPa1, const TIndi& tPa2);

	// 交换两个值
	void Swap(int& a, int& b);

	// 应用AB循环到中间解上
	void ChangeSol(TIndi& tKid, int ABnum, int type);

	// 完成解的构建，步骤5
	void MakeCompleteSol(TIndi& tKid);

	// 将解调整为合法解，步骤5-1
	void MakeUnit();

	// 将解恢复到父代Pa1的状态
	void BackToPa1(TIndi& tKid);

	// 修改tKid为最优的后代解
	void GoToBest(TIndi& tKid);

	// 增加边频率fEdgeFreq的计数
	void IncrementEdgeFreq(int** fEdgeFreq);

	// 计算平均距离的损失
	int Cal_ADP_Loss(int** fEdgeFreq);

	// 计算边熵的损失
	double Cal_ENT_Loss(int** fEdgeFreq);

	// 设置边权重，用于Block2
	void SetWeight(const TIndi& parent1, const TIndi& parent2);

	// 计算简单的C值（Block2）
	int Cal_C_Naive();

	// 搜索E集
	void Search_Eset(int num);

	// 添加AB循环到E集
	void Add_AB(int AB_num);

	// 从E集中删除AB循环
	void Delete_AB(int AB_num);

	// 检查解的合法性（用于调试）
	void CheckValid(TIndi& indi);

	// 生成的子代数量
	int fNumOfGeneratedCh;

	// 评估器对象，用于计算路径长度等评估操作
	TEvaluator* eval;

	// 种群数量
	int fNumOfPop;

private:
	int fFlagImp;  // 用于标记某些重要的条件或状态（具体含义依赖实现）
	int fN;  // 城市数量
	TIndi tBestTmp;  // 暂存的最优解
	int r, exam;  // 可能用于指数和临时变量使用的地方
	int exam_flag;  // 检查标志
	int** near_data;  // 用于存储邻近数据的二维数组
	int* koritsu, * bunki, * kori_inv, * bun_inv;  // 处理局部分段或特定数据
	int koritsu_many, bunki_many;  // 可能用于记录特定状态的数量
	int st, ci, pr, stock, st_appear;  // 多个操作相关计数
	int* check_koritsu;  // 标记特定条件的数组
	int* fRoute;  // 存储路径信息的数组
	int flag_st, flag_circle, pr_type;  // 标志变量，用于控制逻辑流程
	int ch_dis;  // 可能表示分割距离或某种差异
	int* fABcycleL;  // 存储AB循环的长度
	int* fPosi_ABL;  // 存储AB循环的位置
	int fSPosi_ABL;  // AB循环起始位置
	int* fPermu;  // 用于排列操作
	int fEvalType;  // 评估类型标志
	int fEsetType;  // E集的类型
	int fNumOfABcycleInESet;  // E集中的AB循环数量
	int fNumOfABcycle;  // AB循环的总数量
	int fPosiCurr;  // 当前位置的标记
	int fMaxNumOfABcycle;  // 最大AB循环数量

	int* fC;  // 标记某种状态或类型的数组
	int* fJun;  // 可能用于排序或顺序控制
	int* fOrd1, * fOrd2;  // 用于存储顺序控制

	// Speed Up Start
	int* fOrder;    // 顺序数组
	int* fInv;      // 逆序数组
	int** fSegment; // 存储分段信息的二维数组
	int* fSegUnit;  // 分段单元

	int fNumOfUnit; // 单元数量
	int fNumOfSeg;  // 分段数量
	int* fSegPosiList; // 分段位置列表
	int fNumOfSPL; // 分段位置列表数量
	int* LinkAPosi;   // 存储链A的位置
	int** LinkBPosi;  // 存储链B的位置
	int* fPosiSeg;    // 分段位置
	int* fNumOfElementInUnit; // 单元中的元素数量
	int* fCenterUnit; // 中心单元
	int fNumOfElementInCU;    // 中心单元的元素数量
	int* fListOfCenterUnit; // 存储中心单元相关信息
	int fNumOfSegForCenter;   // 中心单元的分段数量
	int* fSegForCenter;       // 中心单元的分段

	int* fGainAB; // AB增益
	int fGainModi; // 修改增益
	int fNumOfModiEdge; // 修改边的数量
	int fNumOfBestModiEdge; // 最佳修改边的数量
	int** fModiEdge; // 修改边信息的二维数组
	int** fBestModiEdge; // 最佳修改边信息的二维数组

	int fNumOfAppliedCycle; // 应用的循环数量
	int fNumOfBestAppliedCycle; // 最佳应用循环数量
	int* fAppliedCylce; // 应用循环
	int* fBestAppliedCylce; // 最佳应用循环
	// Speed Up End

	// Block2
	int* fNumOfElementINAB;     // AB循环中元素的数量
	int** fInEffectNode;        // 有效节点信息
	int** fWeight_RR;           // RR权重矩阵
	int* fWeight_SR;            // SR权重
	int* fWeight_C;             // C权重
	int* fUsedAB, fNumOfUsedAB; // 使用的AB循环和数量
	int fNum_C, fNum_E; // C和E的数量控制
	int fTmax, fMaxStag; // 最大时间和停滞相关
	int* fMoved_AB; // 移动的AB循环
	int fNumOfABcycleInEset;    // E集中的AB循环数量
	int* fABcycleInEset;  // E集中的AB循环信息
	int fDis_AB;                // AB循环距离差异控制
	int fBest_Num_C, fBest_Num_E; // 最优C和E数量

	int** fABcycleLOrd;           // AB循环排序
	int*** fModiEdgeOrd;          // 修改边排序信息
	int*** fBestModiEdgeOrd;      // 最佳修改边排序信息
};

#endif
