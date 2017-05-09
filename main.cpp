#include <iostream>
#include "nms.h"

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

    int testInd = 3;

    switch (testInd)
    {
    case 0:
        srcRects.push_back(cv::Rect(cv::Point(114, 60), cv::Point(178, 124)));
        srcRects.push_back(cv::Rect(cv::Point(120, 60), cv::Point(184, 124)));
        srcRects.push_back(cv::Rect(cv::Point(114, 66), cv::Point(178, 130)));
        break;

    case 1:
        srcRects.push_back(cv::Rect(cv::Point(12, 84), cv::Point(140, 212)));
        srcRects.push_back(cv::Rect(cv::Point(24, 84), cv::Point(152, 212)));
        srcRects.push_back(cv::Rect(cv::Point(12, 96), cv::Point(140, 224)));
        srcRects.push_back(cv::Rect(cv::Point(36, 84), cv::Point(164, 212)));
        srcRects.push_back(cv::Rect(cv::Point(24, 96), cv::Point(152, 224)));
        srcRects.push_back(cv::Rect(cv::Point(24, 108), cv::Point(152, 236)));
        break;

    case 2:
        srcRects.push_back(cv::Rect(cv::Point(12, 30), cv::Point(76, 94)));
        srcRects.push_back(cv::Rect(cv::Point(12, 36), cv::Point(76, 100)));
        srcRects.push_back(cv::Rect(cv::Point(72, 36), cv::Point(200, 164)));
        srcRects.push_back(cv::Rect(cv::Point(84, 48), cv::Point(212, 176)));
        srcRects.push_back(cv::Rect(cv::Point(1, 1), cv::Point(40, 40)));
        break;

    case 3:
        srcRects.push_back(cv::Rect(cv::Point(12, 30), cv::Point(76, 94)));
        scores.push_back(0.7f);

        srcRects.push_back(cv::Rect(cv::Point(12, 36), cv::Point(76, 110)));
        scores.push_back(0.5f);

        srcRects.push_back(cv::Rect(cv::Point(72, 36), cv::Point(200, 164)));
        scores.push_back(0.3f);

        srcRects.push_back(cv::Rect(cv::Point(84, 58), cv::Point(212, 186)));
        scores.push_back(0.4f);
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
    }
    else
    {
        for (auto r : srcRects)
        {
            cv::rectangle(img, r, cv::Scalar(0, 0, 255), 2);
        }
    }

    cv::namedWindow("before", cv::WINDOW_NORMAL);
    cv::imshow("before", img);
    cv::waitKey(1);

    std::vector<cv::Rect> resRects;
    if (srcRects.size() == scores.size())
    {
        nms2(srcRects, scores, resRects, 0.3f, 1);
    }
    else
    {
        nms(srcRects, resRects, 0.3f, 1);
    }

    for (auto r : resRects)
    {
        cv::rectangle(imgCopy, r, cv::Scalar(0, 255, 0), 2);
    }

    cv::namedWindow("after", cv::WINDOW_NORMAL);
    cv::imshow("after", imgCopy);

    cv::waitKey(0);

    return 0;
}
