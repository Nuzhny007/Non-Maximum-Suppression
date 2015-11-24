#include <iostream>
#include <string>
#include <time.h>

#include <opencv2/opencv.hpp>

void nms(
	const std::vector<cv::Rect>& srcRects,
	std::vector<cv::Rect>& resRects,
	float thresh
	)
{
	resRects.clear();

	const size_t size = srcRects.size();

    // If there are no boxes, return an empty list
	if (!size)
	{
		return;
	}

	// Grab the coordinates of the bounding boxes
	std::vector<int> x1(size);
	std::vector<int> y1(size);
	std::vector<int> x2(size);
	std::vector<int> y2(size);
	std::vector<float> area(size);
	std::multimap<int, size_t> idxs; // Sort the bounding boxes by the bottom - right y - coordinate of the bounding box

	for (size_t i = 0; i < size; ++i)
	{
		const cv::Rect& r = srcRects[i];

		x1[i] = r.x;
		y1[i] = r.y;
		x2[i] = r.x + r.width;
		y2[i] = r.y + r.height;
		area[i] = static_cast<float>(r.width * r.height);
		idxs.insert(std::pair<int, size_t>(y2[i], i));
	}
	
	// keep looping while some indexes still remain in the indexes list
	while (idxs.size() > 0)
	{
		// grab the last index in the indexes list, add the index value to the list of picked indexes, then initialize the suppression list(i.e.indexes that will be deleted) using the last index
		auto lastElem = --std::end(idxs);
		const size_t i = lastElem->second;

		resRects.push_back(srcRects[i]);

		idxs.erase(lastElem);

		// loop over all indexes in the indexes list
		for (auto pos = std::begin(idxs); pos != std::end(idxs); )
		{
			// grab the current index
			const size_t j = pos->second;
			
			// find the largest(x, y) coordinates for the start of the bounding box and the smallest(x, y) coordinates for the end of the bounding box
			int xx1 = std::max(x1[i], x1[j]);
			int yy1 = std::max(y1[i], y1[j]);
			int xx2 = std::min(x2[i], x2[j]);
			int yy2 = std::min(y2[i], y2[j]);
			
			// compute the width and height of the bounding box
			int w = std::max(0, xx2 - xx1 + 1);
			int h = std::max(0, yy2 - yy1 + 1);

			// compute the ratio of overlap between the computed bounding box and the bounding box in the area list
			float overlap = (w * h) / area[j];

			// if there is sufficient overlap, suppress the current bounding box
			if (overlap > thresh)
			{
				pos = idxs.erase(pos);
			}
			else
			{
				++pos;
			}
		}
	}
}


/**
*******************************************************************************
*
*   main
*
*******************************************************************************
*/
int main(int argc, char* argv[])
{
	std::vector<cv::Rect> srcRects;

	/*
	// Test 1
	srcRects.push_back(cv::Rect(cv::Point(114, 60), cv::Point(178, 124)));
	srcRects.push_back(cv::Rect(cv::Point(120, 60), cv::Point(184, 124)));
	srcRects.push_back(cv::Rect(cv::Point(114, 66), cv::Point(178, 130)));*/

	/*
	// Test 2
	srcRects.push_back(cv::Rect(cv::Point(12, 84), cv::Point(140, 212)));
	srcRects.push_back(cv::Rect(cv::Point(24, 84), cv::Point(152, 212)));
	srcRects.push_back(cv::Rect(cv::Point(12, 96), cv::Point(140, 224)));
	srcRects.push_back(cv::Rect(cv::Point(36, 84), cv::Point(164, 212)));
	srcRects.push_back(cv::Rect(cv::Point(24, 96), cv::Point(152, 224)));
	srcRects.push_back(cv::Rect(cv::Point(24, 108), cv::Point(152, 236)));*/

	// Test 3
	srcRects.push_back(cv::Rect(cv::Point(12, 30), cv::Point(76, 94)));
	srcRects.push_back(cv::Rect(cv::Point(12, 36), cv::Point(76, 100)));
	srcRects.push_back(cv::Rect(cv::Point(72, 36), cv::Point(200, 164)));
	srcRects.push_back(cv::Rect(cv::Point(84, 48), cv::Point(212, 176)));

	cv::Size size(0, 0);
	for (const auto& r : srcRects)
	{
		size.width = std::max(size.width, r.x + r.width);
		size.height = std::max(size.height, r.y + r.height);
	}

	cv::Mat img = cv::Mat(2 * size.height, 2 * size.width, CV_8UC3, cv::Scalar(0, 0, 0));

	cv::Mat imgCopy = img.clone();

	

	for (auto r : srcRects)
	{
		cv::rectangle(img, r, cv::Scalar(0, 0, 255), 2);
	}

	cv::namedWindow("before", cv::WINDOW_NORMAL);
	cv::imshow("before", img);
	cv::waitKey(1);

	std::vector<cv::Rect> resRects;
	nms(srcRects, resRects, 0.3f);

	for (auto r : resRects)
	{
		cv::rectangle(imgCopy, r, cv::Scalar(0, 255, 0), 2);
	}

	cv::namedWindow("after", cv::WINDOW_NORMAL);
	cv::imshow("after", imgCopy);

	cv::waitKey(0);

	return 0;
}
