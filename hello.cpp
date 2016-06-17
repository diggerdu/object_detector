// just for ssh test
#include <iostream>
#include <cmath>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
const int BLUR_BLOCK_SIZE = 3;
const double EPSILON_RATE = 0.05;
using namespace std;
using namespace cv;

class DetectRes
{
	
};

class MRect
{
	public:
	vector<Point> contour;
	Point center;
	double area;
	double angle;
	
	MRect(vector<Point> c, Point p, double a, double theta) : contour(c), center(p), area(area), angle(theta) {}
	bool operator > (const MRect& other) const
	{
		return area > other.area;
	}	
};

/* Return if absolute value of X is within the range of [MIN, MAX]. */
bool in_range(double x, double min, double max)
{
	x = fabs(x);
	return (min <= x) && (x <= max);
}

Mat pretreat(Mat frame)
{
	Mat treated;
	cvtColor(frame, frame, CV_BGR2GRAY);
	GaussianBlur(frame, treated, Size(BLUR_BLOCK_SIZE, BLUR_BLOCK_SIZE), 3);
	///cautions
	threshold(treated, treated, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));
	morphologyEx(treated, treated, MORPH_GRADIENT, kernel);	
	return treated;		
}

/** Return the centroid of a given CONTOOUR */
Point get_centroid(vector<Point> contour)
{
	Moments moments;
	Point centroid;
	return centroid;
}

/** Caculate the distance */
double distance(Point pt1, Point pt2)
{
	return sqrt(sqrt(pt1.x - pt2.x) + sqrt(pt1.y - pt2.y));
}

/** Rerun the appoximation of contour. */
vector<Point2f> linearApprox(vector<Point> contour)
{
	vector<Point2f> lineared;
	for (int i = 0; i < contour.size(); i++)
		lineared.push_back((Point2f)contour[i]);
	double epsilon = EPSILON_RATE * arcLength(lineared, true);
	approxPolyDP(lineared, lineared, epsilon, true);
	return lineared;
}	

vector<MRect> findRects(Mat image)
{	
	vector<MRect> rectangles;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(image, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);
	for (int i = 0; i < contours.size(); i++)
	{
		vector<Point> contour = contours[i];
		double contour_area = contourArea(contour);
		if (contour.size() < 4 || contour_area < 25)
			continue;
		vector<Point2f> lineared;
		for (int i = 0; i < contour.size(); i++)
			lineared.push_back((Point2f)contour[i]);
		
	}
}
int main(int argc, char** argv )
{
    return 0;
}

