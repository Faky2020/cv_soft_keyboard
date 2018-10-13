#define CVUI_IMPLEMENTATION

#include "SoftKeyboard.h"
#include <string>
#include <iostream>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "cvui.h"

int main()
{
    cv::Mat backgroundImg = cv::imread("./background.jpg");
    std::cout << "resolution:" << backgroundImg.cols << "x" << backgroundImg.rows << std::endl;
    bs::SoftKeyboard keyboard;
    const std::string WINDOW_NAME = "test";
    cvui::init(WINDOW_NAME);
    std::string inputText = "";
    bool inputDone = false;
    while (true)
    {
        cv::Mat imgForRender = backgroundImg.clone();
        if(!inputDone)
            inputDone = keyboard.Show(imgForRender, &inputText);
        cvui::text(imgForRender, 0, 30, inputText, 3, 0xFF1CAE);
        cvui::update();
        cv::imshow(WINDOW_NAME, imgForRender) ;
        int keycode = cv::waitKey(1);
        if (keycode == 27)
            break;
    };
    std::cout << "done" << std::endl;
    return 0;
}

