#include<iostream>
#include<cmath>

#include"ppm.h"


using namespace std;

/*
 * This class takes a file name and instantiate the internal Image class with it.
 * presently only PPm implementation of Image class is available
 * DIP class has
 * -resizing operation. Both bilinear and nearest neighbor approach
 * -negative operation
 * -gamma correction
 * -filtering operation
*/ 
class DIP
{
	Image *srcFile;					//variable to hold the input image file
	unsigned int **filter;			//filter mask is created to do filter operation
	int filterWidth,filterHeight;	//filter mask width and height
	float filterConstant;			//constant to multiply result of filtering to get the final result
									//used to make the filter mask operation operation lighter(only one floating point multiplication is needed)
	
	/*
	 * method to create filter
	 * first nine types are for smoothing 
	 * after type 10 are for sharpening
	*/ 
	int createFilter(int filterType)
	{
		if(filter!=NULL){
			delete[] filter;
		}
		
		switch(filterType)
		{
			case 1:
				this->filterWidth=3,this->filterHeight=3;
				this->filterConstant=1.0/9;
				break;
			case 2:
				this->filterWidth=5,this->filterHeight=5;
				this->filterConstant=1.0/25;
				break;
			case 3:
				this->filterWidth=9,this->filterHeight=9;
				this->filterConstant=1.0/81;
				break;
			case 4:
				this->filterWidth=15,this->filterHeight=15;
				this->filterConstant=1.0/225;
				break;
			case 5:
				this->filterWidth=35,this->filterHeight=35;
				this->filterConstant=1.0/(35*35);
				break;
			case 11:
				this->filterWidth=3,this->filterHeight=3;
				this->filterConstant=1.0;
				break;
			case 12:
				this->filterWidth=3,this->filterHeight=3;
				this->filterConstant=1.0;
				break;
			case 13:
				this->filterWidth=3,this->filterHeight=3;
				this->filterConstant=1.0/9;
				break;
		}
		
		filter=new unsigned int *[this->filterHeight];
		for(int l=0;l<this->filterHeight;l++)
		{
			filter[l]=new unsigned int[this->filterWidth];
			for(int l1=0;l1<this->filterWidth;l1++)
			{
				filter[l][l1]=1;
			}
		}
		
		if(filterType==11){
			filter[1][1]=5,filter[0][0]=0,filter[0][2]=0,filter[2][0]=0,filter[2][2]=0;
			filter[0][1]=-1,filter[1][0]=-1,filter[1][2]=-1,filter[2][1]=-1;
		}
		else if(filterType==12){
			filter[1][1]=9,filter[0][0]=-1,filter[0][2]=-1,filter[2][0]=-1,filter[2][2]=-1;
			filter[0][1]=-1,filter[1][0]=-1,filter[1][2]=-1,filter[2][1]=-1;
		}
		else if(filterType==13){
			filter[1][1]=17,filter[0][0]=-1,filter[0][2]=-1,filter[2][0]=-1,filter[2][2]=-1;
			filter[0][1]=-1,filter[1][0]=-1,filter[1][2]=-1,filter[2][1]=-1;
		}
		
		return 0;
	}
	
	/*
	 * To get the filtered result of a pixel of input image
	 * out of boundary neighbor problem is handled 
	*/
	int getFilteredVal(pixel_t **srcPixelMatrix,int width,int height,int pixelAttribute,int colorDepth,int x,int y)
	{
		int sum=0,tmp=(this->filterWidth-1)/2,tmp1=(this->filterHeight-1)/2,tmp2=pixelAttribute*width;
		float result;

		for(int l=-tmp,indx1=y-tmp;l<=tmp;l++,indx1++)
		{
			for(int l1=-tmp1,indx2=x-tmp1*pixelAttribute;l1<=tmp1;l1++,indx2+=pixelAttribute)
			{	
				if(indx1>=0 && indx1<height && indx2>=0 && indx2<tmp2){
					sum+=this->filter[l+tmp][l1+tmp1]*srcPixelMatrix[indx1][indx2];
				}
			}
		}
		
		result=sum*this->filterConstant+0.5;
		if(result<0){
			result=0;
		}
		else if(result>=colorDepth){
			result=colorDepth-1;
		}
		
		return (int)(result);
	}
	
public:
	DIP(char *srcFileName)
	{
		srcFile=new PPM(srcFileName);
		filter=NULL;
	}
	
	int nnresize(char *destFileName,int targetWidth,int targetHeight);		//resizing using nearest neighbor approach and saves it as destFileName
	int bilresize(char *destFileName,int targetWidth,int targetHeight);		//resizing using nearest bilinear approach and saves it as destFileName
	int neg(char *destFileName);											//negative image and saves it as destFileName
	int gammacorrection(char *destFileName,float gamma);					//intensity transform by gamma correction and saves it as destFileName
	int filtering(char *destFileName,int type);                             //filter image with filter no. type and saves it as destFileName
};


