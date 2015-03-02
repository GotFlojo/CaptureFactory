#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "videosynth.h"
#include "chess.h"
using namespace cv;

int main(int argc, char *argv[])
{
    VideoSynth myVideosynth = VideoSynth(0.5);
    Chess myChess;
    myChess.makePoints();
    //Mat img(640,480, CV_8U);
    Mat img;
    //myChess.render(img);
    while(true)
    {
        //myVideosynth.read(img);
        //myChess.render(img);
        myChess.read(img);
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



