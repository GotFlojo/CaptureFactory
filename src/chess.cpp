#include "chess.h"

Chess::Chess() = default;

void Chess::render(cv::Mat& dst)
{
    //std::cout << dst.size() << std::endl;
    float time = m_t;
    m_t += 1.0/30.0;

     //size of checkerboard
    unsigned int sx = m_gridSize.width, sy = m_gridSize.height;
    //cv::Point3f center = cv::Point3f(0.5f*sx, 0.5f*sy, 0.0f);
    cv::Point3f center = cv::Point3f(0.5f*sx, 0.5f*sy, 0.0f);
    float phi = PI/3.0 + sin(time*3)*PI/8.0;
    float c = cos(phi), s = sin(phi);
    cv::Point3f offset = cv::Point3f(sin(1.2*time), cos(1.8*time), 0);
    offset *= sx*0.2;
    cv::Point3f eyePos= center + (cv::Point3f(cos(time)*c, sin(time)*c, s)*15.0) + offset;
    cv::Point3f targetPos = center + offset;

    cv::Point3f up = cv::Point3f(0, 0 ,1.0);
    lookat(eyePos, targetPos, up, m_R, m_tvec);
    mtx2rvec(m_R,m_rvec);

    drawQuads(dst, m_whiteQuads,cv::Scalar(245,245,245));
    drawQuads(dst, m_blackQuads,cv::Scalar(10,10,10));
}

void Chess::drawQuads(cv::Mat& img, cv::Mat& quads, cv::Scalar color)
{
        cv::Mat imageQuads;
        float shape_factor = quads.size().width/3.0;
        //std::cout << quads.size() << std::endl;
        cv::Mat r3Quads = quads.reshape(1, quads.size().height*shape_factor);
        r3Quads.convertTo(r3Quads, CV_64FC1);
        //std::cout << r3Quads << std::endl;
        //void projectPoints(InputArray objectPoints, InputArray rvec, InputArray tvec, InputArray cameraMatrix, InputArray distCoeffs, OutputArray imagePoints)
        cv::projectPoints(r3Quads, m_rvec*-1, m_tvec*-1, m_cameraMatrix, m_dist_coeff, imageQuads);
        //imageQuads is 2 channel, 1 column, every row equal a set of x-y-coordinates
        //std::cout << imageQuads.size() << imageQuads.channels() << std::endl;
        //std::cout << imageQuads << std::endl;

        cv::Point quadPoints[4];
        //4 rows equal 4 corner points of a quad -> iterate all the points quadwise
        for(size_t quad = 0; quad < (imageQuads.rows); quad+=4)
        {
            //for every 4 coordinates
            for(size_t point = 0; point < 4; ++point)
            {
                //we can access the mat as Point 2d since it is 2 channel
                cv::Point2d quadPoint= imageQuads.at<cv::Point2d>(quad+point);
                quadPoints[point] = quadPoint*4;
            }
            cv::fillConvexPoly(img, quadPoints, 4, color, CV_AA, 2);
        }
}

void Chess::makePoints()
{
    //size of checkerboard
    unsigned int sx=m_gridSize.width, sy=m_gridSize.height;
    //iterate through the points that make up the board
    //for(unsigned int y=0; y<sy; ++y)
    for(unsigned int y=0; y<sy; ++y)
    {
        //for(unsigned int x=0; x<sx; ++x)
        for(unsigned int x=0; x<sx; ++x)
        {
            //four points make up a quad. Each point is a 3D point with it's z coordinate set to zero
            cv::Mat M = (cv::Mat_<int>(1,12) << x, y, 0, x+1, y, 0, x+1, y+1, 0, x, y+1 ,0);
            //are these points for white or black quads?
            if((x+y)%2)
            {
                m_blackQuads.push_back(M);
            }
            else
            {
                m_whiteQuads.push_back(M);
            }
        }
    }
//    std::cout<< blackQuads.size()  << whiteQuads.size() << std::endl;
//    std::cout << whiteQuads << std::endl;
//    whiteQuads.reshape(0, 35*(12/3));
//    int numQuads=whiteQuads.rows;
//    std::cout << whiteQuads.reshape(0, (numQuads*(whiteQuads.cols/3))) << std::endl;
}

void Chess::lookat(cv::Point3f& eye, cv::Point3f& target, cv::Point3f& up, cv::Mat& R, cv::Mat& tvec)
{
     cv::Point3f fwd = target - eye;
     fwd *= 1.0/cv::norm(fwd);
     cv::Point3f right = fwd.cross(up);
     right *=1.0/cv::norm(right);
     cv::Point3f down = fwd.cross(right);

     float m[3][3] = {{right.x, right.y, right.z}, {down.x, down.y, down.y}, {fwd.x, fwd.y, fwd.z}};
     R = (cv::Mat_<double>(3,3) << right.x, right.y, right.z,
                                            down.x, down.y, down.z,
                                            fwd.x, fwd.y, fwd.z);
     cv::Mat eyeMat = (cv::Mat_<double>(3,1) << eye.x, eye.y, eye.z);
     tvec = R*eyeMat;
//
//     std::cout << R << std::endl;
//     std::cout << tvec << std::endl;
}

void Chess::mtx2rvec(cv::Mat& R, cv::Mat& axis)
{
    cv::Mat w, u, vt;
    //std::cout << R << std::endl;
    //std::cout << R-cv::Mat::eye(3,3,CV_64F) << std::endl;
    R-cv::Mat::eye(3,3, CV_64F);
    cv::SVD::compute((R-cv::Mat::eye(3,3, CV_64F)), w, u, vt);

    cv::Mat p = R*vt.row(0).t();
    cv::Mat c = vt.row(0)*p;
    cv::Mat s = vt.row(1)*p;
    axis= vt.row(0).cross(vt.row(1));
    axis *= atan2(s.at<double>(0), c.at<double>(0));
    //std::cout << vt << std::endl;
    //std::cout << p << std::endl;
    //std::cout << c << std::endl;
    //std::cout << s << std::endl;
    //std::cout << axis<< std::endl;
}
