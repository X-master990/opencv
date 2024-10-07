#include<opencv2/opencv.hpp>
#include<iostream>
#include<string>
#include<fstream>
#include <iomanip>
#include <vector>
#include <math.h>

using namespace std;
using namespace cv;

int main(int argc, char* argv[]) {
	Mat grayImage, dstImage, resultImage;
	Mat Image = imread(argv[1]);
	resultImage = Image.clone();
	//imshow("原图", Image);
	cvtColor(Image, grayImage, COLOR_BGR2GRAY);
	//imshow("df", grayImage);
	Mat xxx=Mat(1477,1108,CV_8UC3);
	GaussianBlur(grayImage, xxx, Size(3,3),1);
	threshold(xxx, dstImage, 80, 255, THRESH_BINARY);
	//Mat ele = getStructuringElement(MORPH_RECT, Size(3,3));
	//morphologyEx(dstImage, dstImage, MORPH_GRADIENT, ele);
	Mat ddd = Mat(1477, 1108, CV_8UC3);
	//resize(dstImage, ddd, Size(dstImage.rows / 2, dstImage.cols / 2), 0, 0, INTER_LINEAR);
	//imshow("二值化", dstImage);

	vector<vector<Point>> contours;
	vector<Vec4i> hierarcy;
	findContours(dstImage, contours, hierarcy, RETR_LIST, CHAIN_APPROX_NONE);
	double lenght;
	double area;
	vector<vector<Point>> contours_t;
	for (int i = 0; i < contours.size(); i++) {
		lenght = arcLength(contours[i], true);
		area = contourArea(contours[i]);
		if (lenght <= 90 && lenght >= 50) {
			contours_t.push_back(contours[i]);
		}
	}

	Mat tmp = Mat(1477, 1108, CV_8UC3, Scalar(0, 0, 0));
	cv::drawContours(tmp, contours_t, -1, Scalar(0,0,255));
	//imshow("结果图", tmp);
	Mat ttt;
	resize(tmp, ttt, Size(tmp.rows / 2, tmp.cols / 2), 0, 0, INTER_LINEAR);
	//imshow("sdfa", ttt);

	vector<Point> p;
	int max = -10000;
	int min = 10000;
	Point point;

	for (int i = 0; i < tmp.cols; i++) {
		for (int j = 0; j < tmp.rows; j++) {
			if ((i + j < min) && (tmp.at<Vec3b>(j, i)[2] == 255)) {
				min = i + j;
				point.x = i;
				point.y = j;
				
			}
		}
	}
	//cout << point << endl;
	p.push_back(point);
	max = -10000;
	min = 10000;
	for (int i = 0; i < tmp.cols; i++) {
		for (int j = 0; j < tmp.rows; j++) {
			if (i - j > max && (tmp.at<Vec3b>(j, i)[2] == 255)) {
				max = i - j;
				point.x = i;
				point.y = j;
			}
		}
	}
	p.push_back(point);
	//cout << point << endl;
	max = -10000;
	min = 10000;
	for (int i = 0; i < tmp.cols; i++) {
		for (int j = 0; j < tmp.rows; j++) {
			if (i - j < min && (tmp.at<Vec3b>(j, i)[2] == 255)) {
				min = i - j;
				point.x = i;
				point.y = j;
			}
		}
	}
	p.push_back(point);

	max = -10000;
	min = 10000;
	for (int i = 0; i < tmp.cols; i++) {
		for (int j = 0; j < tmp.rows; j++) {
			if (i + j > max && (tmp.at<Vec3b>(j, i)[2] == 255)) {
				max = i + j;
				point.x = i;
				point.y = j;
			}
		}
	}
	p.push_back(point);

	Point2f src_points[] = {
		Point2f(p[0].x,p[0].y),
		Point2f(p[1].x,p[1].y),
		Point2f(p[2].x,p[2].y),
		Point2f(p[3].x,p[3].y)};

	Point2f dst_points[] = {
		Point2f(0, 0),
		Point2f(1108, 0),
		Point2f(0, 1477),
		Point2f(1108,1477) };

	Mat M = getPerspectiveTransform(src_points, dst_points);

	Mat dst__,tm;
	dst__ = Mat(298*5, 210*5, CV_8UC3);
	warpPerspective(Image, dst__, M, Image.size());
	//imshow("dst", dst__);
	resize(dst__, tm, Size(dst__.rows / 2, dst__.cols / 2), 0, 0, INTER_LINEAR);
	//imshow("dst", tm);

	Rect rect(220, 520, 776, 800);   
	Rect r(50, 520, 90, 800);
	Mat image_ax = dst__(r);
	Mat image_ori = dst__(rect);
	//imshow("结果", image_ori);
	//imshow("ax", image_ax);
	Mat grayimage, dstimage;
	
	cvtColor(image_ori, grayimage, COLOR_BGR2GRAY);
	Mat xy;
	GaussianBlur(grayimage, xy, Size(3, 3), 1);
	threshold(xy, dstimage, 113, 255, THRESH_BINARY);
	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
	morphologyEx(dstimage, dstimage, MORPH_CLOSE, element);

	//imshow("ans", dstimage);
	//113 110
	Mat gray, dstt;
	cvtColor(image_ax, gray, COLOR_BGR2GRAY);
	Mat zz;
	GaussianBlur(gray, zz, Size(3, 3), 1);
	threshold(zz, dstt, 110, 255, THRESH_BINARY);
	//imshow("height", dstt);
	vector<vector<Point>> cont;
	vector<Vec4i> hier;
	Mat sg = Mat(dstt.rows, dstt.cols, CV_8UC3, Scalar(0, 0, 0));
	findContours(dstt, cont, hier, RETR_LIST, CHAIN_APPROX_NONE);//y軸高度用
	
	findContours(dstimage, contours, hierarcy, RETR_LIST, CHAIN_APPROX_NONE);//找到答案卡的邊框
	vector<vector<Point>> conte;
	int cc = 1;
	
	
	while (true) {
		conte.clear();
		
		for (int i = 0; i < cont.size(); i++) {
			double area = contourArea(cont[i], true);
			if ((area >= 95 && area <= 200)) {
				conte.push_back(cont[i]);
			}
		}
		cout << conte.size() << endl;
		break;
		if (conte.size() == 24) break;
		else {
			threshold(zz, dstt, 105+cc, 255, THRESH_BINARY);
			cont.clear();
			findContours(dstt, cont, hier, RETR_LIST, CHAIN_APPROX_NONE);//y軸高度用
			cc++;
		}
	}
		
	
	vector<vector<Point>> t;
	drawContours(sg, conte, -1, Scalar(0, 0, 255)); //sg可以得出y軸高度(邊框
	for (int i = 0; i < contours.size(); i++) {
		double area = contourArea(contours[i], true);
		if ((area >= 95 && area <= 280)) {
			t.push_back(contours[i]);
		}
	}


	Mat sam = Mat(dstimage.rows, dstimage.cols, CV_8UC3, Scalar(0, 0, 0));
	drawContours(sam, t, -1, Scalar(0, 0, 255));
	
	//imshow("dstt", sg);
	//imshow("dstimage", sam);

	string anss = "1234567890AB";
	string accident = "MX";

	vector<double> y_cord;
	double up=-10000, low=10000;
	for (int i = 0; i < conte.size(); i++) {
		for (int j = 0; j < conte[i].size(); j++) {
			if (conte[i][j].y > up)
				up = conte[i][j].y;
			if (conte[i][j].y < low)
				low = conte[i][j].y;
		}
		y_cord.push_back((up + low)/2);
		up = -10000;
		low = 10000;
	}
	reverse(y_cord.begin(), y_cord.end());
	/*
	vector<double> x_cord;
	up = -10000, low = 10000;
	for (int i = 0; i < t.size(); i++) {
		for (int j = 0; j < t[i].size(); j++) {
			if (t[i][j].x > up)
				up = t[i][j].x;
			if (t[i][j].x < low)
				low = t[i][j].x;
		}
		x_cord.push_back(low);
		x_cord.push_back(up);
		up = -10000;
		low = 10000;
	}
	
	reverse(x_cord.begin(), x_cord.end());
	
	for (int i = 0; i < x_cord.size(); i++) {
		cout << x_cord[i] << endl;
	}

	
	for (int i = 0; i < y_cord.size(); i++) {
		cout << y_cord[i] << endl;
	}*/
	fstream write(argv[2], ios::out);
	vector<int> ans;
	string st;
	for (int i = 0; i < y_cord.size(); i++) {
		ans.clear();
		int r = y_cord[i];
		int count = 0;
		for (int x = 0; x < sam.cols; x++) {
			if (sam.at<Vec3b>(r, x)[2] == 255) {
				//cout << "x" << x << endl;
				int index = x / 61;
				//cout << "index:" << index << endl;
				ans.push_back(index);
				x += 61;
			}
			
		}
		
		string xt;
		if (ans.size() == 0)
			st+=accident[1];
		if (ans.size() == 1)
			st+=anss[ans[0]];
		if (ans.size() >= 2)
			st+=accident[0];
	}
	write << st << endl;
	
	waitKey(0);
}