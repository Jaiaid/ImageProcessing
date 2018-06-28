/*
 *An implementaion/extension of image class to ppm format 
*/
#ifndef PPM_H
#define PPM_H

#include<iostream>
#include<fstream>
#include<cstdio>
#include<cstring>

#include"imat.h"

#define MAX_HEADER_LENGTH 255

using namespace std;

class PPM : public File
{
public:
	PPM(char *name):File(name)
	{
		typeExtension[0] = 'p', typeExtension[1] = 'p', typeExtension[2] = 'm', typeExtension[3] = '\0';
	}
	IMat * extract();
	int save(char *destFileName, IMat *image, ...);
};


IMat * PPM::extract()
{
	IMat *image;
	char header[MAX_HEADER_LENGTH+1];
	pixel_t **pixelMatrix;
	int h,w;
	ifstream fin;
	/*
 	 *this variable is used to transform pixel_t type to 1 byte format to save 
 	 *in ppm format. These is done to reduce number of call to file.write
	*/
	unsigned char **transformedMatrix;

	fin.open(this->name,ios::binary);
	if(!fin) return NULL;

	image = new IMat;
	/*
	 *extracting the header 
	*/
	fin.getline(header,MAX_HEADER_LENGTH);
	//extracting comment if any 
	fin.getline(header,MAX_HEADER_LENGTH,'\n');
	if(header[0]=='#'){
		do
		{
			fin.getline(header,MAX_HEADER_LENGTH,'\n');
		}
		while(header[0]=='#');
	}
	//comment extraction finished
	sscanf(header,"%d%d",&w,&h);
	fin>>header;
	fin.get();
	/*
	 * extraction finished
	*/
	image->setHeight(h), image->setWidth(w);
	image->setColoDepthBit(8);
	image->setPixelAttribute(3);

	/*
	 *extract pixel array and save the pointer as base object attribute 
	*/ 
	transformedMatrix = new unsigned char* [h];
	for(int l=0,size=w*image->getPixelAttribute();l<h;l++)
	{
		transformedMatrix[l] = new unsigned char [size];
		fin.read((char *)transformedMatrix[l],size);			//read each row
	}
	
	pixelMatrix = new pixel_t *[h];
	for(int l=0,size=w*CHANNEL_PER_PIXEL;l<h;l++)
	{
		pixelMatrix[l] = new pixel_t [size];
		for(int l1=0,l2=0,gap=image->getPixelAttribute();l1<size;l1+=CHANNEL_PER_PIXEL,l2+=gap)
		{
			pixelMatrix[l][l1] = (unsigned int)transformedMatrix[l][l2];
			pixelMatrix[l][l1+1] = (unsigned int)transformedMatrix[l][l2+1];
			pixelMatrix[l][l1+2] = (unsigned int)transformedMatrix[l][l2+2];
		}
	}
	image->setPixelMatrix(pixelMatrix);

	fin.close();

	for(int l=0;l<h;l++)
	{
		delete[] transformedMatrix[l];
	}
	delete[] transformedMatrix;

	return image;
}

int PPM::save(char *destFileName, IMat *image, ...)
{
	pixel_t **srcPixelMatrix;
	int height, width;
	ofstream of;
	/*
	 *this variable is used to transform pixel_t type to 1 byte format to save 
	 *in ppm format. These is done to reduce number of call to file.write
	*/
	unsigned char **transformedMatrix;

	of.open(destFileName,ios::binary);
	
	if(!of){
		cout<<"Saving output file failed"<<endl;
		return -1;
	}
	
	height = image->getHeight(), width = image->getWidth();
	
	of<<"P6"<<endl;
	of<<width<<' '<<height<<endl;
	of<<"255"<<endl;
	
	srcPixelMatrix = image->getPixelMatrix();
	transformedMatrix = new unsigned char* [height];
	
	for(int l=0,size=width*image->getPixelAttribute();l<height;l++)
	{
		transformedMatrix[l] = new unsigned char [size];
		for(int l1=0,l2=0,gap=image->getPixelAttribute();l1<size;l1+=gap,l2+=CHANNEL_PER_PIXEL)
		{
			transformedMatrix[l][l1] = (unsigned char)srcPixelMatrix[l][l2];
			transformedMatrix[l][l1+1] = (unsigned char)srcPixelMatrix[l][l2+1];
			transformedMatrix[l][l1+2] = (unsigned char)srcPixelMatrix[l][l2+2];
		}
	}
	for(int l=0,size=width*image->getPixelAttribute();l<height;l++)
	{
		of.write((char *)transformedMatrix[l],size);
	}
	
	of.close();
	
	for(int l=0;l<height;l++)
	{
		delete[] transformedMatrix[l];
	}
	delete[] transformedMatrix;
	
	return 0;
}

#endif
