/*
 *An implementaion/extension of image class to ppm format 
*/
#ifndef PPM_H
#define PPM_H

#include<iostream>
#include<fstream>
#include<cstdio>
#include<cstring>

#include"imageclass.h"

using namespace std;

class PPM : public Image
{
	char *name;
public:
	PPM(char *fileName);
	void save(char *fileName,pixel_t **src,int targetWidth,int targetHeight,...);
	~PPM(){};
};


PPM::PPM(char *fileName)
{
	char header[256];
	pixel_t **pixelMatrix;
	int h,w;
	ifstream file;
	/*
 	 *this variable is used to transform pixel_t type to 1 byte format to save 
 	 *in ppm format. These is done to reduce number of call to file.write
	*/
	unsigned char **transformedMatrix;

	file.open(fileName,ios::binary);
	if(!file){
		this->setExistance(false);
		this->setPixelMatrix(NULL);
		return;
	}
	else{
		this->setExistance(true);
	}

	this->name = new char[strlen(fileName)+1];
	strcpy(this->name,fileName);
	/*
	 *extracting the header 
	*/
	file.getline(header,255);
	//extracting comment if any 
	file.getline(header,255,'\n');
	if(header[0]=='#'){
		do
		{
			file.getline(header,255,'\n');
		}
		while(header[0]=='#');
	}
	//comment extraction finished
	sscanf(header,"%d%d",&w,&h);
	file>>header;
	file.get();
	/*
	 * extraction finished
	*/
	this->setHeight(h),this->setWidth(w);
	this->setColoDepthBit(8);
	this->setPixelAttribute(3);

	/*
	 *extract pixel array and save the pointer as base object attribute 
	*/ 
	transformedMatrix = new unsigned char* [h];
	for(int l=0,size=w*this->getPixelAttribute();l<h;l++)
	{
		transformedMatrix[l] = new unsigned char [size];
		file.read((char *)transformedMatrix[l],size);			//read each row
	}

	pixelMatrix = new pixel_t *[h];
	for(int l=0,size=w*this->getPixelAttribute();l<h;l++)
	{
		pixelMatrix[l] = new pixel_t [size];
		for(int l1=0,gap=this->getPixelAttribute();l1<size;l1+=gap)
		{
			pixelMatrix[l][l1] = (unsigned int)transformedMatrix[l][l1];
			pixelMatrix[l][l1+1] = (unsigned int)transformedMatrix[l][l1+1];
			pixelMatrix[l][l1+2] = (unsigned int)transformedMatrix[l][l1+2];
		}
	}
	this->setPixelMatrix(pixelMatrix);

	file.close();

	for(int l=0;l<h;l++)
	{
		delete[] transformedMatrix[l];
	}
	delete[] transformedMatrix;
}

void PPM::save(char *fileName,pixel_t **srcPixelMatrix,int targetWidth,int targetHeight,...)
{
	ofstream file;
	/*
	 *this variable is used to transform pixel_t type to 1 byte format to save 
	 *in ppm format. These is done to reduce number of call to file.write
	*/
	unsigned char **transformedMatrix;

	file.open(fileName,ios::binary);
	
	if(!file){
		cout<<"Saving output file failed"<<endl;
	}
	
	file<<"P6"<<endl;
	file<<targetWidth<<' '<<targetHeight<<endl;
	file<<"255"<<endl;
	
	transformedMatrix = new unsigned char* [targetHeight];
	for(int l=0,size=targetWidth*this->getPixelAttribute();l<targetHeight;l++)
	{
		transformedMatrix[l] = new unsigned char [size];
		for(int l1=0,gap=this->getPixelAttribute();l1<size;l1+=gap)
		{
			transformedMatrix[l][l1] = (unsigned char)srcPixelMatrix[l][l1];
			transformedMatrix[l][l1+1] = (unsigned char)srcPixelMatrix[l][l1+1];
			transformedMatrix[l][l1+2] = (unsigned char)srcPixelMatrix[l][l1+2];
		}
	}
	for(int l=0,size=targetWidth*this->getPixelAttribute();l<targetHeight;l++)
	{
		file.write((char *)transformedMatrix[l],size);
	}
	
	file.close();
	
	for(int l=0;l<targetHeight;l++)
	{
		delete[] transformedMatrix[l];
	}
	delete[] transformedMatrix;
}

#endif
