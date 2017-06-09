#ifndef __DISP_H_
#define __DISP_H_

#include <cv.h>
#include <vector>
#include <opencv.hpp>

using namespace std;
using namespace cv;

void inline disp_points(vector<CvPoint> points, IplImage* image, string winName) {
	cvNamedWindow(winName.c_str(), 1);
	cout << "points size = "<< points.capacity() << endl;
	for (int i = 1; i <= points.capacity(); i++) {
		if (i == 1 || i == 18 || i == 23 || i == 28 || i == 32 || i == 37 || i == 43 || i == 49 || i == 61)
			cvCircle(image, points[i - 1], 2, cvScalar(255, 0, 0));
		else {
			cvCircle(image, points[i - 1], 2, cvScalar(255, 0, 0));
			cvLine(image, points[i - 2], points[i - 1], cvScalar(255, 0, 0));
		}
	}
	cvShowImage(winName.c_str(), image);
}

#endif