#include <iostream>
#include <cmath>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
class DetectRes
{
	
}

const int BLUR_BLOCK_SIZE = 5;
using namespace cv;
/* Return if absolute value of X is within the range of [MIN, MAX]. */
bool in_range(double x, double min, double max)
{
	x = fabs(x);
	return (min <= x) && (x <= max);
}

Mat pretreat(Mat original)
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
Point get_centroid(MatOfPoint contour)
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
