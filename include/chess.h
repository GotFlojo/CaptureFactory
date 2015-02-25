#ifndef CHESS_H
#define CHESS_H

#include <videosynth.h>


class Chess : public VideoSynth
{
    public:
        Chess();
        void render(cv::Mat& dst);
    protected:
        void drawQuads();
    private:
};

#endif // CHESS_H
