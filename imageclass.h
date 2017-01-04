/*
 * Image class is an abstract class
 * It is intended to be implemented by a class of a specific image format 
 * Here a image is parsed and pixel array is kept in pixelMatrix. Each pixel is
 * of type pixel_t which is defined as unsigned int
 * image parsing method is implementaion dependent. Any class which inherits 
 * this class needs to implement it according to it's format.
*/
#ifndef IMGCLS_H
#define IMGCLS_H

#define pixel_t unsigned int

class Image
{
	pixel_t **pixelMatrix; //variable to hold the pixel values of image as a matrix
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
	void setPixelMatrix(pixel_t **src)
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
	
	pixel_t **getPixelMatrix()
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
	
	virtual void save(char *fileName,pixel_t **src,int targetWidth,int targetHeight,...) = 0;//save method depends on image format. So. implementation dependent
	
	~Image()
	{
		if(pixelMatrix!=NULL){
			delete[] pixelMatrix;
		}
	}
	
};

#endif
