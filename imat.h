/*
 * Image class is an abstract class
 * It is intended to be implemented by a class of a specific image format 
 * Here a image is parsed and pixel array is kept in pixelMatrix. Each pixel is
 * of type pixel_t which is defined as unsigned int
 * image parsing method is implementaion dependent. Any class which inherits 
 * this class needs to implement it according to it's format.
*/
#ifndef IMAT_H
#define IMAT_H

#define pixel_t unsigned int
#define CHANNEL_PER_PIXEL 3

class IMat
{
	pixel_t **pixelMatrix; //variable to hold the pixel values of image as a matrix
								//each pixelAttribute number byte(s) holds information for one pixel
								//matrix is (height x width*pixelAttribute) size
								//matrix will have r,g,b(if colored) value sequentially, then other value(total pixelAttribute number including intensity info)
	int height;					//image height
	int width;					//image width
	int pixelAttribute;         //number of attribute held by an image pixel. For example greyscale image, a pixel holds a byte. So, pixelAttribute==1. In ppm it is 3(one for each r,g,b value)  
	int colorDepthBit;			//bit needed to hold a pixel's intensity value per color attribute
public:
	IMat()
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
	
	~IMat()
	{
		if(pixelMatrix!=NULL){
			delete[] pixelMatrix;
		}
	}
	
};

#endif
