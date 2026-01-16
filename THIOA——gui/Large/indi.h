#ifndef __INDI__
#define __INDI__

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// TIndi类：表示个体类（用于TSP问题的解）
class TIndi {
public:
	TIndi();   // 构造函数，初始化个体
	~TIndi();  // 析构函数，释放资源

	// 定义个体信息，N表示城市数量
	void Define(int N);

	// 重载赋值运算符，将另一个TIndi对象赋值给当前对象
	TIndi& operator = (const TIndi& src);

	// 重载相等运算符，用于比较两个个体是否相等
	bool operator == (const TIndi& indi2);

	int fN;  // 城市数量

	// fLink存储城市之间的连接信息，表示路径
	int** fLink;

	// fOrder存储访问城市的顺序（大的数据结构）
	int** fOrder;         // 大规模数据，用于保存城市顺序

	// 评估值，用于表示当前路径的总长度
	int fEvaluationValue;
};

#endif
