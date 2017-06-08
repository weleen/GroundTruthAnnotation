// opencvtest.cpp : 定义控制台应用程序的入口点。
//



//#include "stdafx.h"


#include <opencv.hpp>
#include <iostream>  

#include <fstream>  
#include <stdlib.h>  
using namespace std;
using namespace cv;

//4对相对应的像点计算图像的单应 Homography
vector<Point2f> left_image;
vector<Point2f> right_image;

Mat image_logo;
Mat image_main;

int main(int argc, char** argv)
{
	float i;
	ifstream in(argv[1]);// gt_init.txt
	ifstream in2(argv[2]);// gt_point
	string pic(argv[3]);// init img
	ofstream out1(argv[4]);// gt_homography.txt
	float a[5]={0};
	int k=0;
	while(in>>i)
	{
		a[k]=i;
		k++;
	}
	
	for (int m=0;m<4;m++)
	{
		cout<<a[m]<<endl;
	}

	image_main = imread(pic, CV_LOAD_IMAGE_COLOR);

	left_image.push_back(Point2f(0,0));
	left_image.push_back(Point2f(a[2],0));
	left_image.push_back(Point2f(a[2],a[3]));
	left_image.push_back(Point2f(0,a[3]));

	float j;
	float b[8]={0};
	int l=0;
	while(in2>>j)
	{
		b[l]=j;
		l++;
		if (l==8)
		{   
			right_image.push_back(Point2f(b[0],b[1]));	
			right_image.push_back(Point2f(b[2],b[3]));	 
			right_image.push_back(Point2f(b[4],b[5]));	
			right_image.push_back(Point2f(b[6],b[7]));   

			std::vector<Point2f>::iterator p;

			for (p=right_image.begin();p!=right_image.end();p++)
			{
				cout<< *p<<endl;
			}
			Mat H = findHomography(left_image,right_image,0);

			Mat H1=H.reshape(0,1);
			cout<<"H1"<<H1<<endl;
			out1<<H1;
			out1<<"\n";
			right_image.clear();
			l=0;
		}
	}
	out1.close();  


	return 0;
}





