#ifndef FILES_H
#define FILES_H

#define INTERROR -10000

#include <fstream>
#include <iostream>
#include <string>
using namespace std;

class files
{
	char * filename;
	char tempchar;
	string temp;
	protected:
	fstream fp ;

	public :
	files(char *name)
	{
		filename=name;
		fp.open(filename, ios::in);
	}
	
	files(char *name,int n)
	{
		filename=name;
		fp.open(filename, ios::out);
	}
	bool putin(string b)
	{
		if(fp<<b)
			return true;
		else
			return false;
	}
	int getint()
	{/*
		temp="";
		ignore();
		temp=tempchar;
		while(fp.get(tempchar)&&isdigit(&tempchar))
		{
		temp=temp+tempchar;
		
		//cout<<test;
		}
		if(temp==""&&fp.eof())
		return INTERROR;
		else
		return (atoi(temp.c_str()));
		*/
		int t;
		fp>>t;
		return t;
		
	}
	float getfloat()
	{
		/*
		temp="";
		ignore();
		temp=tempchar;
		while(fp.get(tempchar)&&(isdigit(&tempchar)||tempchar=='.'))
		{
		temp=temp+tempchar;
		}
		if(temp==""&&fp.eof())
		return INTERROR;
		else
		return (float)(atof(temp.c_str()));
		)*/
		float t;
		fp>>t;
		return t;
	}
	long tellg()
	{
	    return fp.tellg();
	}
	void seekg(long _offset)
	{
	    fp.seekg(_offset);
	}
    
	void close()
	{
		fp.close();
	}
	void ignore()
	{
		while(fp.get(tempchar)&&(tempchar==' '||tempchar=='\n'||tempchar=='\t'||tempchar=='\r'))
		;
	}
	bool isdigit(char * ch)
	{
		if((*ch)>='0'&&(*ch)<='9')
			return true;
		else return false;
	}
};
#endif