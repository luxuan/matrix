
#ifndef STRUCT_H
#define STRUCT_H
#include <windows.h>

bool sse=true;		//�Ƿ���SSE�Ż�

/** �洢������������� */
struct MulNode
{
    float *matrix;  //����
    int *xj;    //��matrix��˳�����δ�����з���Ԫ�ص��к�
    int *xr;    //ÿ���е�һ������Ԫ�ر�������matrix�е�λ��
    int row;    //�������������ڿ��ƶ��߳��������
    int column; //���������������ô�����δʹ�ÿɽ���ɾ��
	int num;	//���������ݵĸ���
    
    float *v;   //����	
};

/** ���߳�����Ϊ�������ݵĽṹ�� */
struct MulParam
{   
    MulNode * node; //����CSR��ʽ����ľ����һά�����Ľṹ��
    int row;    //��ʼ������к�
    int num;    //Ҫ���������
    
    HANDLE h;   //���̣߳����˷�����������Ѹ��߳�
};

struct Point
{
    int column;
    int row ;
    float value;
};

struct StartFileReturn
{
    Point * pcache; //�����׵�ַ
    float *v;   //����ָ��
    int rowCount;   //��������
    int columnCount;   //��������
};
#endif