#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "videosynth.h"
using namespace cv;

int main(int argc, char *argv[])
{
    VideoSynth myVideosynth = VideoSynth(0.5);
    Mat img;
    while(true)
    {
        myVideosynth.read(img);
        cv::imshow("capture", img);
        // Press Escape to End
        char c = (char)cv::waitKey(1);
		if (c == 27)
		{
			break;
		}
    }
    return 0;
}



