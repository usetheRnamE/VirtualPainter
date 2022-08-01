#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

cv::Mat imgHSV, mask, imgColor;
struct ColorValues
{
	int hue, sat, val;
} Max, Min;

cv::VideoCapture cap(0);
cv::Mat img;

int main()
{
	cv::namedWindow("Trackbars", (640, 200));

	cv::createTrackbar("Hue Min", "Trackbars", &Min.hue, 179);
	cv::createTrackbar("Hue Max", "Trackbars", &Max.hue, 179);
	cv::createTrackbar("Sat Min", "Trackbars", &Min.sat, 255);
	cv::createTrackbar("Sat Max", "Trackbars", &Max.sat, 255);
	cv::createTrackbar("Val Min", "Trackbars", &Min.val, 255);
	cv::createTrackbar("Val Max", "Trackbars", &Max.val, 255);

	while (true)
	{
		cap.read(img);

		cv::cvtColor(img, imgHSV, cv::COLOR_BGR2HSV);

		cv::Scalar lowerBound(Min.hue, Min.sat, Min.val);
		cv::Scalar upperBound(Max.hue, Max.sat, Max.val);

		cv::inRange(imgHSV, lowerBound, upperBound, mask);

		cv::imshow("Image", img);
		cv::imshow("Mask", mask);

		cv::waitKey(1);
	}
}
