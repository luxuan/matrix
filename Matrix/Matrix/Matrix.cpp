
#include "fetch.h"
#include "parallel.h"
#include "parallel_fc.h"
#include "result.h"
#include "direct.h"
#include "tradition.h"
#include "struct.h"
#include <vector>
#include <time.h>
#include <windows.h>
#include <iostream>

using namespace std;

#define ALG_NUM 13		//�㷨����
int totalRow;			//��������

char * qqmatrixFile = "data\\matrix.mtx";
char * qqvectorFile = "data\\matrix_b.mtx";
void menu();
int trad();				//��ͳ�㷨
int csr_sse(int coreMax);			//CSR+SSE
int dir_sse(int coreMax,int fileMax,int fileCache);			//ֱ��ȡ������+SSE�Ż�

int getWay;				//���ݶ��뷽ʽ 0Ϊcsr 1Ϊ��Ԥȡ 2ΪԤȡ
int sseWay;				//�Ƿ�SSE�Ż� 0Ϊ������SSE�Ż� 1Ϊ����SSE�Ż�
int calWay;				//���㷽ʽ 0Ϊ�ǲ��� 1Ϊ����


int main()
{
    menu();
    system("start http://localhost:50051/VIEW_RESULT/Index.aspx");
    //printf("dir_sse \n");
    //int time = dir_sse(2,4,32);
    //printf(",time=%d\n\n",time);
    //
    //printf("csr_sse \n");
    //time = csr_sse(2);
    //printf(",time=%d\n",time);
    
 //   MulNode *node = GetMulNode(qqmatrixFile,qqvectorFile);    
	//totalRow=node->row;
	//float * f = Compute(1 , node);
	////
	//StartFileReturn * sf = StartFile(qqmatrixFile , qqvectorFile ,1, 100);
 //   //printf("column=%d,row=%d,pointCount=%d\n",sf->columnCount,sf->rowCount,pointCount); 
 //   Compute(1,sf);
 //   
 //   float *f2=GetResult();
	//
	//if(isequal(f,f2,totalRow))
	//cout<<"equal"<<endl;
	//else
	//cout<<"not equal"<<endl;

    //int coreMax=5;
    //int fileMax=10;
    //int fileCache=100;   //27,28,29,30,35
    //printf("begin\n");
    //for(int i=1;i<=4;i++)
    //for(int j=2;j<=20;j+=2)
    //for(int k=2048;k<10240;k+=2048)
    //{
    //
    //    printf("coreMax=%d,fileMax=%d,fileCache=%d",i,j,k);
    //    int time = dir_sse(i,j,k);
    //    printf(",time=%d\n",time);
    //}
 	system("pause");
    return 0;
}



void menu()
{
    int coreMax = 3;    //�˷�����
    int fileMax=3;
    int fileCache = 10240;
    int read_n;
	int mul_n;
	int speed_up[ALG_NUM];
	string name[ALG_NUM];
	memset(speed_up,3,sizeof(int)*ALG_NUM);	

	int t=csr_sse(coreMax);
	int i=0;

	//���д�ͳ�㷨	
	speed_up[i]=trad();	
	name[i]="��ͳ�㷨";
	cout<<name[i]<<":"<<speed_up[i]<<endl;
	i++;//printf("%s:%d\n",name[i],speed_up[i]);
	//cout<<name[i]<<":"<<speed_up[i]<<endl;
    int fm=1;
    int cm=1;
	for( getWay=0;getWay<3;getWay++)
	{
	    if(getWay==1)
	    fm=1;
	    else if(getWay==2)
	    fm=fileMax;
	    
		for( sseWay=0;sseWay<2;sseWay++)
		{
			if(sseWay)
				sse=false;
			else
				sse=true;
			for( calWay=0;calWay<2;calWay++)
			{
			    if(calWay==0)
			    cm=1;
			    else if(calWay==1)
			    cm=coreMax;
				if(getWay==0)
				{
					speed_up[i]=csr_sse(cm);
				}
				else
				{
				    //cout<<"cm:"<<cm<<" fm:"<<fm<<" fileCache:"<<fileCache<<endl;				    			    
					speed_up[i]=dir_sse(cm,fm,fileCache);
					//cout<<i<<endl;
				}

				if(getWay==0)
					name[i]="CSR";
				else if(getWay==1)
					name[i]="��Ԥȡ";
				else
					name[i]="Ԥȡ";

				if(sseWay==0)
					name[i]=name[i]+"<br/>��SSE�Ż�";
				else
					name[i]=name[i]+"<br/>SSE�Ż�";

				if(calWay==0)
					name[i]=name[i]+"<br/>�ǲ���";
				else
					name[i]=name[i]+"<br/>����";
                //cout<<i<<" "<<name[i]<<" "<<speed_up[i]<<endl;
                cout<<name[i]<<":"<<speed_up[i]<<endl;
				i++;
			}
		}
	}

/*
	for(int i=0;i<ALG_NUM;i++)
	{
		//printf("%s:%d\n",name[i],speed_up[i]);
		cout<<name[i]<<":"<<speed_up[i]<<endl;
	}
	*/
	 char buffer[20];     
      //
	  _itoa(ALG_NUM, buffer,10 );
	  string s(buffer);
	char * infile="result.txt";
	files fp(infile,0);
	fp.putin(s+"\n");
	for(int i=0;i<ALG_NUM;i++)
	{
		_itoa( speed_up[i], buffer, 10 );
		fp.putin(name[i]+":"+buffer+"\n");
	}
	
	fp.close();
}
int csr_sse(int coreMax)
{
	//csrѹ��+sse(���߳�)
	clock_t startTime = clock();
	
    MulNode *node = GetMulNode(qqmatrixFile,qqvectorFile);    
	totalRow=node->row;
	Compute(coreMax , node);
	float * f = GetResult();
	/*
    int row = node->row;
    for(int i=0;i<row;i+=1000)
	{
	    cout<<f[i]<<endl;
	    //printf("%f\n",f[i]);
	}
	*/
	delete(f);
    delete(node->matrix);
    delete(node->xj);
    delete(node->xr);
    delete(node);
	clock_t Time1= clock(); 
	return Time1-startTime;
	
}
int trad()
{
	//��ͳ�㷨
	clock_t startTime = clock();
	float * result4;
	result4=new float[totalRow];
	memset(result4,0,sizeof(result4)*totalRow);
	tradition(qqmatrixFile , qqvectorFile, result4);
	clock_t Time4= clock();
	return Time4-startTime;
}


int dir_sse(int coreMax,int fileMax,int fileCache)
{
    clock_t startTime = clock();    //��λ����
    StartFileReturn * sf = StartFile(qqmatrixFile , qqvectorFile ,fileMax, fileCache);
    //printf("column=%d,row=%d,pointCount=%d\n",sf->columnCount,sf->rowCount,pointCount); 
    Compute(coreMax,sf);
    
    float *f=GetResult();
    /*
    int row = sf->rowCount;
    for(int i=0;i<row;i+=1000)
	{
	    cout<<f[i]<<endl;
	    //printf("%f\n",f[i]);
	}
	*/
	delete(f);
    delete(sf->pcache);
    delete(sf->v);
    delete(sf);
    return clock()-startTime;

}