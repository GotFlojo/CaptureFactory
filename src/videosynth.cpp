#include "videosynth.h"

VideoSynth::VideoSynth(float noise)
{
    this->m_noise = noise;
    this->m_frameSize=cv::Size(640, 480);
}

VideoSynth::VideoSynth(cv::Size frame_size, float noise) : VideoSynth(noise)
{

    this->m_frameSize = frame_size;
}

VideoSynth::VideoSynth(std::string bg, float noise) : VideoSynth(noise)
{
    if(!bg.empty())
    {
        this->m_bg = cv::imread(bg, 1);
        this->m_frameSize=this->m_bg.size();
    }

}

VideoSynth::VideoSynth(cv::Size frame_size, std::string bg, float noise) : VideoSynth(bg,noise)
{
    this->m_frameSize=frame_size;
}

VideoSynth::~VideoSynth() {}

bool VideoSynth::isOpenend()
{
    return true;
}

void VideoSynth::render(cv::Mat& dst)
{
}


bool VideoSynth::read(cv::Mat& dst)
{
    cv::Mat buf;
    if(m_bg.empty())
    {
        buf=cv::Mat::zeros(m_frameSize, CV_8UC3);
    }
    else
    {
        m_bg.copyTo(buf);
    }

    render(buf);

    if(m_noise > 0.0)
    {
        cv::Mat noise = cv::Mat::zeros(m_frameSize, CV_8SC3);
        cv::randn(noise, cv::Mat::zeros(1,3, CV_8UC1), cv::Mat(1,3, CV_8UC1, cv::Scalar(255*m_noise)));
        cv::add(buf, noise, buf, cv::noArray(), CV_8UC3);
    }
    buf.copyTo(dst);
    return true;
}
