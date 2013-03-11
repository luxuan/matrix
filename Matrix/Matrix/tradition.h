#ifndef TRADITION_H
#define TRADITION_H

#include "files.h"
#include <xmmintrin.h>
#include "direct.h"
#include <math.h>
#include "struct.h"
void direct(char * matrixFile , char * vectorFile,float * reslut)
{
	float * v;
	vector< Point> coo;
	
	files vec(vectorFile);
	int n=vec.getint();
	v=new float[n];
	int i;
	for(i=0;i<n;i++)
	{
		v[i]=vec.getfloat();
	}
	vec.close();
    files mat(matrixFile);
	
		int column=mat.getint();
		int row=mat.getint();
		int num2=mat.getint();	
	//reslut=new float[row];
	//memset(reslut,0,sizeof(reslut)*row);
	Point *m;
	for(i=0;i<num2;i++)
	{
		m=new Point;
		m->column=mat.getint()-1;
		m->row=mat.getint()-1;
		m->value=mat.getfloat();
		coo.push_back(*m);
		
		delete m;
	}
	mat.close();
	//取数据结束
	//进行计算
	int line=0;
	int num=num2;
	
	for(i=line;i<line+num;i++)
	{
		//reslut[coo[i].row]=reslut[coo[i].row]+v[coo[i].column]*coo[i].value;
		Add(v[coo[i].column]*coo[i].value,coo[i].row);
	}
}
void tradition(char * matrixFile , char * vectorFile,float * reslut)
{
	float * v;
	vector< Point> coo;
	
	files vec(vectorFile);
	int n=vec.getint();
	v=new float[n];
	int i;
	for(i=0;i<n;i++)
	{
		v[i]=vec.getfloat();
	}
	vec.close();
    files mat(matrixFile);
	
		int column=mat.getint();
		int row=mat.getint();
		int num2=mat.getint();	
	float **matrix;
	matrix=new float*[row];
	for(i=0;i<row;i++)
	{
		matrix[i]=new float[column];
		memset(matrix[i],0,sizeof(matrix)*column);
	}
	
	//reslut=new float[row];
	//memset(reslut,0,sizeof(reslut)*row);

	int c,r;
	for(i=0;i<num2;i++)
	{
		c=mat.getint()-1;
		r=mat.getint()-1;
		matrix[r][c]=mat.getfloat();
	}
	
	mat.close();
	//取数据结束
	//进行计算
	//int line=0;
	//int num=num2;
	
	for(i=0;i<row;i++)
	{
		for(int j=0;j<column;j++)
		{
			reslut[i]=reslut[i]+v[j]*matrix[i][j];
		}
	}
}
bool isequal(float * result1,float * result2,int row)
{
	for(int i=0;i<row;i++)
	{
		if(fabs((result1[i]-result2[i])/result2[i])>0.00001)
		{
		cout<<"not equal\t"<<result1[i]<<"\t"<<result2[i]<<" "<<fabs((result1[i]-result2[i])/result2[i])<<" "<<i<<endl;
		//return false;
		}
	}
	return true;
}
#endif