#ifndef VIDEOSYNTH_H
#define VIDEOSYNTH_H

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

class VideoSynth
{
    public:

        VideoSynth(float noise=0.0);
        VideoSynth(cv::Size frame_size, float noise=0.0);
        VideoSynth(std::string bg, float noise=0.0);
        VideoSynth(cv::Size frame_size, std::string bg="", float noise=0.0);
        virtual ~VideoSynth();


        virtual bool read(cv::Mat& dst);
        bool isOpenend();
    private:
        float m_noise;
        cv::Mat m_bg;
        cv::Size m_frameSize;

        virtual void render(cv::Mat&);
};

#endif // VIDEOSYNTH_H
