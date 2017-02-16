/*
 *An implementaion/extension of image class to ppm format 
*/
#ifndef PGM_H
#define PGM_H

#include<iostream>
#include<fstream>
#include<cstdio>
#include<cstring>

#include"imageclass.h"

#define MAX_HEADER_LENGTH 255

using namespace std;

class PGM : public Image
{
	char *name;
public:
	PGM(char *fileName);
	void save(char *fileName,pixel_t **src,int targetWidth,int targetHeight,...);
	~PGM(){};
};


PGM::PGM(char *fileName)
{
	char header[MAX_HEADER_LENGTH+1];
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
	file.getline(header,MAX_HEADER_LENGTH);
	//extracting comment if any 
	file.getline(header,MAX_HEADER_LENGTH,'\n');
	if(header[0]=='#'){
		do
		{
			file.getline(header,MAX_HEADER_LENGTH,'\n');
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
	this->setPixelAttribute(1);

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
	for(int l=0,size=w*CHANNEL_PER_PIXEL;l<h;l++)
	{
		pixelMatrix[l] = new pixel_t [size];
		for(int l1=0,l2=0,gap=this->getPixelAttribute();l1<size;l1+=CHANNEL_PER_PIXEL,l2+=gap)
		{
			pixelMatrix[l][l1] = (unsigned int)transformedMatrix[l][l2];
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

void PGM::save(char *fileName,pixel_t **srcPixelMatrix,int targetWidth,int targetHeight,...)
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
	
	file<<"P5"<<endl;
	file<<targetWidth<<' '<<targetHeight<<endl;
	file<<"255"<<endl;
	
	transformedMatrix = new unsigned char* [targetHeight];
	for(int l=0,size=targetWidth*this->getPixelAttribute();l<targetHeight;l++)
	{
		transformedMatrix[l] = new unsigned char [size];
		for(int l1=0,l2=0,gap=this->getPixelAttribute();l1<size;l1+=gap,l2+=CHANNEL_PER_PIXEL)
		{
			transformedMatrix[l][l1] = (unsigned char)srcPixelMatrix[l][l2];
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
