#include<iostream>
#include<fstream>
#include<cstring>
#include<cstdlib>

#include"imageprocessor.h"


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
int main(int argc, char **argv)
{
	if(argc<4){
		cout<<"<input ppm file> <operation & parameter> <output ppm file>"<<endl<<endl;
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
	
	DIP imagePrcsr(argv[1]); //creates an image processor  which process file with name  in argv[1]
	
	if(strcmp(argv[2],"resize")==0){
		if(argc==7){
			if(argv[3][0]=='b'){
				imagePrcsr.bilresize(argv[6],atoi(argv[4]),atoi(argv[5]));
			}
			else if(argv[3][0]=='n'){
				imagePrcsr.nnresize(argv[6],atoi(argv[4]),atoi(argv[5]));
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
		imagePrcsr.neg(argv[3]);
	}
	else if(strcmp(argv[2],"TH")==0 && argc==5){
		imagePrcsr.threshold(argv[4],atoi(argv[3]));
	}
	else if(strcmp(argv[2],"gamma")==0 && argc==5){
		imagePrcsr.gammacorrection(argv[4],atof(argv[3]));
	}
	else if(strcmp(argv[2],"filter")==0 && argc==5){
		imagePrcsr.filtering(argv[4],atoi(argv[3]));
	}
	else{
		cout<<"wrong cmd format"<<endl;
	}
	
	return 0;
}
