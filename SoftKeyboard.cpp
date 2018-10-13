#include "SoftKeyboard.h"
#include <fstream>
#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include "json.hxx"
#include "cvui.h"
namespace nl = nlohmann;
        
namespace bs
{
    SoftKeyboard::SoftKeyboard()
    {
        std::string keyboardConfig = "./keyMapping.json";
        std::ifstream myStream(keyboardConfig);
        if (!myStream.is_open())
        {
            std::cout << "Error.can't open " << keyboardConfig << std::endl;
            return;
        }
        nl::json js;
        myStream >> js;
        for (auto& it = js.begin(); it != js.end(); ++it)
        {
            m_keyToLocationMap.insert(std::make_pair(it.key(), KeyLocation(it.value()[0], it.value()[1], it.value()[2], it.value()[3])));
        }
        std::string keyboardImgFile = "./soft_keyboard.png";
        m_keyboardImg = cv::imread(keyboardImgFile);
    }


    SoftKeyboard::~SoftKeyboard()
    {
    }

    bool SoftKeyboard::Show(cv::Mat& background, std::string* outputStr)
    {
        bool inputDone = false;
        float scale = (float)background.cols / m_keyboardImg.cols;
        int kbHeight = int(scale * m_keyboardImg.rows);
        cv::Mat scaledKb;
        cv::resize(m_keyboardImg, scaledKb, cv::Size(background.cols, kbHeight));
        auto scaledKeyToLocationMap = m_keyToLocationMap;
        std::for_each(scaledKeyToLocationMap.begin(), scaledKeyToLocationMap.end(), [scale](auto& keyToLocationPair)
        {
            keyToLocationPair.second.leftUpCorner.x = int(keyToLocationPair.second.leftUpCorner.x  * scale);
            keyToLocationPair.second.leftUpCorner.y = int(keyToLocationPair.second.leftUpCorner.y * scale);
            keyToLocationPair.second.width = int(keyToLocationPair.second.width * scale);
            keyToLocationPair.second.height = int(keyToLocationPair.second.height * scale);
        });
        cv::Point kbGlobalLeftUpPos(0, background.rows - scaledKb.rows);
        cvui::image(background, kbGlobalLeftUpPos.x, kbGlobalLeftUpPos.y, scaledKb);
        for (auto& keyToLocationPair : scaledKeyToLocationMap)
        {
            int gxx = kbGlobalLeftUpPos.x + keyToLocationPair.second.leftUpCorner.x;
            int gyy = kbGlobalLeftUpPos.y + keyToLocationPair.second.leftUpCorner.y;
            cv::Rect keyRect = cv::Rect(cv::Point(0,0), scaledKb.size()) & cv::Rect(keyToLocationPair.second.leftUpCorner.x, keyToLocationPair.second.leftUpCorner.y,
                keyToLocationPair.second.width, keyToLocationPair.second.height);
            cv::Mat keyMat = cv::Mat(scaledKb, keyRect);
            cv::Mat keyDownMat;
            keyMat.convertTo(keyDownMat, -1, 0.5, 0.3);
            if (cvui::button(background, gxx, gyy, keyMat, keyMat, keyDownMat))
            {
                std::cout << "key:" << keyToLocationPair.first << " is pressed" << std::endl;
                if (keyToLocationPair.first.size() == 1 && keyToLocationPair.first[0] >= 'a' && keyToLocationPair.first[0] <= 'z')
                {
                    outputStr->push_back(keyToLocationPair.first[0]);
                }
                else if (keyToLocationPair.first == "space")
                {
                    outputStr->push_back(' ');
                }
                else if (keyToLocationPair.first == "delete" && !outputStr->empty())
                {
                    outputStr->pop_back();
                }
                else if (keyToLocationPair.first == "enter" || keyToLocationPair.first == "hide")
                {
                    inputDone = true;
                    break;
                }
                //"shift":[3, 228, 181, 90],
                //    "delete" : [1104, 229, 173, 90],
                //    "hide" : [1, 13, 119, 90],
                //    "num" : [263, 339, 243, 90],
                //    "space" : [454, 338, 372, 90],
                //    "comma" : [835, 339, 179, 90],
                //    "enter" : [1030, 339, 241, 90]
            }
        }
        return inputDone;

    }

}