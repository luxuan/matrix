#ifndef PVFILE_H
#define PVFILE_H
#define BLOCK_SIZE 1024	//cache
#define FILE_READ_SIZE 1024	//file

#include "random_file.h"
#include "struct.h"
#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <queue>

long NextFilePoint();    /** 下一个文件读位置 */

bool ReadPoint(Point * , FILE *);
   
void PushInput(int _i);
void PushOutput(int _i);
int PopInput();
int PopOutput();

//TODO 启动多线程的变量初始化、销毁
Point * pcache;
int blockCount;
int pointCount;
long dataPoint; //已经下一个读文件的位置，互斥访问
HANDLE dataPointMutex = CreateMutex(NULL,FALSE,NULL);

HANDLE inputSemaphore;  //_blockCount
HANDLE outputSemaphore; //0

int fileThreadNeedCount=3;	
int fileThreadRunningCount=0; 
HANDLE fileRunningMutex = CreateMutex(NULL,FALSE,NULL);
char * matrixFile;

//根据全局状态，自身控制退出
//TODO 同步检测
DWORD WINAPI FileThread(LPVOID pParam)
{
    //相比parallel.h中的计数器的时效要求低，启动后才累加
    WaitForSingleObject(fileRunningMutex,INFINITE); 
    fileThreadRunningCount++;
    ReleaseMutex(fileRunningMutex);
    
    RandomFile rf(matrixFile);
    Point * pWrite;
    Point * pMax;
    long fileMax=-1;
    bool tag = true;    //TODO out
    while(fileThreadRunningCount<=fileThreadNeedCount && tag) //TODO
    {
        //printf("x ");
        int index = PopInput(); // NextMemoryPoint()
        //printf("y ");
        //printf("index=%d\n",index);
        pWrite = index*BLOCK_SIZE + pcache;    
        pMax = pWrite + BLOCK_SIZE;
        while(pWrite < pMax)
        {
            if(rf.tellg()>=fileMax)   //超过边界时，请求下一个文件位置
            {
                //printf("rf.tellg()=%d,fileMax=%d ",rf.tellg(),fileMax);
                long fi = NextFilePoint();  //ok
                fileMax = fi + FILE_READ_SIZE;  //debugged: (+=) ->(=)
                rf.seekg(fi);
                rf.adapt();
                //printf("fi=%d,tellg()=%d\n",fi,rf.tellg());
            }
            if(!rf.read(pWrite))
            {
                //printf("eof=%d\n",feof(fp));    //Visual C++ :16
                //printf("pMax-pWrite=%d\n",pMax-pWrite);
                memset(pWrite,0,sizeof(Point)*(pMax-pWrite));
                WaitForSingleObject(fileRunningMutex,INFINITE); 
                fileThreadRunningCount--;
                //printf("-- %d ",fileThreadRunningCount);
                ReleaseMutex(fileRunningMutex);
                tag = false; 
                break;
            }
            //printf("pWrite:  column=%d, row=%d,value=%f\n",pWrite->column,pWrite->row,pWrite->value);
            (pWrite->row)--;    //debugged:add
            (pWrite->column)--; //debugged:add
            pWrite++;
        }//while(pWrite < pMax)
        //printf("before PushOutput\n");
        //printf("x ");
        PushOutput(index);
        //printf("y \n");
        //printf("after PushOutput\n");
    }//while(fileThreadRunningCount<=fileThreadNeedCount && tag)
    rf.close();
    //printf("out running=%d,need=%d \n",fileThreadRunningCount,fileThreadNeedCount);
    return 0;
}

/** 下一个文件读位置 */
long NextFilePoint()
{
    long rLong;
    WaitForSingleObject(dataPointMutex,INFINITE);
    rLong = dataPoint;
    dataPoint += FILE_READ_SIZE;
    ReleaseMutex(dataPointMutex);
    return rLong;
}

queue<int> input;
queue<int> output;
HANDLE inputMutex = CreateMutex(NULL,FALSE,NULL);
HANDLE outputMutex = CreateMutex(NULL,FALSE,NULL);

/** 缓存数据使用后，将其设为可写状态 */
void PushInput(int _i)
{
    WaitForSingleObject(inputMutex,INFINITE); 
    input.push(_i);
    ReleaseMutex(inputMutex);
    ReleaseSemaphore(inputSemaphore,1,NULL);
    //printf("return input\n");
}

/** 缓存数据写满后，将其设为可读状态 */
void PushOutput(int _i)
{
    WaitForSingleObject(outputMutex,INFINITE); 
    output.push(_i);
    ReleaseMutex(outputMutex);
    ReleaseSemaphore(outputSemaphore,1,NULL);
}

/** 获缓存写位置 */
int PopInput()
{
    int value;
    //printf("x ");
    WaitForSingleObject(inputSemaphore,INFINITE);
    //printf("y ");
    WaitForSingleObject(inputMutex,INFINITE); 
    value = input.front();
    input.pop();
    ReleaseMutex(inputMutex);
    return value;
}

/** 获取缓存读位置 */
int PopOutput()
{
    int value;
    clock_t startTime = clock();    //开始计时
    //printf("a ");
    WaitForSingleObject(outputSemaphore,INFINITE);
    //printf("b \n");
    /*if(clock() - startTime >10)
    {
        wait();
        fileThreadNeedCount++;
        signal();
        CreateThread(NULL,0,FileThread,pParam,0,NULL);
    }
    */
    WaitForSingleObject(outputMutex,INFINITE); 
    //debugged: if-else (isFileRunning)
    if(output.empty())
    {
        //printf("Release output\n");
        value = -1;
        ReleaseSemaphore(outputSemaphore,1,NULL);
    }
    else
    {
        value = output.front();
        output.pop();
        if(output.empty() && fileThreadRunningCount == 0)
            ReleaseSemaphore(outputSemaphore,1,NULL);
    }
    ReleaseMutex(outputMutex);
    return value;
}
#endif
