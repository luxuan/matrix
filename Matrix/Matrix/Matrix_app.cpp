
#include "fetch.h"
#include "parallel.h"
#include "direct.h"
#include "tradition.h"
#include <vector>
#include <time.h>
#include <iostream>
#include <math.h>
using namespace std;

int main()
{
    double diff_time;
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

			
    //TODO delete node
    
    diff_time = (double)( clock() - startTime ) / CLOCKS_PER_SEC;   
    printf("used time = %f s\n",diff_time);
    printf("total time = %f s\n",diff_time * coreMax);
    cin>>coreMax;
	system("pause");
    return 0;
	/*
	vector< matrixvalue2> coo;
	matrixvalue2 * m;
	int n=500000;
	int t=n;
	while(t--)
	{
		m=new matrixvalue2;
		m->col=0;
		m->row=0;
		m->value=0;
		coo.push_back(*m);
	}
	t=n;
	while(t--)
	{
		cout<<coo[t].value;
	}
	system("pause");*/
}