#ifndef MUL_H
#define MUL_H

#include "struct.h"
#include <xmmintrin.h>
#include "result.h"
/**
    计算矩阵与向量的乘积,将结果写在p->result中
    node:含用CSR格式保存的矩阵和一维向量的结构体
    row:开始计算的行号
    num:要计算的行数
    result:以引用的方式返回计算结果，如result[num]=value;
*/

bool Mul(MulNode * node , int row , int num )
{
    // TODO jzw完善
	//result=new float[node->row];
	//memset(result,0,sizeof(float)*node->row);
	int i,j;
	__m128 r1,res;
	if(sse)
	{
		for(i=row;i<row+num;i++)
		{
			for(j=node->xr[i];j<node->xr[i+1];j+=4)
			{
				r1.m128_f32[0]=node->matrix[j];
				r1.m128_f32[1]=node->matrix[j+1];
				r1.m128_f32[2]=node->matrix[j+2];
				r1.m128_f32[3]=node->matrix[j+3];
				res = _mm_mul_ps(_mm_set_ps(node->v[node->xj[j+3]], node->v[node->xj[j+2]], node->v[node->xj[j+1]], node->v[node->xj[j]]), r1);			
				//result[i]=result[i]+res.m128_f32[0]+res.m128_f32[1]+res.m128_f32[2]+res.m128_f32[3];
				Add(res.m128_f32[0]+res.m128_f32[1]+res.m128_f32[2]+res.m128_f32[3],i);
				
				//Add(res.m128_f32[0]+res.m128_f32[1]+res.m128_f32[2]+res.m128_f32[3],i);
				//if(i==0)
				//for(int x=0;x<4;x++)
				//if(res.m128_f32[x]!=0)printf("res %d\n",res.m128_f32[x]);
			}
		}
	}
	else
	{
		for(i=row;i<row+num;i++)
		{
			for(j=node->xr[i];j<node->xr[i+1];j++)
			{				
				//result[i]=result[i]+node->matrix[j]*node->v[node->xj[j]];
				Add(node->matrix[j]*node->v[node->xj[j]],i);
			}
		}
	}

    return true;
}

#endif