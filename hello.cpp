// just for test
#include <iostream>
#include <cmath>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
const int BLUR_BLOCK_SIZE = 5;
const double EPSILON_RATE = 0.05;
using namespace std;
using namespace cv;

class DetectRes
{
	
};

class Rect
{
	private:
	vector<vector<Point> > _contour;
	Point _center;
	double _area;
	double _angle;
	
	public:
	Rect(vector<vector<Point> > c, Point p, double a, double theta)  : _contour(c), _center(p), _area(a), _angle(theta) {};
	int compareTo(Rect other)
	{	
		if (_area > -other._area)
			return 1;
		if (_area == -other._area)
			return 0;
		if (_area < -other._area)
			return -1;
	};
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
	cvtColor(original, treated, CV_BGR2GRAY);
	GaussianBlur(treated, treated, Size(BLUR_BLOCK_SIZE, BLUR_BLOCK_SIZE), 0);
	equalizeHist(treated, treated);
	
	///cautions
	threshold(treated, treated, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	
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

vector<vector<point2f> > linearApprox(vector<vector<point> > contour)
{
	vector<point2f> lineared;
	contour.convertTo(lineared, CV_32FC2);
	double epsilon = EPSILON_RATE * arcLength(lineared, true);
	approxPloyDP(lineared, lineared, epsilon, true);
	return lineared;
}	

int main(int argc, char** argv )
{
    if ( argc != 2 )
    {
        std::cout<<"usage: DisplayImage.out <Image_Path>\n";
        return -1;
    }

    Mat image;
    image = imread( argv[1], 1 );

    if ( !image.data )
    {
        std::cout<<"No image data \n";
        return -1;
    }
    //namedWindow("Display Image", WINDOW_AUTOSIZE );
    //imshow("Display Image", image);
 	Mat out;
	out = pretreat(image); 
	namedWindow("display", WINDOW_AUTOSIZE);
	imshow("Display Image", out);
    waitKey(0);

    return 0;
}

