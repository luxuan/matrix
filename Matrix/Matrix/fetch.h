#ifndef FETCH_H
#define FETCH_H

#include "struct.h"
#include "files.h"
# include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
using namespace std;

/**
    从文件中读取数据
    matrixFile:矩阵文件名（如mutix.mtx）
    vectorFile:向量文件名（如mutix_b.mtx）
*/
typedef struct matrixvalue
{
	int col;
	int row;
	float value;
}matrixvalue;

bool cmp(matrixvalue a, matrixvalue b) {
	if (a.row == b.row) {
		return a.col < b.col;
	}
	return a.row < b.row;
}

MulNode * GetMulNode(char * matrixFile , char * vectorFile)
{
    MulNode * node = new MulNode ;
	int i,j;
    // TODO jzw完善

	files vec(vectorFile);
	int n=vec.getint();
	node->v=new float[n];
	for(i=0;i<n;i++)
	{
		node->v[i]=vec.getfloat();
	}
	vec.close();
    files mat(matrixFile);
	
		node->column=mat.getint();
		node->row=mat.getint();
		node->num=mat.getint();
	
	
	vector< matrixvalue> coo;
	matrixvalue *m;
	node->xr=new int[node->row+2];
	memset(node->xr,0,sizeof(node->xr)*(node->row+2));
	node->xj=new int[node->num*4];	
	memset(node->xj,0,sizeof(node->xj)*node->num*4);
	node->matrix=new float[node->num*4];
	memset(node->matrix,0,sizeof(node->matrix)*node->num*4);
	for(i=0;i<node->num;i++)
	{
		m=new matrixvalue;
		m->col=mat.getint();
		m->row=mat.getint();
		m->value=mat.getfloat();//printf("%d %d %f\n",m->col,m->row,m->value);//cout<<" "<<m->col<<" "<<m->row<<m->value<<endl;
		coo.push_back(*m);
		node->xr[m->row-1]++;
		delete m;
	}
	mat.close();
	
	sort(coo.begin(), coo.end(), cmp);
	
	int sum=0;
	int current=0;
	for(i=0;i<node->row;i++)
	{
		for(j=0;j<node->xr[i];j++)
		{
			node->matrix[sum]=coo[current].value;			
			node->xj[sum]=coo[current].col-1;
			sum++;
			current++;
		}
		int t=4-node->xr[i]%4;
		while(t%4)
		{
			node->matrix[sum]=0;
			node->xr[i]++;
			node->xj[sum]=0;
			t--;
			sum++;
		}
		//原先node->xr每个数据表示该行的数据数目，现在修改后表示每行第一个数的位置
		if(i)
		{
			node->xr[i]=node->xr[i]+node->xr[i-1];
		}
	}
	
	//修改node->xr数据表示的意思，为并行做准备
	for(i=node->row;i>0;i--)
	{
		if(node->xr[i-1]<0)
		{
			cout<<"xj[0]<0"<<endl;
		}
		node->xr[i]=node->xr[i-1];
		
	}
	node->xr[0]=0;

	node->num=sum;
	coo.clear();
/*
	for(i=0;i<node->num;i++)
	{
		cout<<node->matrix[i]<<" ";
	}
	cout<<'\n';
	for(i=0;i<node->num;i++)
	{
		cout<<node->xj[i]<<" ";
	}
	cout<<'\n';
	for(i=0;i<=node->row;i++)
	{
		cout<<node->xr[i]<<" ";
	}
	*/
    return node;
}



#endif
