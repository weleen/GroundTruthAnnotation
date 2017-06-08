#include <iostream>
#include <cv.h>
#include "Video.h"
#include <fstream>
#include <vector>

using namespace std;
using namespace cv;

int main(int argc,char** argv)
{
	vector<CvPoint> points;
	vector<CvPoint> rect;
	ifstream gt(argv[1]);
	bool flag = true;
	if (gt.eof() || gt.bad() || gt.fail())
	{
		cout << "failed to open groundtruth!" << endl;
		return EXIT_FAILURE;
	}
	while (!(gt.eof() || gt.bad() || gt.fail()))
	{
		CvPoint pt;
		gt >> pt.x >> pt.y;
		if (!(gt.eof() || gt.bad() || gt.fail()))
		{
			points.push_back(pt);
		}
	}
	char *windowName = "gt";
	string videoPath = argv[2];
	string outPath = argv[3];
	Video* pVideoInput = 0;
	Video* pVideoOutput = 0;
	namedWindow(windowName, 1);
	pVideoInput = new Video(videoPath, false, "%05d.jpg");
	pVideoOutput = new Video(outPath, true, "%05d.jpg");
	Mat frameGrey;
	Mat frame;
	for (int frameIdx = 0;; frameIdx++)
	{
		cout << "frame index = " << frameIdx << endl;
		bool ok = pVideoInput->ReadFrame(frameGrey);
		cvtColor(frameGrey, frame, CV_GRAY2RGB);
		if (!ok) break;
		rect.clear();
		rect.push_back(points[frameIdx * 4]);
		rect.push_back(points[frameIdx * 4+1]);
		rect.push_back(points[frameIdx * 4+2]);
		rect.push_back(points[frameIdx * 4+3]);
		for (int i = 0; i != 4; i++)
		{
			line(frame, rect[i], rect[(i + 1) % 4], cv::Scalar(0, 0, 255), 10);
		}
		imshow(windowName, frame);
		if (pVideoOutput)
		{
			bool ok = pVideoOutput->WriteFrame(frame);
			if (!ok) cout << "error writing results video frame" << endl;
		}
		if(waitKey(100) == int('p')) flag = false;
		while (flag == false)
		{
			if (waitKey() == int('p'))
			{
				flag = true;
				break;
			}
		}
	}
	return 0;
}