int DIP::nnresize(char *destFileName,int targetWidth,int targetHeight)
{
	pixel_t **srcPixelMatrix,**destPixelMatrix;
	int height,width,pixelAttribute;
	Image *destFile=new PPM(destFileName);
	
	if(!(this->srcFile->exists())){
		cout<<"Source file not found"<<endl;
		return -1;
	}
	
	srcPixelMatrix=this->srcFile->getPixelMatrix();
	
	height=this->srcFile->getHeight();
	width=this->srcFile->getWidth();
	pixelAttribute=this->srcFile->getPixelAttribute();

	destPixelMatrix=new pixel_t *[targetHeight];
	for(int y=0;y<targetHeight;y++)
	{
		destPixelMatrix[y]=new pixel_t [targetWidth*pixelAttribute];

		for(int x=0,indx2=0;x<targetWidth;x++,indx2+=pixelAttribute)
		{
			int oldx,oldy;
			
			if(targetHeight==1){
				oldy=0;
			}
			else{
				oldy=(int)(((height-1)*y*1.0)/(targetHeight-1)+0.5);
			}

			if(targetWidth==1){
				oldx=0;
			}
			else{
				oldx=(int)(((width-1)*x*1.0)/(targetWidth-1)+0.5);
			}

			int oldindx2=oldx*pixelAttribute;
			destPixelMatrix[y][indx2]=srcPixelMatrix[oldy][oldindx2];
			destPixelMatrix[y][indx2+1]=srcPixelMatrix[oldy][oldindx2+1];
			destPixelMatrix[y][indx2+2]=srcPixelMatrix[oldy][oldindx2+2];
		}
	}
	
	destFile->save(destFileName,destPixelMatrix,targetWidth,targetHeight);	//saving file with created pixel matix
	delete[] destPixelMatrix;												//free memory
	
	return 0;
}
	
int DIP::bilresize(char *destFileName,int targetWidth,int targetHeight)
{
	pixel_t **srcPixelMatrix,**destPixelMatrix;
	int height,width,pixelAttribute;
	Image *destFile=new PPM(destFileName);
	
	if(!(this->srcFile->exists())){
		cout<<"Source file not found"<<endl;
		return -1;
	}
	
	srcPixelMatrix=this->srcFile->getPixelMatrix();
	
	height=this->srcFile->getHeight();
	width=this->srcFile->getWidth();
	pixelAttribute=this->srcFile->getPixelAttribute();

	destPixelMatrix=new pixel_t *[targetHeight];
	for(int y=0;y<targetHeight;y++)
	{
		float sum1,sum2,oldy,oldx;
		int cel_x,cel_y,floor_x,floor_y;
		
		destPixelMatrix[y]=new pixel_t [targetWidth*pixelAttribute];

		if(targetHeight==1){
			oldy=0;
			cel_y=0;
			floor_y=0;
		}
		else{
			oldy=((height-1)*y*1.0)/(targetHeight-1);
			floor_y=(int)oldy;
			cel_y=(int)(oldy+0.5);
		}

		for(int x=0;x<targetWidth;x++)
		{
			int indx1,indx2,indx3,indx4;
			
			if(targetWidth==1){
				oldx=0;
				cel_x=0;
				floor_x=0;
			}
			else{
				oldx=((width-1)*x*1.0)/(targetWidth-1);
				floor_x=(int)oldx;
				cel_x=(int)(oldx+0.5);
			}
			
			indx1=floor_y,indx2=floor_x*pixelAttribute;
			indx3=cel_y,indx4=cel_x*pixelAttribute;
			
			sum1=(cel_x-oldx)*srcPixelMatrix[indx1][indx2]+(1-cel_x+oldx)*srcPixelMatrix[indx1][indx4];
			sum2=(cel_x-oldx)*srcPixelMatrix[indx3][indx2]+(1-cel_x+oldx)*srcPixelMatrix[indx3][indx4];
			destPixelMatrix[y][x*3]=(int)((cel_y-oldy)*sum1+(1-cel_y+oldy)*sum2+0.5);

			sum1=(cel_x-oldx)*srcPixelMatrix[indx1][indx2+1]+(1-cel_x+oldx)*srcPixelMatrix[indx1][indx4+1];
			sum2=(cel_x-oldx)*srcPixelMatrix[indx3][indx2+1]+(1-cel_x+oldx)*srcPixelMatrix[indx3][indx4+1];
			destPixelMatrix[y][x*3+1]=(int)((cel_y-oldy)*sum1+(1-cel_y+oldy)*sum2+0.5);

			sum1=(cel_x-oldx)*srcPixelMatrix[indx1][indx2+2]+(1-cel_x+oldx)*srcPixelMatrix[indx1][indx4+2];
			sum2=(cel_x-oldx)*srcPixelMatrix[indx3][indx2+2]+(1-cel_x+oldx)*srcPixelMatrix[indx3][indx4+2];
			destPixelMatrix[y][x*3+2]=(int)((cel_y-oldy)*sum1+(1-cel_y+oldy)*sum2+0.5);
		}
	}
	
	destFile->save(destFileName,destPixelMatrix,targetWidth,targetHeight);	//saving file with created pixel matix
	delete[] destPixelMatrix;												//free memory
	
	return 0;
}

