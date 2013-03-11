
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
    clock_t startTime = clock();    //单位毫秒
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
    clock_t startTime = clock();    //单位毫秒
    int coreMax = 3;
	vector <int > a;
    char * matrixFile = "matrix.mtx";
    char * vectorFile = "matrix_b.mtx";

	
	//csr压缩+sse
    MulNode *node = GetMulNode(matrixFile,vectorFile);    
	float * result1;
	result1=new float[node->row];
	memset(result1,0,sizeof(float)*node->row);
	Mul( node , 0 , node->row-1 ,result1);
	clock_t Time1= clock(); 
    //float * result = Compute(coreMax , node);

	//取出直接计算+sse
	float * result2;
	result2=new float[node->row];
	memset(result2,0,sizeof(result2)*node->row);
	direct_sse(matrixFile , vectorFile, result2);
	clock_t Time2= clock(); 

	//取出直接计算
	float * result3;
	result3=new float[node->row];
	memset(result3,0,sizeof(result3)*node->row);
	direct(matrixFile , vectorFile, result3);
	clock_t Time3= clock();

	//传统算法
	float * result4;
	result4=new float[node->row];
	memset(result4,0,sizeof(result4)*node->row);
	tradition(matrixFile , vectorFile, result4);
	clock_t Time4= clock();

	//判断计算结果是否相同
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

	//计算各个算法的计算时间
	cout<<"time1:"<<Time1-startTime<<" "<<"time2:"<<Time2-Time1<<" "<<"time3:"<<Time3-Time2<<" "<<"time4:"<<Time4-Time3<<endl;


}

#define ALG_NUM 13		//算法个数
int totalRow;			//矩阵行数

char * qqmatrixFile = "matrix.mtx";
char * qqvectorFile = "matrix_b.mtx";
int trad();				//传统算法
int csr_sse();			//CSR+SSE
int dir_sse();			//直接取数运算+SSE优化
int getWay;				//数据读入方式 0为csr 1为非预取 2为预取
int sseWay;				//是否SSE优化 0为不进行SSE优化 1为进行SSE优化
int calWay;				//运算方式 0为非并行 1为并行
int main()
{
	system("color 0a");
	/*
	printf("*******************************************\n");
	printf("               矩阵乘优化\n");
	printf("*******************************************\n");
	printf("1.使用默认参数\n");
	printf("2.手动设定参数\n");
	
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

	//运行传统算法	
	speed_up[i]=trad();	
	name[i]="传统算法";
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
					name[i]="非预取";
				else
					name[i]="预取";

				if(seeWay==0)
					name[i]=name[i]+"+非SSE优化";
				else
					name[i]=name[i]+"+SSE优化";

				if(calWay==0)
					name[i]=name[i]+"+非并行";
				else
					name[i]=name[i]+"+并行";

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
	//csr压缩+sse
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
	//传统算法
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
	//取出直接计算+sse
	clock_t startTime = clock();
	float * result2;
	result2=new float[totalRow];
	memset(result2,0,sizeof(result2)*totalRow);
	direct_sse(qqmatrixFile , qqvectorFile, result2);
	clock_t Time2= clock();
	return Time2-startTime;
}