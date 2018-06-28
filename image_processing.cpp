#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>

#include "file.h"
#include "pgm.h"
#include "ppm.h"
#include "imat.h"
#include "dip.h"


using namespace std;

/*
 * Here image is considered a 2D array. Like ....
 * ------------>x/width
 * |############
 * |############
 * y/height
 * 
 * main function creates an interface to interact with DIP class and process an image
 */
 
 
int typeExtensionIndx(char *fileName);
File * fileBuilder(char *fileName);

 
int main(int argc, char **argv)
{
	if(argc<4){
		cout<<"<input file> <operation & parameter> <output file>"<<endl<<endl;
		cout<<"operation list\n1.resize\n2.neg\n3.gamma\n4.smooth\n\n";
		cout<<"OPERATION AND PARAMETER LIST"<<endl;
		cout<<"----------------------------"<<endl;
		cout<<"1.resize <b(for bilinear approach)\\(for nearest neighbor approach)> <width> <height>"<<endl;
		cout<<"2.neg"<<endl;
		cout<<"3.TH <threshold value>"<<endl;
		cout<<"4.gamma <gamma value>"<<endl;
		cout<<"5.filter <filter type>"<<endl;
		
		return 0;
	}

	DIP imagePrcsr;
	File *srcImgFile = fileBuilder(argv[1]);
	File *destImgFile = fileBuilder(argv[1]);
	IMat *img = srcImgFile->extract();

	if(strcmp(argv[2],"resize")==0){
		if(argc==7){
			if(argv[3][0]=='b'){
				imagePrcsr.bilresize(img,atoi(argv[4]),atoi(argv[5]));
				destImgFile->save(argv[6],img);
			}
			else if(argv[3][0]=='n'){
				imagePrcsr.nnresize(img,atoi(argv[4]),atoi(argv[5]));
				destImgFile->save(argv[6],img);
			}
			else{
				cout<<"unknown parameter"<<endl;
			}
		}
		else{
			cout<<"wrong cmd format"<<endl;
		}
	}
	else if(strcmp(argv[2],"neg")==0 && argc==4){
		imagePrcsr.neg(img);
		destImgFile->save(argv[3],img);
	}
	else if(strcmp(argv[2],"TH")==0 && argc==5){
		imagePrcsr.threshold(img,atoi(argv[3]));
		destImgFile->save(argv[4],img);
	}
	else if(strcmp(argv[2],"gamma")==0 && argc==5){
		imagePrcsr.gammacorrection(img,atof(argv[3]));
		destImgFile->save(argv[4],img);
	}
	else if(strcmp(argv[2],"filter")==0 && argc==5){
		imagePrcsr.filtering(img,atoi(argv[3]));
		destImgFile->save(argv[4],img);
	}
	else{
		cout<<"wrong cmd format"<<endl;
	}
	
	return 0;
}


int typeExtensionStrIndx(char *fileName)
{
	int ans;
	for(int l=strlen(fileName)-1;l>0;l--)
	{
		if(fileName[l]=='.'){
			ans=l;		
		}
	}

	return ans+1;
}
 
 
File * fileBuilder(char *fileName)
{
	File *file;

	if(strcmp(&fileName[typeExtensionStrIndx(fileName)],"ppm")==0){
		file=(File *)new PPM(fileName);
	}
	else if(strcmp(&fileName[typeExtensionStrIndx(fileName)],"pgm")==0){
		file=(File *)new PGM(fileName);
	}

	return file;
}
