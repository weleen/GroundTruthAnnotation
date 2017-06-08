#include <iostream>
#include <cv.h>
#include <opencv.hpp>
#include <fstream>
#include <vector>

using namespace std;

#define max(a,b) (((a) > (b)) ? (a) : (b))  
#define min(a,b) (((a) < (b)) ? (a) : (b))  

//注意参数是有符号短整型，该函数的作用是使i限定为[a,b]区间内  
int bound(short i, short a, short b) {
	return min(max(i, min(a, b)), max(a, b));
}

CvScalar getInverseColor(CvScalar c) {
	CvScalar s;
	for (int i = 0; i <= 2; ++i)
	{
		s.val[i] = 255 - c.val[i];
	}
	return s;
}

IplImage* src = 0;
IplImage* dst = 0;
int n = 0;
vector<CvPoint> points;

void on_mouse(int event, int x, int y, int flags, void* ustc) {
	CvPoint pt;
	CvPoint tmp_pt = { -1, -1 };
	CvFont font;
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.4, 0.4, 0, 1, CV_AA);
	char temp[16];
	CvSize text_size;
	int baseline;

	CvScalar clrPoint = cvScalar(255, 0, 0, 0);
	CvScalar clrText = cvScalar(255, 255, 255, 0);

	if (event == CV_EVENT_MOUSEMOVE) {
		cvCopy(dst, src);

		x = bound(x, 0, src->width - 1);
		y = bound(y, 0, src->height - 1);
		pt = cvPoint(x, y);
		cvCircle(src, pt, 2, clrPoint, CV_FILLED, CV_AA, 0);

		//sprintf(temp, "%d (%d,%d)", n + 1, x, y);
		sprintf(temp, "%d", n + 1);
		cvGetTextSize(temp, &font, &text_size, &baseline);
		tmp_pt.x = bound(pt.x, 0, src->width - text_size.width);
		tmp_pt.y = bound(pt.y, text_size.height + baseline, src->height - 1 - baseline);
		cvPutText(src, temp, tmp_pt, &font, clrText);

		cvShowImage("src", src);
	}
	else if (event == CV_EVENT_LBUTTONDOWN) {
		pt = cvPoint(x, y);
		points.push_back(pt); n++;
		cvCircle(src, pt, 2, clrPoint, CV_FILLED, CV_AA, 0);

		sprintf(temp, "%d (%d,%d)", n, x, y);
		cvGetTextSize(temp, &font, &text_size, &baseline);
		tmp_pt.x = bound(pt.x, 0, src->width - text_size.width);
		tmp_pt.y = bound(pt.y, text_size.height + baseline, src->height - 1 - baseline);
		//cvPutText(src, temp, tmp_pt, &font, clrText);

		cvCopy(src, dst);
		cvShowImage("src", src);
	}
	else if (event == CV_EVENT_RBUTTONDOWN)	{
		if (!points.empty()) {
			cvCopy(dst, src);

			pt = points.back();
			points.pop_back();
			cvCircle(src, pt, 2, getInverseColor(clrPoint), CV_FILLED, CV_AA, 0);

			sprintf(temp, "%d (%d,%d)", n, pt.x, pt.y); --n;
			cvGetTextSize(temp, &font, &text_size, &baseline);
			tmp_pt.x = bound(pt.x, 0, src->width - text_size.width);
			tmp_pt.y = bound(pt.y, text_size.height + baseline, src->height - 1 - baseline);
			// display the coordinate
			//cvPutText(src, temp, tmp_pt, &font, getInverseColor(clrText));

			cvCopy(src, dst);
			cvShowImage("src", src);
		}
	}
}

int main(int argc,char** argv) {
	// image name
	string doc = argv[1];
	string outDoc = argv[2];
	string format("\\%05d.jpg");
	string outFile("\\%05d.txt");

	int annotNum;
	cout << "how many points you need?" << endl;
	cin >> annotNum;

	string image;
	string annotFile;
	char imnum[100];
	char txtnum[100];
	for (int num = 0;; num++) {
		sprintf(imnum, format.c_str(), num);
		image = doc;
		image.append(imnum);
		char* cstr = new char[image.length() + 1];
		strcpy(cstr, image.c_str());
		printf("%s\n", cstr);
		src = cvLoadImage(cstr);
		if (src == NULL) {
			cout << "read image error!" << endl; 
			continue;
		}
		dst = cvCloneImage(src);

		cvNamedWindow("src", 1);
		cvSetMouseCallback("src", on_mouse, 0);

		cvShowImage("src", src);
		while (n != annotNum && cvWaitKey(15)!=27);
		cvDestroyAllWindows();
		cvReleaseImage(&src);
		cvReleaseImage(&dst);

		sprintf(txtnum, outFile.c_str(), num);
		annotFile = outDoc;
		annotFile.append(txtnum);
		ofstream file(annotFile);
		if (!file) {
			printf("open %s error!", annotFile);
			return -1;
		}
		vector<CvPoint>::iterator it = points.begin();
		for (int i = 1; i != annotNum; i++) {
			for (; it != points.end(); ++it) {
				if (i != annotNum) {
					file << it->x << ' ' << it->y << '\n';
					cout << it->x << ' ' << it->y << '\n';
				}
				else {
					file << it->x << ' ' << it->y;
					cout << it->x << ' ' << it->y;
				}
			}
		}
		file << endl;
		cout << endl;
		file.close();
		points.clear();
		n = 0;
	}
	return 0;
}