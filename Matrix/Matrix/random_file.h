#ifndef RANDOM_FILE_H
#define RANDOM_FILE_H

#include "files.h"
#include <stdio.h>
class RandomFile:public files
{
	char ch;
public :
    RandomFile(char *name):files(name)
    {
    }
	void adapt()
	{
	    while(fp.get(ch)&& ch != '\n');  //debugged:while(fp.get()!='\n');
	}
	bool read(Point *p)
    {
        fp>>(p->column);
        fp>>(p->row);
        fp>>(p->value);
        //之后指向换行
		return !fp.eof();
		/*
		int i=0;
		while((line[i]=fp.get())!=' ')i++;
		line[i]='\0';
		p->column = atoi(line);
		i=fp.tellg();
		
		i=0;
		while((line[i]=fp.get())!=' ')i++;
		line[i]='\0';
		p->row = atoi(line);
		i=fp.tellg();
		
		i=0;
		while((line[i]=fp.get())!='\n')i++;
		line[i]='\0';
		p->value = atof(line);
		i=fp.tellg();
        */
    }  
    void close()
    {
        fp.close();
    } 

};


#endif