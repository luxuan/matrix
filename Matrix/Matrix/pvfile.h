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

long NextFilePoint();    /** ��һ���ļ���λ�� */

bool ReadPoint(Point * , FILE *);
   
void PushInput(int _i);
void PushOutput(int _i);
int PopInput();
int PopOutput();

//TODO �������̵߳ı�����ʼ��������
Point * pcache;
int blockCount;
int pointCount;
long dataPoint; //�Ѿ���һ�����ļ���λ�ã��������
HANDLE dataPointMutex = CreateMutex(NULL,FALSE,NULL);

HANDLE inputSemaphore;  //_blockCount
HANDLE outputSemaphore; //0

int fileThreadNeedCount=3;	
int fileThreadRunningCount=0; 
HANDLE fileRunningMutex = CreateMutex(NULL,FALSE,NULL);
char * matrixFile;

//����ȫ��״̬����������˳�
//TODO ͬ�����
DWORD WINAPI FileThread(LPVOID pParam)
{
    //���parallel.h�еļ�������ʱЧҪ��ͣ���������ۼ�
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
            if(rf.tellg()>=fileMax)   //�����߽�ʱ��������һ���ļ�λ��
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

/** ��һ���ļ���λ�� */
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

/** ��������ʹ�ú󣬽�����Ϊ��д״̬ */
void PushInput(int _i)
{
    WaitForSingleObject(inputMutex,INFINITE); 
    input.push(_i);
    ReleaseMutex(inputMutex);
    ReleaseSemaphore(inputSemaphore,1,NULL);
    //printf("return input\n");
}

/** ��������д���󣬽�����Ϊ�ɶ�״̬ */
void PushOutput(int _i)
{
    WaitForSingleObject(outputMutex,INFINITE); 
    output.push(_i);
    ReleaseMutex(outputMutex);
    ReleaseSemaphore(outputSemaphore,1,NULL);
}

/** �񻺴�дλ�� */
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

/** ��ȡ�����λ�� */
int PopOutput()
{
    int value;
    clock_t startTime = clock();    //��ʼ��ʱ
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
