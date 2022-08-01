#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

std::vector<std::vector<int>> Colors { {0,179,226,255,153,255} };

std::vector<cv::Scalar> RGBColors
{
	{255, 0, 0}
};

cv::Mat img;

void GetContours(cv::Mat& maskImg, cv::Point& borderPoint)
{
	const unsigned int minArea = 200;

	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;

	cv::findContours(maskImg, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

	std::vector<std::vector<cv::Point>> conPoly(contours.size());
	std::vector<cv::Rect> boundRect(contours.size());

	for (int i = 0; i < contours.size(); i++)
	{
		int area = cv::contourArea(contours[i]);

		if (area > minArea)
		{
			float peri = cv::arcLength(contours[i], true);

			cv::approxPolyDP(contours[i], conPoly[i], .02 * peri, true);

			boundRect[i] = cv::boundingRect(conPoly[i]);

			borderPoint.x = boundRect[i].x + boundRect[i].width / 2;
			borderPoint.y = boundRect[i].y;

			cv::drawContours(img, conPoly, i, cv::Scalar(255,0,255), 2);
			cv::rectangle(img, boundRect[i].tl(), boundRect[i].br(), cv::Scalar(0, 255, 0), 5);
		}
	}
}

cv::Point borderPoint(0, 0);

void FindColor(cv::Mat& img)
{
	cv::Mat imgHSV;
	cv::cvtColor(img, imgHSV, cv::COLOR_BGR2HSV);

	for (int i = 0; i < Colors.size(); i++)
	{
		cv::Scalar lowerBound(Colors[i][0], Colors[i][2], Colors[i][4]);
		cv::Scalar upperBound(Colors[i][1], Colors[i][3], Colors[i][5]);

		cv::Mat mask;

		cv::inRange(imgHSV, lowerBound, upperBound, mask);
		cv::imshow(std::to_string(i), mask);


		GetContours(mask, borderPoint);
	}
}

void DrawOnCanvas()
{
	cv::circle(img, borderPoint, 10, RGBColors[0], cv::FILLED);
}

int main()
{ 
	cv::VideoCapture cap(0); // 0 - webcam ID

	while (true)
	{
		cap.read(img);

		FindColor(img);
		DrawOnCanvas();

		cv::imshow("Cap", img);
		cv::waitKey(1);
	}
}