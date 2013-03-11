#ifndef PARALLEL_H
#define PARALLEL_H

#include "struct.h"
#include "mul.h"
#include <stdio.h>
#include <vector>
using namespace std;

DWORD WINAPI MulThread(LPVOID pParam);

//!多个线程共享变量，必须确保每个线程结束才能销毁
HANDLE hMutex = CreateMutex(NULL,FALSE,NULL);
int runningThread=0;
int repeat=1;

void Compute(int threadCount , MulNode * node)
{
    int rowCount = node->row;
    if(rowCount<=0) return;
    InitResult(rowCount);    
    int rowIndex = 0;
    int num = repeat*rowCount/threadCount*repeat+1;   //TODO 确定算法？
    //printf("rowCount=%d , num=%d\n",rowCount , num);
    //TODO 怎样控制线程数
    HANDLE currentThread=NULL; 
    HANDLE id=GetCurrentThread(); 
    DWORD dw=GetCurrentThreadId(); 
    //这里 id 等于 CTestView::OnRunThread()里面的hx=0xfffffffe 
    
    DuplicateHandle(GetCurrentProcess(),id,GetCurrentProcess(),
            &currentThread,0,FALSE,DUPLICATE_SAME_ACCESS);
    vector<HANDLE> vh;
    while(rowIndex<rowCount)
    {
        //wait(s(3));
        if(runningThread >= threadCount)
        {
            //达到最大线程数时，等待当前线程结束后再产生新的线程
            SuspendThread(currentThread);
        }
        
        //为线程函数分配内存空间
        MulParam * pParam = (MulParam *)HeapAlloc(GetProcessHeap(),
                    HEAP_ZERO_MEMORY,sizeof(MulParam));
        if(pParam == NULL)
        {
            printf("HeapAlloc Fail!\n");
            // TODO ExitProcess(0);
        }
        
        pParam->node = node;
        pParam->row = rowIndex;
        rowIndex += num ;
        pParam->num = rowCount>rowIndex ? num : (rowCount-rowIndex+num);
        pParam->h = currentThread;
        
        //同步runningThread
        WaitForSingleObject(hMutex,INFINITE);
        runningThread++;
        ReleaseMutex(hMutex);
        HANDLE h = CreateThread(NULL,0,MulThread,pParam,0,NULL);
        vh.push_back(h);
    }
    //!CloseHandle(hMutex);  //debugged
    for (vector<HANDLE>::iterator iter = vh.begin() ; iter != vh.end() ; iter++ )
    {
        WaitForSingleObject(*iter,INFINITE);
    }    
    //printf("Compute end\n");
}

DWORD WINAPI MulThread(LPVOID pParam)
{
    MulParam * p = (MulParam *)pParam;  //读取参数

    Mul(p->node , p->row , p->num);

    //同步runningThread，可使用延时检测是否有效
    WaitForSingleObject(hMutex,INFINITE);
    runningThread--;
    ReleaseMutex(hMutex);
    //signal(s(3));
    ResumeThread(p->h);
    
    //释放保存参数的内存（主线程分配的）
    HeapFree(GetProcessHeap(),0,pParam);
    return 0;
}


#endif