int DIP::neg(char *destFileName)
{
	pixel_t **srcPixelMatrix,**destPixelMatrix;
	int height,width,pixelAttribute,maxColor;
	Image *destFile=new PPM(destFileName);
	
	if(!(this->srcFile->exists())){
		cout<<"Source file not found"<<endl;
		return -1;
	}
	
	srcPixelMatrix=this->srcFile->getPixelMatrix();
	
	height=this->srcFile->getHeight();
	width=this->srcFile->getWidth();
	pixelAttribute=this->srcFile->getPixelAttribute();
	maxColor=(1<<this->srcFile->getColorDepthBit())-1;

	destPixelMatrix=new pixel_t *[height];
	for(int y=0;y<height;y++)
	{
		destPixelMatrix[y]=new pixel_t [width*pixelAttribute];
		for(int x=0,indx2=0;x<width;x++,indx2+=pixelAttribute)
		{
			destPixelMatrix[y][indx2]=maxColor-srcPixelMatrix[y][indx2];
			destPixelMatrix[y][indx2+1]=maxColor-srcPixelMatrix[y][indx2+1];
			destPixelMatrix[y][indx2+2]=maxColor-srcPixelMatrix[y][indx2+2];
		}
	}
	
	destFile->save(destFileName,destPixelMatrix,width,height);	//saving file with created pixel matix
	delete[] destPixelMatrix;									//free memory
	
	return 0;
}

int DIP::gammacorrection(char *destFileName,float gamma)
{
	pixel_t **srcPixelMatrix,**destPixelMatrix;
	int height,width,pixelAttribute,colorNum;
	double constant,possibleGammaVal[256];
	Image *destFile=new PPM(destFileName);
	
	if(!(this->srcFile->exists())){
		cout<<"Source file not found"<<endl;
		return -1;
	}
	
	srcPixelMatrix=this->srcFile->getPixelMatrix();
	
	height=this->srcFile->getHeight();
	width=this->srcFile->getWidth();
	pixelAttribute=this->srcFile->getPixelAttribute();
	colorNum=(1<<this->srcFile->getColorDepthBit());
	constant=((colorNum-1)*1.0)/pow((colorNum-1),gamma);
	/*
	 * possibleGammaVal is collection of possible pix^gamma. As number of pixel
	 * most probably will be much larger than colorNum, calculate power for each pixel
	 * will be inefficient; 
	*/
	for(int l=0;l<colorNum;l++)
	{
		possibleGammaVal[l]=pow(l,gamma);
	}
	
	destPixelMatrix=new pixel_t *[height];
	for(int y=0;y<height;y++)
	{
		destPixelMatrix[y]=new pixel_t [width*pixelAttribute];

		for(int x=0,indx2=0;x<width;x++,indx2+=pixelAttribute)
		{
			destPixelMatrix[y][indx2]=constant*possibleGammaVal[srcPixelMatrix[y][indx2]];
			destPixelMatrix[y][indx2+1]=constant*possibleGammaVal[srcPixelMatrix[y][indx2+1]];
			destPixelMatrix[y][indx2+2]=constant*possibleGammaVal[srcPixelMatrix[y][indx2+2]];
		}
	}
	
	destFile->save(destFileName,destPixelMatrix,width,height);	//saving file with created pixel matix
	delete[] destPixelMatrix;									//free memory
	
	return 0;
}

int DIP::filtering(char *destFileName,int filterType)
{
	pixel_t **srcPixelMatrix,**destPixelMatrix;
	int height,width,pixelAttribute,colorDepth;
	Image *destFile=new PPM(destFileName);
	
	if(!(this->srcFile->exists())){
		cout<<"Source file not found"<<endl;
		return -1;
	}
	
	srcPixelMatrix=this->srcFile->getPixelMatrix();
	height=this->srcFile->getHeight();
	width=this->srcFile->getWidth();
	pixelAttribute=this->srcFile->getPixelAttribute();
	colorDepth=(1<<(this->srcFile->getColorDepthBit()));
	
	this->createFilter(filterType);
		
	destPixelMatrix=new pixel_t *[height];
	for(int y=0;y<height;y++)
	{
		destPixelMatrix[y]=new pixel_t [width*pixelAttribute];
		for(int x=0,indx2=0;x<width;x++,indx2+=pixelAttribute)
		{
			destPixelMatrix[y][indx2]=(unsigned char)getFilteredVal(srcPixelMatrix,width,height,pixelAttribute,colorDepth,indx2,y);
			destPixelMatrix[y][indx2+1]=(unsigned char)getFilteredVal(srcPixelMatrix,width,height,pixelAttribute,colorDepth,indx2+1,y);
			destPixelMatrix[y][indx2+2]=(unsigned char)getFilteredVal(srcPixelMatrix,width,height,pixelAttribute,colorDepth,indx2+2,y);
		}
	}
	
	destFile->save(destFileName,destPixelMatrix,width,height);	//saving file with created pixel matix
	delete[] destPixelMatrix;									//free memory
	
	return 0;
}
