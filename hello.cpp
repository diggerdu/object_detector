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
	private:
	vector<Point2f> _contour;
	int _shape, _x, _y;
	double _arclen, _scale;
	public:
	DetectRes(vector<Point2f> c, int s, double arc) : _shape(s), _contour(c), _arclen(arc) {}
	void setCentroid(double x, double y)
	{
		_x = (int)x;
		_y = (int)y;
	}
	void setScale(double s) { _scale = s / _arclen;}
	void setScale2(double s) { _scale = s;}
	/*
	int size() { return _shape;}
	vector<Point2f> getContour{ return _contour;}
	double scale() { return _scale;}
	double center_x() { return _x;}
	double center_y() { return _y;}
	double arclen() { return -arclen;}
	*/
};	

class MRect
{
	public:
	vector<Point> contour;
	Point center;
	double area;
	double angle;
	
	MRect(vector<Point> c, Point p, double a, double theta) : contour(c), center(p), area(area), angle(theta) {}
	bool operator < (const MRect& other) const
	{
		return -area < -other.area;
	}	
};

/* Return if absolute value of X is within the range of [MIN, MAX]. */
bool MInRange(double x, double min, double max)
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
double distance(Point& pt1, Point& pt2)
{
	Point diff = pt1 - pt2;
	return cv::sqrt(diff.x * diff.x + diff.y * diff.y);
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
		RotatedRect ro_rect = minAreaRect(lineared);
		//Point points[4];
		//ro_rect.points(points);
		double rect_width = ro_rect.size.width;
		double rect_height = ro_rect.size.height;
		double rect_area = rect_width * rect_height;
		if (!MInRange(rect_width / rect_height, 0.9 ,1.1))
			continue;
		if (!MInRange(rect_area / contour_area, 0.9 , 1.1))		
			continue;
		MRect rect = MRect(contour, ro_rect.center, rect_area, ro_rect.angle);
		rectangles.push_back(rect);
	}
	return rectangles;
}

DetectRes detectMarker(Mat frame)
{
	Mat treated = pretreat(frame);
	imwrite("ZTreated.png", treated);
	vector<MRect> rectangles = findRects(treated);
	sort(rectangles.begin(), rectangles.end());
	for (int i = 0; i < rectangles.size(); i++)
		for (int j = 0; j < rectangles.size(); j++)
		{
			MRect outer = rectangles[i];
			MRect inner = rectangles[j];
			Point diff = outer.center - inner.center;
			double distance = sqrt(diff.x * diff.x + diff.y * diff.y);
			if (!MInRange(distance, 0, 0.5) ||
			    !MInRange(outer.angle - inner.angle, 0, 2.5) ||
			    !MInRange(outer.area / inner.area, 1.5, 3))
				continue;
			
			vector<Point2f> outer_approx = linearApprox(outer.contour);
			vector<Point2f> inner_approx = linearApprox(inner.contour);

			double outer_len = arcLength(outer.contour, true) / 4.0;
			double inner_len = arcLength(inner.contour, true) / 4.0;
			DetectRes res = DetectRes(outer_approx, 4, outer_len);
			res.setCentroid(outer.center.x, outer.center.y);
			res.setScale2((1.75 / outer_len) + (1.2 / inner_len));
			return res;
			break;
		}

}
int main(int argc, char** argv )
{
    return 0;
}

