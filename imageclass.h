#include<iostream>
#include<fstream>
#include<cstdio>
#include<cstring>

using namespace std;


/*
 * Image class is an abstract class
 * It is intended to be implemented by a class of a specific image format 
 * Here a image is parsed an pixel array is kept in pixelMatrix
 * image parsing method is implementaion dependent. Any class which inherits 
 * this class needs to implement it according to it's format.
*/
class Image
{
	unsigned char **pixelMatrix;//variable to hold the pixel values of image as a matrix
								//each pixelAttribute number byte(s) holds information for one pixel
								//matrix is (height x width*pixelAttribute) size
								//matrix will have r,g,b(if colored) value sequentially, then other value(total pixelAttribute number including intensity info)
	int height;					//image height
	int width;					//image width
	int pixelAttribute;         //number of attribute held by an image pixel. For example greyscale image, a pixel holds a byte. So, pixelAttribute==1. In ppm it is 3(one for each r,g,b value)  
	int colorDepthBit;			//bit needed to hold a pixel's intensity value per color attribute
	bool existance;				//true if file used to instantiate really exists
public:
	Image()
	{
		pixelMatrix=NULL;
	}

	void setHeight(int height)
	{
		this->height=height;
	}
	
	void setWidth(int width)
	{
		this->width=width;
	}
	
	/*
	 *derived class will call setPixelMatrix after extracting the pixel matrix from an image format  
	*/ 
	void setPixelMatrix(unsigned char **src)
	{
		this->pixelMatrix = src;
	}
	
	void setPixelAttribute(int pixelAttribute)
	{
		this->pixelAttribute=pixelAttribute;
	}
	
	void setColoDepthBit(int colorDepthBit)
	{
		this->colorDepthBit=colorDepthBit;
	}
	
	void setExistance(bool existance)
	{
		this->existance=existance;
	}
	
	int getHeight()
	{
		return this->height;
	}
	
	int getWidth()
	{
		return this->width;
	}
	
	unsigned char **getPixelMatrix()
	{
		return this->pixelMatrix;
	}
	
	int getPixelAttribute()
	{
		return this->pixelAttribute;
	}
	
	int getColorDepthBit()
	{
		return this->colorDepthBit;
	}
	
	bool exists()
	{
		return this->existance;
	}
	
	virtual void save(char *fileName,unsigned char **src,int targetWidth,int targetHeight,...) = 0;//save method depends on image format. So. implementation dependent
	
	~Image()
	{
		if(pixelMatrix!=NULL){
			delete[] pixelMatrix;
		}
	}
	
};


/*
 *An implementaion/extension of image class to ppm format 
*/
class PPM:public Image
{
	char *name;
	
public:
	PPM(char *fileName)
	{
		char header[256];
		unsigned char **pixelMatrix;
		int h,w;
		ifstream file;
		
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
		pixelMatrix = new unsigned char *[h];
		for(int l=0,size=w*3;l<h;l++)
		{
			pixelMatrix[l] = new unsigned char [w*3];
			file.read((char *)pixelMatrix[l],size);				//read each row
		}
		this->setPixelMatrix(pixelMatrix);
		
		file.close();
	}

	void save(char *fileName,unsigned char **src,int targetWidth,int targetHeight,...);
	
	~PPM()
	{
		
	}
};

void PPM::save(char *fileName,unsigned char **srcPixelMatrix,int targetWidth,int targetHeight,...)
{
	ofstream file;

	file.open(fileName,ios::binary);
	
	if(!file){
		cout<<"Saving output file failed"<<endl;
	}
	
	file<<"P6"<<endl;
	file<<targetWidth<<' '<<targetHeight<<endl;
	file<<"255"<<endl;
	
	for(int l=0,size=targetWidth*3;l<targetHeight;l++)
	{
		file.write((char *)srcPixelMatrix[l],size);	
	}
	
	file.close();
}
