#ifndef RESULT_H
#define RESULT_H

#include "pvfile.h"
#include "random_file.h"
#include <windows.h>

float * result; //�ں���StartFile�г�ʼ��
HANDLE resultMutex = CreateMutex(NULL,FALSE,NULL);

/** ����������ʽ����result */
void Add(float value,int row)
{
    WaitForSingleObject(resultMutex,INFINITE);
    //if(row==0 && value!=0)printf("update =%d\n",value);
    result[row] += value;
    ReleaseMutex(resultMutex);
}
void InitResult(int row)
{
    result=new float[row]; 
    memset(result,0,sizeof(float)*row);
}
float * GetResult()
{
    return result;
}

StartFileReturn * StartFile(char * _matrixFile , char * _vectorFile , int _threadCount ,int _blockCount)
{
    matrixFile = _matrixFile;
    fileThreadNeedCount = _threadCount;
    //printf("fileThreadNeedCount=%d\n",fileThreadNeedCount);
    blockCount=_blockCount;
    pcache=new Point[_blockCount*BLOCK_SIZE];
    memset(pcache,0,sizeof(Point)*_blockCount*BLOCK_SIZE);  //init pcache
    //memset(cacheTag,true,sizeof(bool)*_blockCount);    //init cacheTag
    while(!input.empty()) input.pop();  //debugged: ���̱߳�����ʼ��
    while(!output.empty()) output.pop();    //debugged: ���̱߳�����ʼ��
    for(int i=0;i<_blockCount;i++){ input.push(i); }
    inputSemaphore = CreateSemaphore(NULL,_blockCount,_blockCount,NULL);
    outputSemaphore = CreateSemaphore(NULL,0,_blockCount,NULL);
    
    //read title
    files title(matrixFile);
	int columnCount=title.getint();
	int rowCount=title.getint();
	pointCount=title.getint();
    dataPoint = title.tellg()-2;    //ǰ��ͬ��
    
    title.close();
    
    InitResult(rowCount);    
    
    //read vector
    float * v=new float[columnCount];
    files vec(_vectorFile);
	vec.getint();
	for(int i=0;i<columnCount;i++)
	{
		v[i]=vec.getfloat();
	}
	vec.close();
    
    //��������������߳�
    for(int i=0;i<fileThreadNeedCount;i++)
    {
        CreateThread(NULL,0,FileThread,NULL,0,NULL);
    }
    StartFileReturn * sfr = new StartFileReturn;
    sfr->columnCount = columnCount;
    sfr->rowCount = rowCount;
    sfr->pcache = pcache;
    sfr->v = v; 
    return sfr;
}


#endif


