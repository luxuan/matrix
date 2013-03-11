#ifndef PARALLEL_H
#define PARALLEL_H

#include "struct.h"
#include "mul.h"
#include <stdio.h>
#include <vector>
using namespace std;

DWORD WINAPI MulThread(LPVOID pParam);

//!����̹߳������������ȷ��ÿ���߳̽�����������
HANDLE hMutex = CreateMutex(NULL,FALSE,NULL);
int runningThread=0;
int repeat=1;

void Compute(int threadCount , MulNode * node)
{
    int rowCount = node->row;
    if(rowCount<=0) return;
    InitResult(rowCount);    
    int rowIndex = 0;
    int num = repeat*rowCount/threadCount*repeat+1;   //TODO ȷ���㷨��
    //printf("rowCount=%d , num=%d\n",rowCount , num);
    //TODO ���������߳���
    HANDLE currentThread=NULL; 
    HANDLE id=GetCurrentThread(); 
    DWORD dw=GetCurrentThreadId(); 
    //���� id ���� CTestView::OnRunThread()�����hx=0xfffffffe 
    
    DuplicateHandle(GetCurrentProcess(),id,GetCurrentProcess(),
            &currentThread,0,FALSE,DUPLICATE_SAME_ACCESS);
    vector<HANDLE> vh;
    while(rowIndex<rowCount)
    {
        //wait(s(3));
        if(runningThread >= threadCount)
        {
            //�ﵽ����߳���ʱ���ȴ���ǰ�߳̽������ٲ����µ��߳�
            SuspendThread(currentThread);
        }
        
        //Ϊ�̺߳��������ڴ�ռ�
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
        
        //ͬ��runningThread
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
    MulParam * p = (MulParam *)pParam;  //��ȡ����

    Mul(p->node , p->row , p->num);

    //ͬ��runningThread����ʹ����ʱ����Ƿ���Ч
    WaitForSingleObject(hMutex,INFINITE);
    runningThread--;
    ReleaseMutex(hMutex);
    //signal(s(3));
    ResumeThread(p->h);
    
    //�ͷű���������ڴ棨���̷߳���ģ�
    HeapFree(GetProcessHeap(),0,pParam);
    return 0;
}


#endif
