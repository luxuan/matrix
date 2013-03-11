#ifndef DIRECT_H
#define DIRECT_H

#include "files.h"
#include "struct.h"
#include <xmmintrin.h>
#include "result.h"


void direct_sse(Point * _pcache,float * _v)
{
    int t;
    while((t=PopOutput()) >= 0)
    {
	    Point * p=_pcache+t*BLOCK_SIZE;
	    int i;
	    __m128 r1,res;
	    if(sse)
	    {
		    for(i=0;i<BLOCK_SIZE;i+=4,p+=4)
		    {
			    r1.m128_f32[0]=(p)->value;
			    r1.m128_f32[1]=(p+1)->value;
			    r1.m128_f32[2]=(p+2)->value;
			    r1.m128_f32[3]=(p+3)->value;
			    res = _mm_mul_ps(_mm_set_ps(_v[(p+3)->column],_v[(p+2)->column], _v[(p+1)->column], _v[(p)->column]), r1);			
			    //reslut[coo[i].row]=reslut[coo[i].row]+res.m128_f32[0];
			    //reslut[coo[i+1].row]=reslut[coo[i+1].row]+res.m128_f32[1];
			    //reslut[coo[i+2].row]=reslut[coo[i+2].row]+res.m128_f32[2];
			    //reslut[coo[i+3].row]=reslut[coo[i+3].row]+res.m128_f32[3];
			    Add(res.m128_f32[0],(p)->row);
			    Add(res.m128_f32[1],(p+1)->row);
			    Add(res.m128_f32[2],(p+2)->row);
			    Add(res.m128_f32[3],(p+3)->row);
		    }
		    /*
		    for(;i<BLOCK_SIZE;i++)
		    {
			    //reslut[coo[i].row]=reslut[coo[i].row]+v[coo[i].column]*coo[i].value;
			    Add(_v[coo[i].column]*coo[i].value,coo[i].row);
		    }
		    */
	    }
	    else
	    {
		    for(i=0;i<BLOCK_SIZE;i++,p++)
		    {
			    //reslut[coo[i].row]=reslut[coo[i].row]+v[coo[i].column]*coo[i].value;
			    Add(_v[p->column]*p->value,p->row);
		    }
	    }
	    PushInput(t);   //debugged:add by Lius
	}//while
}

#endif