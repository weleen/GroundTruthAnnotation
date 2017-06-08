// ImgToVideo_test.cpp : Defines the entry point for the console application.
//

#include <cv.h>
#include <highgui.h>
#include <iostream>
#include <direct.h>//for mk_dir
#include <io.h>//for _acess()
#include <string>

using namespace std;

static int frameIdx = 0;
int recursive_mkdir(char *dir);//创建多级目录
int ImageToVideo(char* outDir, char* videoName, char* inputDir, int startFrame, int endFrame, int imgW,
	int imgH, char* imgExt, double fps, int isColor, int fourcc);//图片序列转化为视频
int  VideoToImage(char* videoName, char* outDir, char* imgExt, int maxFrameCount);//视频转化为图片序列


int main(int argc, char** argv) {
	//视频转图片
	char* videoName1 = argv[1];
	char* outDir1 = argv[2];
	cout << "how many frames do you want at last? (suggest 500)" << endl;
	int f;
	cin >> f;
	int images = VideoToImage(videoName1, outDir1, ".jpg", f);
	std::cout << "total frames have been extracted from video." << std::endl;
	return 0;
}

//将图片序列转换为视频,返回视频帧数
int ImageToVideo(char* outDir, char* videoName, char* inputDir, int startFrame, int endFrame, int imgW, int imgH, char* imgExt, double fps = 24, int isColor = 1, int fourcc = CV_FOURCC('X', 'V', 'I', 'D'))
{
	//判断输入文件夹是否存在
	if (_access(inputDir, 0) == -1)
	{
		std::cout << "the input directory does not exist!" << std::endl;
		return 0;
	}
	//判断输出文件夹是否创建 若没有则创建；若为NULL则默认当前工作目录
	char fullVideoName[255];//输出视频的完整文件名：路径+文件名
	strcpy_s(fullVideoName, "");
	if (outDir == NULL)
	{
		sprintf_s(fullVideoName, "%s", videoName);//把videoName打印成一个字符串保存在fullVideoName 中 
	}
	else
	{
		if (_access(outDir, 0) == -1)
		{
			recursive_mkdir(outDir);
		}
		sprintf_s(fullVideoName, "%s%s", outDir, videoName);//将字符串outDir和videoName连接起来，打印，保存在fullVideoName中
	}
	int frameCount = 0;
	CvVideoWriter *pWriter = NULL;
	CvSize size = cvSize(imgW, imgH);
	pWriter = cvCreateVideoWriter(videoName, fourcc, fps, size, isColor);//CREATE WRITER

	IplImage *pImg = NULL;
	char cur_fn[255];//表示某张图片的路径
	while (startFrame <= endFrame)
	{
		strcpy_s(cur_fn, "");
		sprintf_s(cur_fn, "%s%d%s", inputDir, startFrame, imgExt);//need to change  
		pImg = cvLoadImage(cur_fn, isColor);
		if (!pImg)
		{
			std::cout << "can't open an image file" << std::endl;
			return frameCount;
		}
		cvWriteFrame(pWriter, pImg);
		cvWaitKey(1);
		std::cout << "Write frame " << startFrame << std::endl;
		startFrame++;
		cvReleaseImage(&pImg);
		frameCount++;
	}
	cvReleaseVideoWriter(&pWriter);
	rename(videoName, fullVideoName);//移动文件到指定文件夹
	return  frameCount;
}

//将视频转换为图片序列 返回由视频分解得到的图片总帧数 目前OpenCV只支持AVI格式 因此使用之前需要
//将视频转化问AVI格式
int  VideoToImage(char* videoName, char* outDir, char* imgExt, int maxFrameCount) {
	CvCapture *cap = cvCaptureFromFile(videoName);
	if (cap == NULL) {
		cerr << "input video handler is NULL" << endl;
		return -1;
	}
	//保存图片的文件夹路径一定要有，因为OpenCV不会自动创建文件夹
	if (_access(outDir, 0) == -1) {
		recursive_mkdir(outDir);
		std::cout << "the ouput directory does not exist, and the have been created autonomously!" << std::endl;
	}
	char cur_fn[255];//保存当前帧所得图片的文件名
	IplImage* pImg = NULL;
	pImg = cvQueryFrame(cap);
	int width = pImg->width;
	int height = pImg->height;
	IplImage *pRGBImg = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
	int totalFrame = int(cvGetCaptureProperty(cap, CV_CAP_PROP_FRAME_COUNT));
	cout << "Total frames are " << totalFrame << endl;
	int clipLength = totalFrame / maxFrameCount;
	cout << "clipLength is " << clipLength << endl;
	int frame = 0;
	bool flag = true;
	while ((pImg = cvQueryFrame(cap)) != NULL && ((frame / clipLength)<maxFrameCount)) {
		if (!flag) {
			frame++;
			if (frame%clipLength != 0)
				flag = false;
			else
				flag = true;
			continue;
		}
		pRGBImg = cvCloneImage(pImg);
		// print some properties
		cout << "Current position is " << cvGetCaptureProperty(cap, CV_CAP_PROP_POS_FRAMES) << endl;
		strcpy_s(cur_fn, "");
		sprintf_s(cur_fn, "%s%05d%s", outDir, frame / clipLength, imgExt); //这里的设置适合形如 00123.jpg 00124.jpg的图片序列
		cvSaveImage(cur_fn, pRGBImg, NULL);
		frame++;
		if (frame%clipLength != 0)
			flag = false;
		else
			flag = true;
	}
	cvReleaseImage(&pImg);
	cvReleaseImage(&pRGBImg);
	cvReleaseCapture(&cap);
	return frame;
}


//该函数借鉴了网上资料，自动创建多级目录
int recursive_mkdir(char *dir)
{
	//分解路径名E:\\AA\\BB\\CC\\
	 //
	std::string str = dir;
	int index = 0;
	int i = 0;
	while (1)
	{
		std::string::size_type pos = str.find("\\", index);
		std::string str1;
		str1 = str.substr(0, pos);
		if (pos != -1 && i > 0)
		{
			if (_access(str1.c_str(), 0) == -1)
			{
				_mkdir(str1.c_str());
			}
		}
		if (pos == -1)
		{
			break;
		}
		i++;
		index = pos + 1;
	}
	return 0;
}
