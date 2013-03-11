#ifndef PARALLEL_FC_H
#define PARALLEL_FC_H

#include "struct.h"
#include "mul.h"
#include "direct.h"
#include <stdio.h>
#include <vector>
using namespace std;

DWORD WINAPI FCThread(LPVOID pParam);

StartFileReturn * sf;
void Compute(int threadCount , StartFileReturn * _sf)
{
	sf = _sf;
	int rowCount = sf->rowCount;
    if(rowCount<=0) return;

    vector<HANDLE> vh;
    for(int i=0;i<threadCount;i++)
    {
        HANDLE h = CreateThread(NULL,0,FCThread,NULL,0,NULL);
        vh.push_back(h);
    }
    
    for (vector<HANDLE>::iterator iter = vh.begin() ; iter != vh.end() ; iter++ )
    {
        WaitForSingleObject(*iter,INFINITE);
    }   
    //printf("mul thread end "); 
}

DWORD WINAPI FCThread(LPVOID pParam)
{
    direct_sse(sf->pcache , sf->v);
    return 0;
}


#endif
