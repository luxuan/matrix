
#include "fetch.h"
#include "parallel.h"
#include "result.h"
#include "direct.h"
#include "tradition.h"
#include "struct.h"
#include <vector>
#include <time.h>
#include <iostream>
#include <string>
#include <stdio.h>
using namespace std;

int main1()
{
    double diff_time;
    int coreMax = 2;
	vector <int > a;
    char * qqmatrixFile = "matrix.mtx";
    char * qqvectorFile = "matrix_b.mtx";
    MulNode *node = GetMulNode(qqmatrixFile,qqvectorFile);
	//float * result;
    clock_t startTime = clock();    //��λ����
	/*
	result=new float[node->row];
	memset(result,0,sizeof(result)*node->row);
	Mul( node , 0 , node->row ,result);
	*/
    float * result = Compute(coreMax , node);
    //Sleep(1000);
	for(int i=0;i<node->row;i+=1000)
	{
	    cout<<result[i]<<endl;
	}
    //TODO delete node
    
    //printf(""
    StartFileReturn * sf = StartFile(qqmatrixFile , qqvectorFile , coreMax*4);
    printf("column=%d,row=%d\n",sf->columnCount,sf->rowCount); 
    //Next();
    //Next();
    
    diff_time = (double)( clock() - startTime ) / CLOCKS_PER_SEC;   
    printf("used time = %f s\n",diff_time);
 	system("pause");
    return 0;
}

void viewCompare()
{
    clock_t startTime = clock();    //��λ����
    int coreMax = 3;
	vector <int > a;
    char * matrixFile = "matrix.mtx";
    char * vectorFile = "matrix_b.mtx";

	
	//csrѹ��+sse
    MulNode *node = GetMulNode(matrixFile,vectorFile);    
	float * result1;
	result1=new float[node->row];
	memset(result1,0,sizeof(float)*node->row);
	Mul( node , 0 , node->row-1 ,result1);
	clock_t Time1= clock(); 
    //float * result = Compute(coreMax , node);

	//ȡ��ֱ�Ӽ���+sse
	float * result2;
	result2=new float[node->row];
	memset(result2,0,sizeof(result2)*node->row);
	direct_sse(matrixFile , vectorFile, result2);
	clock_t Time2= clock(); 

	//ȡ��ֱ�Ӽ���
	float * result3;
	result3=new float[node->row];
	memset(result3,0,sizeof(result3)*node->row);
	direct(matrixFile , vectorFile, result3);
	clock_t Time3= clock();

	//��ͳ�㷨
	float * result4;
	result4=new float[node->row];
	memset(result4,0,sizeof(result4)*node->row);
	tradition(matrixFile , vectorFile, result4);
	clock_t Time4= clock();

	//�жϼ������Ƿ���ͬ
	if(isequal(result1,result2,node->row))
		cout<<"1=2"<<endl;
	else
		cout<<"1not=2"<<endl;
	if(isequal(result2,result3,node->row))
		cout<<"2=3"<<endl;
	else
		cout<<"2not=3"<<endl;
	if(isequal(result3,result4,node->row))
		cout<<"3=4"<<endl;
	else
		cout<<"3not=4"<<endl;

	//��������㷨�ļ���ʱ��
	cout<<"time1:"<<Time1-startTime<<" "<<"time2:"<<Time2-Time1<<" "<<"time3:"<<Time3-Time2<<" "<<"time4:"<<Time4-Time3<<endl;


}

#define ALG_NUM 13		//�㷨����
int totalRow;			//��������

char * qqmatrixFile = "matrix.mtx";
char * qqvectorFile = "matrix_b.mtx";
int trad();				//��ͳ�㷨
int csr_sse();			//CSR+SSE
int dir_sse();			//ֱ��ȡ������+SSE�Ż�
int getWay;				//���ݶ��뷽ʽ 0Ϊcsr 1Ϊ��Ԥȡ 2ΪԤȡ
int sseWay;				//�Ƿ�SSE�Ż� 0Ϊ������SSE�Ż� 1Ϊ����SSE�Ż�
int calWay;				//���㷽ʽ 0Ϊ�ǲ��� 1Ϊ����
int main()
{
	system("color 0a");
	/*
	printf("*******************************************\n");
	printf("               ������Ż�\n");
	printf("*******************************************\n");
	printf("1.ʹ��Ĭ�ϲ���\n");
	printf("2.�ֶ��趨����\n");
	
	system("pause");
	system("start http://127.0.0.1/index.asp?id=hhhhhhhhhhhhhhhhh");
	*/
	int read_n;
	int mul_n;
	int speed_up[ALG_NUM];
	string name[ALG_NUM];
	memset(speed_up,3,sizeof(int)*ALG_NUM);	

	int t=csr_sse();
	int i=0;

	//���д�ͳ�㷨	
	speed_up[i]=trad();	
	name[i]="��ͳ�㷨";
	i++;//printf("%s:%d\n",name[i],speed_up[i]);
	//cout<<name[i]<<":"<<speed_up[i]<<endl;

	for(int getWay=0;getWay<3;getWay++)
	{
		for(int seeWay=0;seeWay<2;seeWay++)
		{
			if(sseWay)
				sse=false;
			else
				sse=true;
			for(int calWay=0;calWay<2;calWay++)
			{
				if(getWay)
				{
					speed_up[i]=csr_sse();
				}
				else
				{
					speed_up[i]=dir_sse();
				}

				if(getWay==0)
					name[i]="CSR";
				else if(getWay==1)
					name[i]="��Ԥȡ";
				else
					name[i]="Ԥȡ";

				if(seeWay==0)
					name[i]=name[i]+"+��SSE�Ż�";
				else
					name[i]=name[i]+"+SSE�Ż�";

				if(calWay==0)
					name[i]=name[i]+"+�ǲ���";
				else
					name[i]=name[i]+"+����";

				i++;
			}
		}
	}


	for(int i=0;i<ALG_NUM;i++)
	{
		//printf("%s:%d\n",name[i],speed_up[i]);
		cout<<name[i]<<":"<<speed_up[i]<<endl;
	}
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
	system("pause");
	return 0;
}

int csr_sse()
{
	//csrѹ��+sse
	clock_t startTime = clock();
    MulNode *node = GetMulNode(qqmatrixFile,qqvectorFile);    
	float * result1;
	result1=new float[node->row];
	memset(result1,0,sizeof(float)*node->row);
	Mul( node , 0 , node->row-1 ,result1);
	clock_t Time1= clock(); 
	totalRow=node->row;
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
int dir_sse()
{
	//ȡ��ֱ�Ӽ���+sse
	clock_t startTime = clock();
	float * result2;
	result2=new float[totalRow];
	memset(result2,0,sizeof(result2)*totalRow);
	direct_sse(qqmatrixFile , qqvectorFile, result2);
	clock_t Time2= clock();
	return Time2-startTime;
}