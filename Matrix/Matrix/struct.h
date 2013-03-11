
#ifndef STRUCT_H
#define STRUCT_H
#include <windows.h>

bool sse=true;		//是否开启SSE优化

/** 存储矩阵和向量数据 */
struct MulNode
{
    float *matrix;  //矩阵
    int *xj;    //按matrix的顺序依次存放其中非零元素的列号
    int *xr;    //每行中第一个非零元素被存贮在matrix中的位置
    int row;    //矩阵行数，用于控制多线程任务分配
    int column; //矩阵列数，暂无用处，若未使用可将其删除
	int num;	//矩阵中数据的个数
    
    float *v;   //向量	
};

/** 多线程中作为参数传递的结构体 */
struct MulParam
{   
    MulNode * node; //含用CSR格式保存的矩阵和一维向量的结构体
    int row;    //开始计算的行号
    int num;    //要计算的行数
    
    HANDLE h;   //父线程，当乘法计算结束后唤醒父线程
};

struct Point
{
    int column;
    int row ;
    float value;
};

struct StartFileReturn
{
    Point * pcache; //缓存首地址
    float *v;   //向量指针
    int rowCount;   //矩阵行数
    int columnCount;   //矩阵列数
};
#endif