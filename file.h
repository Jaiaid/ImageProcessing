#ifndef FILE_H
#define FILE_H

#include <fstream>
#include <cstring>

#include "imat.h"

class File
{
protected:
	char typeExtension[16];
	char *name;		
	bool existance;				//true if file used to instantiate really exists
public:
	File(char *name);
	
	bool exists()
	{
		return this->existance;
	}
	
	virtual IMat * extract() = 0;
	virtual int save(char *destFileName, IMat *image, ...) = 0;
	
	~File()
	{
		delete[] this->name;
	}
};


File::File(char *name)
{
	std::ifstream fin;
	this->name = new char[strlen(name)+1];
	strcpy(this->name, name);
	
	fin.open(name,std::ios::binary);
	
	if(!fin) this->existance = false;
	else this->existance = true;
	
	fin.close();
}

#endif
