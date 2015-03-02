#ifndef CHESS_H
#define CHESS_H

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <videosynth.h>
#include <iostream>
#include "prettyprint.hpp"
#include <math.h>

# define PI 3.14159265358979323846


class Chess : public VideoSynth
{
    public:
        Chess();
        void render(cv::Mat& dst) override;
    //protected:
        void drawQuads(cv::Mat& img, cv::Mat& quads, cv::Scalar color=cv::Scalar(0, 255, 0));
    //private:
        void makePoints();
        void lookat(cv::Point3f& eye, cv::Point3f& target, cv::Point3f& up, cv::Mat& R, cv::Mat& tvec);
        void mtx2rvec(cv::Mat& R, cv::Mat& axis);

        cv::Mat m_whiteQuads, m_blackQuads, m_R, m_tvec, m_rvec;;
        cv::Size m_gridSize=cv::Size(10,7);
        std::vector<double> m_dist_coeff = {-0.2, 0.1, 0, 0};
        float m_fx=0.9;
        float m_t=0;
        cv::Mat m_cameraMatrix = (cv::Mat_<double>(3,3) << m_fx*m_frameSize.width, 0, 0.5*(m_frameSize.width-1),
                                                          0, m_fx*m_frameSize.width, 0.5*(m_frameSize.height-1),
                                                          0.0, 0.0, 1.0);

};

#endif // CHESS_H
