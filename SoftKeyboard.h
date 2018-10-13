#ifndef SOFTKEYBOARD_H
#include <string>
#include <unordered_map>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>

namespace bs
{
    class SoftKeyboard
    {
    public:
        SoftKeyboard();
        ~SoftKeyboard();

        bool Show(cv::Mat& background, std::string* outputStr);

    private:
        typedef struct _keyLocation {
            cv::Point leftUpCorner;
            int width;
            int height;
            _keyLocation(int xx, int yy, int ww, int hh)
            {
                leftUpCorner = cv::Point(xx, yy);
                width = ww;
                height = hh;
            }
            _keyLocation operator = (const _keyLocation& other) 
            {
                leftUpCorner = other.leftUpCorner;
                width = other.width;
                height = other.height;
                return *this;
            }
        }KeyLocation;

    private:
        cv::Mat m_keyboardImg;
        std::unordered_map<std::string, KeyLocation> m_keyToLocationMap;
    };
}
#endif