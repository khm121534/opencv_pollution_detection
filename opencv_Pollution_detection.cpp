// opencv_Pollution_detection.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;


int main()
{
    // Image Load
    Mat img = imread("./data/bottle/test/contamination/020.png");
    //Mat img = imread("./data/bottle/train/good/000.png");
    if (img.empty()) {
        cout << "이미지 로드 실패\n";
        return -1;
    }

    // ROI 
    const int RADIUS = 190;
    const int CENTER_X = 450;
    const int CENTER_Y = 460;

    Rect roi_rect(CENTER_X - RADIUS, CENTER_Y - RADIUS, RADIUS * 2, RADIUS * 2);
    
    Mat square_roi = img(roi_rect);
 

    Mat mask = Mat::zeros(square_roi.size(), CV_8UC1);
    Point mask_center(RADIUS, RADIUS);

    circle(mask, mask_center, RADIUS, Scalar(255), -1);

    Mat circular_roi;
    square_roi.copyTo(circular_roi, mask);
    circle(img, Point(CENTER_X, CENTER_Y), RADIUS, Scalar(0, 0, 255), 2);


    // Feature Extraction
    // grayscale
    Mat roi_gray;
    if (circular_roi.channels() == 3) {
        cvtColor(circular_roi, roi_gray, COLOR_BGR2GRAY);
    }
    else {
        roi_gray = circular_roi;
    }


    // 통계값 계산
    Scalar mean_val, stddev_val;
    meanStdDev(roi_gray, mean_val, stddev_val);
    

    double avg_brightness = mean_val[0];    // 평균 밝기
    double std_deviation = stddev_val[0];   // 표준 편차



    cout << "평균 밝기 : " << avg_brightness << endl;
    cout << "표준 편차 : " << stddev_val << endl;


    // 양/불여부 판정
    const double BRIGHTNESS_THRESHOLD = 30.0;
    const double STD_DEV_THRESHOLD = 17.0;

    // 양/불여부 판정 (평균 밝기가 높거나, 표준 편차가 높으면 불량)
    if (avg_brightness > BRIGHTNESS_THRESHOLD || std_deviation > STD_DEV_THRESHOLD) {
        cout << "판정: 불량 입니다! (DEFECT)\n";
    }
    else {
        cout << "판정: 정상 입니다! (GOOD)\n";
    }
  
    // 이미지 출력
    imshow("Original", img);
    imshow("ROI", circular_roi);
    waitKey(0);

    return 0;
}

