#include <iostream>
#include "nms.h"

///
enum class TestCases
{
	OnlyRectangles0,
	OnlyRectangles1,
	OnlyRectangles2,
	RectanglesScores,
	RectanglesScoresMinSum,
	SoftNMS
};

/**
 * @brief main
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char* argv[])
{
    std::vector<cv::Rect> srcRects;
    std::vector<float> scores;

	TestCases testInd = TestCases::SoftNMS;
    float minScoresSum = 0;

    switch (testInd)
    {
    case TestCases::OnlyRectangles0:  // Only rectangles
        srcRects.emplace_back(cv::Point(114, 60), cv::Point(178, 124));
        srcRects.emplace_back(cv::Point(120, 60), cv::Point(184, 124));
        srcRects.emplace_back(cv::Point(114, 66), cv::Point(178, 130));
        break;

    case TestCases::OnlyRectangles1:  // Only rectangles
        srcRects.emplace_back(cv::Point(12, 84), cv::Point(140, 212));
        srcRects.emplace_back(cv::Point(24, 84), cv::Point(152, 212));
        srcRects.emplace_back(cv::Point(12, 96), cv::Point(140, 224));
        srcRects.emplace_back(cv::Point(36, 84), cv::Point(164, 212));
        srcRects.emplace_back(cv::Point(24, 96), cv::Point(152, 224));
        srcRects.emplace_back(cv::Point(24, 108), cv::Point(152, 236));
        break;

    case TestCases::OnlyRectangles2:  // Only rectangles
        srcRects.emplace_back(cv::Point(12, 30), cv::Point(76, 94));
        srcRects.emplace_back(cv::Point(12, 36), cv::Point(76, 100));
        srcRects.emplace_back(cv::Point(72, 36), cv::Point(200, 164));
        srcRects.emplace_back(cv::Point(84, 48), cv::Point(212, 176));
        srcRects.emplace_back(cv::Point(1, 1), cv::Point(40, 40));
        break;

    case TestCases::RectanglesScores:  // Rectangles and scores
        srcRects.emplace_back(cv::Point(12, 30), cv::Point(76, 94));
        scores.emplace_back(0.7f);

        srcRects.emplace_back(cv::Point(12, 36), cv::Point(76, 110));
        scores.emplace_back(0.5f);

        srcRects.emplace_back(cv::Point(72, 36), cv::Point(200, 164));
        scores.emplace_back(0.3f);

        srcRects.emplace_back(cv::Point(84, 58), cv::Point(212, 186));
        scores.emplace_back(0.4f);
        break;

    case TestCases::RectanglesScoresMinSum:  // Rectangles, scores and minimum score sum
        srcRects.emplace_back(cv::Point(12, 30), cv::Point(76, 94));
        scores.emplace_back(0.7f);

        srcRects.emplace_back(cv::Point(12, 36), cv::Point(76, 110));
        scores.emplace_back(0.5f);

        srcRects.emplace_back(cv::Point(72, 36), cv::Point(200, 164));
        scores.emplace_back(0.3f);

        srcRects.emplace_back(cv::Point(84, 58), cv::Point(212, 186));
        scores.emplace_back(0.4f);

        minScoresSum = 0.8f;
        break;

	case TestCases::SoftNMS: // Soft nms
		srcRects.emplace_back(cv::Point(12, 30), cv::Point(76, 94));
		scores.emplace_back(0.9f);

		srcRects.emplace_back(cv::Point(12, 36), cv::Point(76, 110));
		scores.emplace_back(0.7f);

		srcRects.emplace_back(cv::Point(72, 36), cv::Point(200, 164));
		scores.emplace_back(0.6f);

		srcRects.emplace_back(cv::Point(84, 58), cv::Point(212, 186));
		scores.emplace_back(0.5f);
		break;
    }

    cv::Size size(0, 0);
    for (const auto& r : srcRects)
    {
        size.width = std::max(size.width, r.x + r.width);
        size.height = std::max(size.height, r.y + r.height);
    }

    cv::Mat img = cv::Mat(2 * size.height, 2 * size.width, CV_8UC3, cv::Scalar(0, 0, 0));

    cv::Mat imgCopy = img.clone();


    if (srcRects.size() == scores.size())
    {
        for (size_t i = 0; i < srcRects.size(); ++i)
        {
            const auto& r = srcRects[i];

            cv::rectangle(img, r, cv::Scalar(0, 0, 255), 2);
            cv::putText(img, std::to_string(scores[i]), cv::Point(r.x + 2, r.y + r.height - 4), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(200, 200, 200), 1, cv::LINE_8, false);
        }
        if (minScoresSum > 0)
        {
            cv::putText(img,
                        std::string("min scores sum = ") + std::to_string(minScoresSum),
                        cv::Point(10, img.rows - 4),
                        cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(200, 200, 200), 1, cv::LINE_8, false);
        }
    }
    else
    {
        for (auto r : srcRects)
        {
            cv::rectangle(img, r, cv::Scalar(0, 0, 255), 2);
        }
    }

    cv::namedWindow("before", cv::WINDOW_AUTOSIZE);
    cv::imshow("before", img);
    cv::waitKey(1);

    std::vector<cv::Rect> resRects;
	std::vector<float> resScores;
    
	switch (testInd)
	{
	case TestCases::OnlyRectangles0:
	case TestCases::OnlyRectangles1:
	case TestCases::OnlyRectangles2:
		nms(srcRects, resRects, 0.3f, 1);
		break;

	case TestCases::RectanglesScores:
	case TestCases::RectanglesScoresMinSum:
		nms2(srcRects, scores, resRects, 0.3f, 1, minScoresSum);
		break;

	case TestCases::SoftNMS:
		soft_nms(srcRects, scores, resRects, resScores, 0.3f, 0.5f, Methods::GaussNMS, 0.5f);
		break;
	}

    for (auto r : resRects)
    {
        cv::rectangle(imgCopy, r, cv::Scalar(0, 255, 0), 2);
    }

    cv::namedWindow("after", cv::WINDOW_AUTOSIZE);
    cv::imshow("after", imgCopy);

    cv::waitKey(0);

    return 0;
}
